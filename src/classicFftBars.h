//
//  classicFftBars.h
//  audioInputOct
//
//  Created by Stefan Goodchild on 02/04/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef audioInputOct_classicFftBars_h
#define audioInputOct_classicFftBars_h

#pragma once

#include "ofMain.h"

class ClassicFftBars
{
    
    public :
    ClassicFftBars();
    ClassicFftBars(ofVec2f pos, int c) {
        position = pos;
        channel = c;
        plotHeight = 100;
    }
    void setup();
    void draw(float* fftOutput, float* eqOutput, int fftBinSize);
    
    ofVec2f position;
	int plotHeight;
    void plot(float* array, int length, float scale, float offset);

	float* fftOut;
	float* eqOut;
    
    int bufferSize;
    int channel;
    char title[255];
    
};

#endif
