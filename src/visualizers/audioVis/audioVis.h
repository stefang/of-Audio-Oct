//
//  audioVis.h
//  visualizer framework
//
//  Created by Stefan Goodchild on 22/08/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "visualizer.h"

#include "audioManager.h"

#include "averageVolume.h"
#include "classicFftBars.h"
#include "octaves.h"
#include "spectrum.h"

class AudioVis : public Visualizer
{
    public :
    AudioVis();
    
    void setup();
    void update();
    void draw();
    
    vector<AverageVolume> averageVolumes;
    vector<Spectrum> spectrums;    
    vector<ClassicFftBars> classicBars;    
    vector<Octaves> octaveEqs;    

};
