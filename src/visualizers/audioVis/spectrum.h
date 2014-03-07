//
//  spectrum.h
//  audioInputOct
//
//  Created by Stefan Goodchild on 21/03/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef audioInputOct_spectrum_h
#define audioInputOct_spectrum_h

#pragma once

#include "ofMain.h"

class Spectrum
{
    
    public :
    Spectrum();
    Spectrum(ofVec2f pos, int c) {
        
        cout << pos << endl;
        position = pos;
        channel = c;
    }

    void setup();
    void update();
    void draw(float* audioInput, int size);
    
    char title[255];
    int channel;
    
    ofVec2f position;
    ofColor colour;

    
};

#endif
