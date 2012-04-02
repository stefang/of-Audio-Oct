//
//  midiBlocks.cpp
//  audioInputOct
//
//  Created by Stefan Goodchild on 21/03/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "midiBlocks.h"

MidiBlocks::MidiBlocks() {
}

void MidiBlocks::setup() {
    width = ofGetViewportWidth();
    blockSize = (int)((width - 12) / 127);
}

void MidiBlocks::update(int value) {
    notes[value] = true;
}

void MidiBlocks::draw() {
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(20, 20);
    ofSetColor(225);
    for (int i = 0; i < 127; i++) {
        if (notes[i]==true) {
            ofSetColor(245, 58, 135);
        } else {
            ofSetColor(10, 10, 10);
        }
        ofFill();		
        
        ofRect(i*blockSize, 0, blockSize-1, blockSize-1);
        
    }
    ofPopMatrix();
    ofPopStyle();
}
