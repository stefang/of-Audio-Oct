#pragma once

#include "ofMain.h"

// ADDONS

#include "ofxFft.h"
#include "ofxXmlSettings.h"

// HELPERS

#include "audioManager.h"
#include "audio.h"

// VISUALISERS

#include "audioVis.h"

class testApp : public ofBaseApp {
    
public:
    
    struct AppConfig {
        string audio_in;
    };
    
	void setup();
	void update();
	void draw();
	void exit();
	
    void loadSettings();
    void setupVisualizers();
    void setAudioDevice();
    
    AudioManager audioManager;
    AudioVis audioVis;
    
    AppConfig config;
    
};
