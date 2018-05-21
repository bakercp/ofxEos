//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxEos.h"
#include "ofxFaceTracker2.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;

    ofxFaceTracker2 tracker;
    ofVideoGrabber grabber;

    eos::morphablemodel::MorphableModel morphable_model;
    eos::core::LandmarkMapper landmark_mapper;
    
    ofTexture isoMapTex;
    
    ofMesh _mesh;
};
