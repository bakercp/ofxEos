//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxGui.h"
#include "ofxEos.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;

    void setupGui();
    
    void loadModel(const std::string& modelFile,
                   const std::string& blendshapesFile);
    
    void modelUpdated(float& f) { modelNeedsUpdate = true; }

    eos::morphablemodel::MorphableModel morphableModel;
    eos::morphablemodel::Blendshapes blendshapes;

    ofParameterGroup shapeCoefficentsGroup;
    std::vector<ofParameter<float>> shapeCoefficientsParams;
    std::vector<float> shapeCoefficients;

    ofParameterGroup blendShapeCoefficentsGroup;
    std::vector<ofParameter<float>> blendShapeCoefficientsParams;
    std::vector<float> blendShapeCoefficients;

    ofParameterGroup colorCoefficentsGroup;
    std::vector<ofParameter<float>> colorCoefficientsParams;
    std::vector<float> colorCoefficients;

    ofParameter<void> randomBlendShapeCoefficients;
    ofParameter<void> resetBlendShapeCoefficients;

    ofParameter<void> randomColorCoefficients;
    ofParameter<void> resetColorCoefficients;;
    
    bool modelNeedsUpdate = true;
    
    ofMesh _mesh;
    ofEasyCam cam;

    ofxPanel guiShape;
    ofxPanel guiBlend;
    ofxPanel guiColor;
    
    float offset = -5;

};
