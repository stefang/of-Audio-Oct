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
        volHistory.assign(400, 0.0);
    }
    
    void setup();
    void update(float scaledVol);
    void draw(float scaledVol);
    
    vector <float> volHistory;
    
    ofVec2f position;
    ofColor colour;
    
};


#endif
