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
    width = ofGetScreenWidth();
    blockSize = (int)((width - 100) / 127)/2;
}

void MidiBlocks::update(int value) {
    notes[value] = true;
}

void MidiBlocks::draw() {
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(50, 20);
    ofSetColor(225);
    ofSetLineWidth(1);
    for (int i = 0; i < 127; i++) {
        
        if (notes[i]==true) {
            ofSetColor(245, 58, 135);
        } else {
            ofSetColor(0, 0, 0);
        }
        ofFill();		
        
        ofRect(i*blockSize, 0, blockSize, blockSize);
        
    }
    ofPopMatrix();
    ofPopStyle();
}
