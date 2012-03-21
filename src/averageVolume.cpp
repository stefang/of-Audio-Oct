//
//  averageVolume.cpp
//  audioInputOct
//
//  Created by Stefan Goodchild on 21/03/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "averageVolume.h"

AverageVolume::AverageVolume() {
}

void AverageVolume::setup() {
}

void AverageVolume::update(float scaledVol) {
    //lets record the volume into an array
    volHistory.push_back( scaledVol );
    
    //if we are bigger the the size we want to record - lets drop the oldest value
    if( volHistory.size() >= 400 ){
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    }

}

void AverageVolume::draw(float scaledVol) {
    // draw the average volume:
    
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(position);
    
    ofSetColor(225);
    ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
    ofRect(0, 0, 400, 100);
    
    ofSetColor(245, 58, 135);
    ofFill();		
    ofCircle(50, 50, scaledVol * 25.0f);

    //lets draw the volume history as a graph
    ofBeginShape();
    for (int i = 0; i < volHistory.size(); i++){
        if( i == 0 ) ofVertex(i+1, 99);
        
        ofVertex(i+1, 99 - volHistory[i] * 70);
        
        if( i == volHistory.size() -1 ) ofVertex(i+1, 99);
    }
    ofEndShape(false);		
    
    ofPopMatrix();
    ofPopStyle();
}