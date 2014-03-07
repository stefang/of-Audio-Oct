//
//  averageVolume.cpp
//  audioInputOct
//
//  Created by Stefan Goodchild on 21/03/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "averageVolume.h"

AverageVolume::AverageVolume() {
}

void AverageVolume::setup() {
    colour.setHsb(0, 255, 255);
}

void AverageVolume::update() {
}

void AverageVolume::draw(float scaledVol) {
    // draw the average volume:
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(position);
    
    ofSetColor(225);
    ofNoFill();
    ofDrawBitmapString("smoothedVol", 4, 18);
    ofDrawBitmapString(ofToString(scaledVol), 4, 96);
    ofRect(0, 0, 100, 100);
    
    
    ofSetColor(colour);
    ofFill();		
    ofCircle(50, 50, scaledVol * 15.0f);

    ofPopMatrix();
    ofPopStyle();
}