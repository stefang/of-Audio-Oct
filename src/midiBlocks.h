//
//  midiBlocks.h
//  audioInputOct
//
//  Created by Stefan Goodchild on 21/03/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef audioInputOct_midiBlocks_h
#define audioInputOct_midiBlocks_h

#pragma once

#include "ofMain.h"

class MidiBlocks
{
    
    public :
    MidiBlocks();
    void setup();
    void update(int value);
    void draw();

    Boolean notes[127];
    int width;
    int blockSize;
    
    ofColor colour;

    
};

#endif
