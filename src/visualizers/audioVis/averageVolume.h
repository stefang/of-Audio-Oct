//
//  averageVolume.h
//  audioInputOct
//
//  Created by Stefan Goodchild on 21/03/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef audioInputOct_averageVolume_h
#define audioInputOct_averageVolume_h

#pragma once

#include "ofMain.h"

class AverageVolume
{

    public :
    AverageVolume();
    AverageVolume(ofVec2f pos) {
        position = pos;
    }
    
    void setup();
    void update();
    void draw(float scaledVol);
    
    ofVec2f position;
    ofColor colour;
    
};


#endif
