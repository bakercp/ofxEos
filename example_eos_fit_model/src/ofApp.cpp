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

    
    std::filesystem::path imageFile = "image_0010.png";
    
    auto basename = std::filesystem::path(imageFile).stem();
    
    // Input image.
    ofPixels pixels;
    ofLoadImage(pixels, imageFile);
    cv::Mat image = ofxCv::toCv(pixels);
    
    // 2D landmarks for the image, in ibug .pts format.
    auto landmarks = ofxEos::loadLandmarks(imageFile.replace_extension(".pts").string());

    // a Morphable Model stored as cereal BinaryArchive.
    auto morphable_model = ofxEos::loadModel("sfm_shape_3448.bin");

    // The landmark mapper is used to map 2D landmark points (e.g. from the
    // ibug scheme) to vertex ids.
    auto landmark_mapper = ofxEos::loadLandmarkMapper("ibug_to_sfm.txt");
    
    // The expression blendshapes.
    const auto blendshapes = ofxEos::loadBlendshapes("expression_blendshapes_3448.bin");
    
    // These two are used to fit the front-facing contour to the ibug contour landmarks:
    
    // file with model contour indices
    const auto model_contour = ofxEos::loadModelContour("sfm_model_contours.json");
    const auto ibug_contour = ofxEos::loadContourLandmarks("ibug_to_sfm.txt");

    // The edge topology is used to speed up computation of the occluding face contour fitting:
    // file with model's precomputed edge topology
    const auto edge_topology = ofxEos::loadEdgeTopology("sfm_3448_edge_topology.json");

    
    // Draw the loaded landmarks:
    cv::Mat outimg = image.clone();
    for (auto&& lm : landmarks)
    {
        cv::rectangle(outimg,
                      cv::Point2f(lm.coordinates[0] - 2.0f, lm.coordinates[1] - 2.0f),
                      cv::Point2f(lm.coordinates[0] + 2.0f, lm.coordinates[1] + 2.0f), {255, 0, 0});
    }

    
    // Fit the model, get back a mesh and the pose:
    eos::core::Mesh mesh;
    eos::fitting::RenderingParameters rendering_params;
    std::tie(mesh, rendering_params) = eos::fitting::fit_shape_and_pose(morphable_model,
                                                                        blendshapes,
                                                                        landmarks,
                                                                        landmark_mapper,
                                                                        image.cols,
                                                                        image.rows,
                                                                        edge_topology,
                                                                        ibug_contour,
                                                                        model_contour,
                                                                        5,
                                                                        eos::cpp17::nullopt,
                                                                        30.0f);

    // The 3D head pose can be recovered as follows:
    float yaw_angle = glm::degrees(glm::yaw(rendering_params.get_rotation()));
    float pitch_angle = glm::degrees(glm::pitch(rendering_params.get_rotation()));
    float roll_angle = glm::degrees(glm::roll(rendering_params.get_rotation()));

    // Extract the texture from the image using given mesh and camera parameters:
    const Eigen::Matrix<float, 3, 4> affine_from_ortho = eos::fitting::get_3x4_affine_camera_matrix(rendering_params,
                                                                                                    image.cols,
                                                                                                    image.rows);
    const auto isomap = eos::render::extract_texture(mesh,
                                                     affine_from_ortho,
                                                     eos::core::from_mat(image),
                                                     true);
    
    // Draw the fitted mesh as wireframe, and save the image:
    eos::render::draw_wireframe(outimg,
                                mesh,
                                rendering_params.get_modelview(),
                                rendering_params.get_projection(),
                                eos::fitting::get_opencv_viewport(image.cols,
                                                                  image.rows));
    
    std::filesystem::path outputfile = ofToDataPath(imageFile.replace_extension(".out.png"), true);
    
    ofPixels outPix;
    ofxCv::toOf(outimg, outPix);
    ofSaveImage(outPix, outputfile.string());
    
    // Save the mesh as textured obj:
    outputfile.replace_extension(".obj");
    eos::core::write_textured_obj(mesh, outputfile.string());

    auto objectFile = outputfile;
    
    // And save the isomap:
    outputfile.replace_extension(".isomap.png");

    ofxCv::toOf(eos::core::to_mat(isomap), outPix);
    ofSaveImage(outPix, outputfile.string());

    // Re-load the model.
    generatedModel.loadModel(objectFile.string());
    generatedModel.setScale(1, -1, 1);
    

}


void ofApp::draw()
{
    ofEnableDepthTest();
    cam.begin();
    generatedModel.drawFaces();
    cam.end();
    ofDisableDepthTest();
}
