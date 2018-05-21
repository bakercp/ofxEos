//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // a Morphable Model stored as cereal BinaryArchive.
    std::string modelfile = ofToDataPath("sfm_shape_3448.bin", true);
    std::string mappingsfile = ofToDataPath("ibug_to_sfm.txt", true);

    morphable_model = eos::morphablemodel::load_model(modelfile);
    landmark_mapper = eos::core::LandmarkMapper(mappingsfile);

    grabber.setup(1280, 720);
    
    // Setup tracker
    tracker.setup();

}



void ofApp::update()
{
    grabber.update();
    
    // Update tracker when there are new frames
    if (grabber.isFrameNew())
    {
        tracker.update(grabber);
        
        auto instances = tracker.getInstances();
        
        if (!instances.empty())
        {
            auto& _landmarks = instances[0].getLandmarks();
            
            eos::core::LandmarkCollection<Eigen::Vector2f> landmarks;

            int ibugId = 1;

            for (auto p: _landmarks.getImagePoints())
            {
                auto landmark = eos::core::Landmark<Eigen::Vector2f>();
                
                landmark.name = std::to_string(ibugId);
                landmark.coordinates[0] = p.x;
                landmark.coordinates[1] = p.y;
//                // From the iBug website:
//                // "Please note that the re-annotated data for this challenge are saved in the Matlab convention of 1
//                // being the first index, i.e. the coordinates of the top left pixel in an image are x=1, y=1."
//                // ==> So we shift every point by 1:
//                landmark.coordinates[0] -= 1.0f;
//                landmark.coordinates[1] -= 1.0f;

                
                landmarks.push_back(landmark);
                ibugId++;
            }
            
            // These will be the final 2D and 3D points used for the fitting:
            std::vector<Eigen::Vector4f> model_points; // the points in the 3D shape model
            std::vector<int> vertex_indices;    // their vertex indices
            std::vector<Eigen::Vector2f> image_points; // the corresponding 2D landmark points
            

            // Sub-select all the landmarks which we have a mapping for (i.e. that are defined in the 3DMM):
            for (int i = 0; i < landmarks.size(); ++i)
            {
                auto converted_name = landmark_mapper.convert(landmarks[i].name);
                if (!converted_name)
                { // no mapping defined for the current landmark
                    continue;
                }
                int vertex_idx = std::stoi(converted_name.value());
                auto vertex = morphable_model.get_shape_model().get_mean_at_point(vertex_idx);
                model_points.emplace_back(Eigen::Vector4f(vertex.x(), vertex.y(), vertex.z(), 1.0f));
                vertex_indices.emplace_back(vertex_idx);
                image_points.emplace_back(landmarks[i].coordinates);
            }

            cv::Mat image = ofxCv::toCv(grabber.getPixels());
            
            // Estimate the camera (pose) from the 2D - 3D point correspondences
            auto pose = eos::fitting::estimate_orthographic_projection_linear(image_points, model_points, true, image.rows);
            auto rendering_params = eos::fitting::RenderingParameters(pose, image.cols, image.rows);
            
            // The 3D head pose can be recovered as follows:
            const float yaw_angle = glm::degrees(glm::yaw(rendering_params.get_rotation()));
            // and similarly for pitch and roll.
            
            // Estimate the shape coefficients by fitting the shape to the landmarks:
            const Eigen::Matrix<float, 3, 4> affine_from_ortho =
            eos::fitting::get_3x4_affine_camera_matrix(rendering_params, image.cols, image.rows);
            const vector<float> fitted_coeffs = eos::fitting::fit_shape_to_landmarks_linear(
                                                                                            morphable_model.get_shape_model(), affine_from_ortho, image_points, vertex_indices);
            
            // Obtain the full mesh with the estimated coefficients:
            const eos::core::Mesh mesh = morphable_model.draw_sample(fitted_coeffs, vector<float>());
            
            _mesh = ofxEos::toOf(mesh);
            
            // Extract the texture from the image using given mesh and camera parameters:
            const eos::core::Image4u isomap = eos::render::extract_texture(mesh, affine_from_ortho, eos::core::from_mat(image));
            
            // Save the mesh as textured obj:
//            std::filesystem::path outputfile = outputbasename + ".obj";
//            eos::core::write_textured_obj(mesh, outputfile.string());
//
//            // And save the isomap:
//            outputfile.replace_extension(".isomap.png");
            
            ofPixels isoMapPix;
            ofxCv::toOf(eos::core::to_mat(isomap), isoMapPix);

            isoMapTex.loadData(isoMapPix);
            
            // ofSaveImage(isoMapPix, outputfile.string());
//            cout << "Finished fitting and wrote result mesh and isomap to files with basename "
//            << outputfile.stem().stem() << "." << endl;
        }
    }

    
    

}


void ofApp::draw()
{
    grabber.draw(0, 0);
    
    if (isoMapTex.isAllocated())
    {
        isoMapTex.draw(0, 0);
    }
    grabber.bind();
    _mesh.draw();
    grabber.unbind();
}
