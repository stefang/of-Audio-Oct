//
//  spectrum.cpp
//  audioInputOct
//
//  Created by Stefan Goodchild on 21/03/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "spectrum.h"

void Spectrum::setup() {
}

void Spectrum::update() {
}

void Spectrum::draw( vector <float> chn ) {
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(position);
    
    ofSetColor(225);
    sprintf(title,"Channel: %i", channel);
    
    ofDrawBitmapString(title, 4, 18);
    
    ofSetLineWidth(1);	
    ofRect(0, 0, 512, 100);
    
    ofSetColor(245, 58, 135);
    ofSetLineWidth(3);
    
    ofBeginShape();
    for (int i = 0; i < chn.size(); i++){
        ofVertex((i*4)+2, 50 -chn[i]*90.0f);
    }
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();
}