#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxMidi.h"

// Visualizer Classes

#include "averageVolume.h"
#include "spectrum.h"

// public ofxMidiListener
class testApp : public ofBaseApp, public ofxMidiListener {
	
	public:
		
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
            
    int channelCount;
    void audioIn(float * input, int bufferSize, int nChannels); 

    vector <float> chn[6];
            
    float smoothedVol[6];
    float scaledVol[6];
    
    void newMidiMessage(ofxMidiEventArgs& eventArgs);

    int port;
    int id;
    int value;
    double timestamp;
    char msg[255];

    ofxMidiIn midiIn;
    ofSoundStream soundStream;

    // Visualizers

    vector<AverageVolume> averageVolumes; 
    vector<Spectrum> spectrums;
    
};

#endif	

