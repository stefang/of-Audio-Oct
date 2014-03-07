//
//  octaves.h
//  audioInputOct
//
//  Created by Stefan Goodchild on 03/04/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef audioInputOct_octaves_h
#define audioInputOct_octaves_h

#pragma once

#include "ofMain.h"

class Octaves
{
    
    public :
    Octaves();
    Octaves(ofVec2f pos, int c) {
        position = pos;
        channel = c;
    }
    void setup();
    void draw(int nAverages, float* averages, float* peaks);
    
    ofVec2f position;
    int channel;
    char title[255];
    
    ofColor colour;
    
};


#endif
