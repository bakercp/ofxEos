//
// Copyright (c) 2018 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofAppNoWindow.h"
#include "ofApp.h"


int main()
{
    ofAppNoWindow window;
    ofSetupOpenGL(&window, 0, 0, OF_WINDOW);
    return ofRunApp(std::make_shared<ofApp>());
}
