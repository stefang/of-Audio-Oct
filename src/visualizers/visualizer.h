//
//  templateViz.h
//  Rackabeat2013
//
//  Created by Stefan Goodchild on 21/06/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef queen2012_templateViz_h
#define queen2012_templateViz_h

#pragma once

#include "ofMain.h"

#include "audioManager.h"

class Visualizer  
{
    
public:
    Visualizer() :  
        audioManager(NULL)
    {};
    
    virtual ~Visualizer() { cout << "~Visualizer destroyed" << endl; };
    
    virtual void setup();
    virtual void enable() {};
    virtual void disable() {};
    virtual void update();
    virtual void draw();
    
    void setAudioManagerRef( AudioManager* audioMgr );
    
protected:
    
    AudioManager*   audioManager;
    
};

#endif
