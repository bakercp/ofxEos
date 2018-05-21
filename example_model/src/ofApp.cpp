//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    loadModel(ofToDataPath("sfm_shape_3448.bin", true),
              ofToDataPath("expression_blendshapes_3448.bin", true));
    
}


void ofApp::update()
{
    if (modelNeedsUpdate)
    {
        for (std::size_t i = 0; i < shapeCoefficientsParams.size(); ++i)
            shapeCoefficients[i] = shapeCoefficientsParams[i];

        for (std::size_t i = 0; i < blendShapeCoefficientsParams.size(); ++i)
            blendShapeCoefficients[i] = blendShapeCoefficientsParams[i];

        for (std::size_t i = 0; i < colorCoefficientsParams.size(); ++i)
            colorCoefficients[i] = colorCoefficientsParams[i];

        _mesh.clear();
        _mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        
        Eigen::VectorXf shape;
        
        if (blendShapeCoefficients.size() > 0 && blendshapes.size() > 0)
        {
            shape = morphableModel.get_shape_model().draw_sample(shapeCoefficients) + eos::morphablemodel::to_matrix(blendshapes) * Eigen::Map<const Eigen::VectorXf>(blendShapeCoefficients.data(), blendShapeCoefficients.size());
        }
        else
        {
            shape = morphableModel.get_shape_model().draw_sample(shapeCoefficients);
        }

        const Eigen::VectorXf color = morphableModel.get_color_model().get_mean();
        
        eos::core::Mesh mesh = eos::morphablemodel::sample_to_mesh(shape, color,
                                                                   morphableModel.get_shape_model().get_triangle_list(),
                                                                   morphableModel.get_color_model().get_triangle_list());
        
        for (std::size_t i = 0; i < mesh.vertices.size(); ++i)
        {
            _mesh.addVertex({ mesh.vertices[i][0],
                              mesh.vertices[i][1],
                              mesh.vertices[i][2] });
        }

        for (std::size_t i = 0; i < mesh.tvi.size(); ++i)
        {
            _mesh.addIndex(mesh.tvi[i][0]);
            _mesh.addIndex(mesh.tvi[i][1]);
            _mesh.addIndex(mesh.tvi[i][2]);
        }
        
        modelNeedsUpdate = false;
    }

}


void ofApp::draw()
{
    ofEnableDepthTest();
    cam.begin();
    ofPushMatrix();
    ofScale(3.5, 3.5, 3.5);
    ofSetColor(ofColor::yellow);
    _mesh.drawFaces();
    ofSetColor(ofColor::black);
    _mesh.drawWireframe();
    ofPopMatrix();
    cam.end();
    ofDisableDepthTest();
    
    guiShape.draw();
    guiBlend.draw();
    guiColor.draw();
    
}


void ofApp::setupGui()
{
    shapeCoefficentsGroup.clear();
    shapeCoefficientsParams.clear();
    shapeCoefficentsGroup.setName("Shape PCA Components");

    for (std::size_t i = 0; i < shapeCoefficients.size(); ++i)
    {
        shapeCoefficientsParams.push_back(ofParameter<float>());
        shapeCoefficentsGroup.add(shapeCoefficientsParams.back().set("Comp: " + ofToString(i), 0, -10, 10));
        shapeCoefficientsParams.back().addListener(this, &ofApp::modelUpdated);
    }

    blendShapeCoefficentsGroup.clear();
    blendShapeCoefficientsParams.clear();
    blendShapeCoefficentsGroup.setName("Blend Shape PCA Components");
    for (std::size_t i = 0; i < blendShapeCoefficients.size(); ++i)
    {
        blendShapeCoefficientsParams.push_back(ofParameter<float>());
        blendShapeCoefficentsGroup.add(blendShapeCoefficientsParams.back().set(blendshapes[i].name, 0, -10, 10));
        blendShapeCoefficientsParams.back().addListener(this, &ofApp::modelUpdated);
    }

    colorCoefficentsGroup.clear();
    colorCoefficientsParams.clear();
    colorCoefficentsGroup.setName("Color PCA Components");
    for (std::size_t i = 0; i < colorCoefficients.size(); ++i)
    {
        colorCoefficientsParams.push_back(ofParameter<float>());
        colorCoefficentsGroup.add(colorCoefficientsParams.back().set("Comp: " + ofToString(i), 0, -3.5, 3.5));
        colorCoefficientsParams.back().addListener(this, &ofApp::modelUpdated);
    }
        
    guiShape.setup(shapeCoefficentsGroup);

    guiBlend.setup(blendShapeCoefficentsGroup);
    guiBlend.setPosition(guiShape.getWidth() + 14, guiShape.getPosition().y);
    
    guiColor.setup(colorCoefficentsGroup);
    guiColor.setPosition(guiBlend.getPosition().x + guiBlend.getWidth() + 14, guiShape.getPosition().y);

}


void ofApp::loadModel(const std::string& modelFile,
                      const std::string& blendshapesFile)
{
    std::filesystem::path _modelFile = modelFile;
    std::filesystem::path _blendshapesFile = blendshapesFile;

    if (_modelFile.empty())
    {
        ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a model file.");

        if (!openFileResult.bSuccess)
        {
            ofLogError("ofApp::loadModel") << "No model file selected.";
            return;
        }
    }
    
    if (_blendshapesFile.empty())
    {
        ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a blendshape file.");
        
        if (!openFileResult.bSuccess)
        {
            ofLogError("ofApp::loadModel") << "No blendshape file selected, skipping.";
        }
    }

    try
    {
        if (_modelFile.extension() == ".scm")
        {
            morphableModel = eos::morphablemodel::load_scm_model(_modelFile.string());
        }
        else
        {
            morphableModel = eos::morphablemodel::load_model(_modelFile.string());
        }
    }
    catch (const std::exception& exc)
    {
        ofLogError("ofApp::loadModel") << "Invalid model file: " << exc.what();
        return;
    }
    
    if (!_blendshapesFile.empty())
    {
        try
        {
            blendshapes = eos::morphablemodel::load_blendshapes(_blendshapesFile.string());
        }
        catch (const std::exception& exc)
        {
            ofLogError("ofApp::loadModel") << "Invalid blendshape file: " << exc.what();
            return;
        }
    }

    shapeCoefficients = std::vector<float>(morphableModel.get_shape_model().get_num_principal_components());
    
    colorCoefficients.clear();
    if (morphableModel.has_color_model())
    {
        colorCoefficients = std::vector<float>(morphableModel.get_color_model().get_num_principal_components());
    }
    
    blendShapeCoefficients.clear();
    if (blendshapes.size() > 0)
    {
        blendShapeCoefficients = std::vector<float>(blendshapes.size());
    }

    modelNeedsUpdate = true;
    setupGui();
}


