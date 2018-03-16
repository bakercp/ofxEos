//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    /**
     * This app demonstrates estimation of the camera and fitting of the shape
     * model of a 3D Morphable Model from an ibug LFPW image with its landmarks.
     *
     * First, the 68 ibug landmarks are loaded from the .pts file and converted
     * to vertex indices using the LandmarkMapper. Then, an orthographic camera
     * is estimated, and then, using this camera matrix, the shape is fitted
     * to the landmarks.
     */

    // a Morphable Model stored as cereal BinaryArchive.
    std::string modelfile = ofToDataPath("sfm_shape_3448.bin", true);
    std::string isomapfile = "";
    // an input image
    std::string imagefile = ofToDataPath("image_0010.png", true);
    // 2D landmarks for the image, in ibug .pts format
    std::string landmarksfile = ofToDataPath("image_0010.pts", true);
    // landmark identifier to model vertex number mapping
    std::string mappingsfile = ofToDataPath("ibug_to_sfm.txt", true);
    // basename for the output rendering and obj files
    std::string outputbasename = ofToDataPath("image_0010", true);


    // Load the image, landmarks, LandmarkMapper and the Morphable Model:
    ofPixels pix;
    ofLoadImage(pix, imagefile);
    
    cv::Mat image = ofxCv::toCv(pix);
    
    eos::core::LandmarkCollection<Eigen::Vector2f> landmarks;

    try
    {
        landmarks = eos::core::read_pts_landmarks(landmarksfile);
    } catch (const std::runtime_error& e)
    {
        cout << "Error reading the landmarks: " << e.what() << endl;
        ofExit();
    }
    eos::morphablemodel::MorphableModel morphable_model;
    try
    {
        morphable_model = eos::morphablemodel::load_model(modelfile);
    } catch (const std::runtime_error& e)
    {
        cout << "Error loading the Morphable Model: " << e.what() << endl;
        ofExit();
    }
    // The landmark mapper is used to map 2D landmark points (e.g. from the ibug scheme) to vertex ids:
    eos::core::LandmarkMapper landmark_mapper;
    try
    {
        landmark_mapper = eos::core::LandmarkMapper(mappingsfile);
    } catch (const std::exception& e)
    {
        cout << "Error loading the landmark mappings: " << e.what() << endl;
        ofExit();
    }

    // Draw the loaded landmarks:
    cv::Mat outimg = image.clone();
    for (auto&& lm : landmarks)
    {
        cv::rectangle(outimg, cv::Point2f(lm.coordinates[0] - 2.0f, lm.coordinates[1] - 2.0f),
                      cv::Point2f(lm.coordinates[0] + 2.0f, lm.coordinates[1] + 2.0f), {255, 0, 0});
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

    // Estimate the camera (pose) from the 2D - 3D point correspondences
    eos::fitting::ScaledOrthoProjectionParameters pose =
    eos::fitting::estimate_orthographic_projection_linear(image_points, model_points, true, image.rows);
    eos::fitting::RenderingParameters rendering_params(pose, image.cols, image.rows);

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

    // Extract the texture from the image using given mesh and camera parameters:
    const eos::core::Image4u isomap = eos::render::extract_texture(mesh, affine_from_ortho, eos::core::from_mat(image));

    // Save the mesh as textured obj:
    std::filesystem::path outputfile = outputbasename + ".obj";
    eos::core::write_textured_obj(mesh, outputfile.string());

    // And save the isomap:
    outputfile.replace_extension(".isomap.png");

    ofPixels isoMapPix;
    ofxCv::toOf(eos::core::to_mat(isomap), isoMapPix);
    ofSaveImage(isoMapPix, outputfile.string());
    
    cout << "Finished fitting and wrote result mesh and isomap to files with basename "
    << outputfile.stem().stem() << "." << endl;


    ofExit();
}
