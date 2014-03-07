#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
    
    ofSetFrameRate(60);
    
    // Setups
    
    loadSettings();
    setAudioDevice();
    
    // Viz
    
    audioVis.setAudioManagerRef( &audioManager );
    audioVis.setup();

}

//--------------------------------------------------------------
void testApp::update() {
    
    audioManager.analyzeFFTOctaves();
    audioVis.update();
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));

}

//--------------------------------------------------------------
void testApp::draw() {
    
    ofBackground(0);
    audioVis.draw();

}

//--------------------------------------------------------------
void testApp::exit() {
    audioManager.stop();
}

//--------------------------------------------------------------
void testApp::loadSettings() {
    ofxXmlSettings configXml;
    
    if ( configXml.loadFile("appConfig.xml") ) {
        cout << "Loaded appConfig.xml" << endl;
    } else {
        cout << "Could not load appConfig.xml" << endl;
    }
    
    config.audio_in = configXml.getValue( "AUDIO::IN", utils::audio::getDeviceInfo(0).name );
    
}

void testApp::setAudioDevice() {
    audioManager.setupAudioInput( config.audio_in, false );
}