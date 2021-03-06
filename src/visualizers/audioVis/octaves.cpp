//
//  octaves.cpp
//  audioInputOct
//
//  Created by Stefan Goodchild on 03/04/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "octaves.h"

Octaves::Octaves() {
}

void Octaves::setup() {
    colour.setHsb(0, 255, 255);
}

void Octaves::draw(int nAverages, float* averages, float* peaks) {
    
    ofPushStyle();
	ofPushMatrix();
    ofTranslate(position);
    sprintf(title,"Octaves: %i", channel);
    ofDrawBitmapString(title, 4, 18);
    ofNoFill();
    ofRect(0, 0, nAverages*22+7, 100);
    
    ofPushMatrix();
    ofTranslate(5,0);
    
    ofFill();

    ofSetColor(colour);
    for (int i = 0; i < nAverages; i++){
		ofRect(i*22,95,20,-averages[i] * 220 -1);
	}

    ofSetColor(255, 0, 0);
	for (int i = 0; i < nAverages; i++){
		ofRect(i*22,(-peaks[i] * 220)+95,20,-2);
	}

    ofPopMatrix();
	ofPopMatrix();
    ofPopStyle();
}
