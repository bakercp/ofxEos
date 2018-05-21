//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Load the model from all of the default files.
    // Each file can be specified individually.
    model = ofxEos::Model::fromFiles();

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
            model.update(instances[0].getLandmarks().getImagePoints());
        }
    }
}


void ofApp::draw()
{
//    grabber.draw(0, 0);
    
//    ofSetColor(
    grabber.bind();
    ofEnableDepthTest();
    cam.begin();
    ofPushMatrix();
    ofScale(3.5, 3.5, 3.5);
//    ofSetColor(ofColor::yellow);
//    model.mesh().drawFaces();
//    ofSetColor(ofColor::white);
//    model.mesh().drawWireframe();
    model.draw();
    
    
    
    ofPopMatrix();
    cam.end();
    ofDisableDepthTest();
    grabber.unbind();
    
    
//    for (auto& v: model.mesh().getVertices())
//    {
////        ofDrawCircle(v, 20);
//    }
    
}
