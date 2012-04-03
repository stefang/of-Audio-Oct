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
}

void ClassicFftBars::draw(float* fftOutput, float* eqOutput, int fftBinSize) {
    
    ofPushStyle();
	ofPushMatrix();
    
    ofTranslate(position);
    sprintf(title,"FFT: %i", channel);
    ofDrawBitmapString(title, 4, 18);
    plot(fftOutput, fftBinSize, -plotHeight, plotHeight/2);
    ofPushMatrix();

    ofTranslate(fftBinSize*4+ 20, 0, 0);
    sprintf(title,"FFT EQ: %i", channel);
    ofDrawBitmapString(title, 4, 18);
    plot(eqOutput, fftBinSize, -plotHeight, plotHeight/2);
    ofPopMatrix();

	ofPopMatrix();
    ofPopStyle();
}

void ClassicFftBars::plot(float* array, int length, float scale, float offset) {
    ofRect(0, 0, length*4 + 5, plotHeight);
	ofPushMatrix();
	ofTranslate(4, (plotHeight / 2 + offset) -3, 0);
    ofPushStyle();
    ofSetColor(245, 58, 135);
    ofFill();
    for (int i = 0; i < length; i++)
        ofRect(i*4, 0, 3, (array[i] * scale) - 1);
    ofPopStyle();
    ofPopMatrix();
}