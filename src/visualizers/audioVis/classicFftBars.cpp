//
//  classicFftBars.cpp
//  audioInputOct
//
//  Created by Stefan Goodchild on 02/04/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "classicFftBars.h"

ClassicFftBars::ClassicFftBars() {
}

void ClassicFftBars::setup() {
    colour.setHsb(0, 255, 255);
}

void ClassicFftBars::draw(float* fftOutput, float* eqOutput, int fftBinSize) {
    
    ofPushStyle();
	ofPushMatrix();
    
    ofTranslate(position);
    sprintf(title,"FFT EQ: %i", channel);
    ofDrawBitmapString(title, 4, 18);
    plot(eqOutput, fftBinSize, -plotHeight, plotHeight/2);

    ofPopMatrix();
    ofPopStyle();
}

void ClassicFftBars::plot(float* array, int length, float scale, float offset) {
    ofNoFill();
    ofRect(0, 0, length*2 + 5, plotHeight);
	ofPushMatrix();
	ofTranslate(4, (plotHeight / 2 + offset) -3, 0);
    ofPushStyle();
    ofSetColor(colour);
    ofFill();
    for (int i = 0; i < length; i++)
        ofRect(i*2, 0, 1, (array[i] * scale) - 1);
    ofPopStyle();
    ofPopMatrix();
}