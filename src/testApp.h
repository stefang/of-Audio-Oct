#ifndef _TEST_APP
#define _TEST_APP

#define BUFFER_SIZE 128
#define CHANNEL_COUNT 2

#include "ofMain.h"

#include "ofxMidi.h"
#include "ofxFft.h"
#include "FFTOctaveAnalyzer.h"

// Visualizer Classes

#include "averageVolume.h"
#include "spectrum.h"
#include "midiBlocks.h"
#include "classicFftBars.h"
#include "octaves.h"

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
    void audioInputSetup();
    
    vector <float> chn[CHANNEL_COUNT];
    
    ofxFft* fft[CHANNEL_COUNT];
    FFTOctaveAnalyzer FFTanalyzer[CHANNEL_COUNT];
    
    float* audioInput[CHANNEL_COUNT];
	float* fftOutput[CHANNEL_COUNT];
	float* eqFunction[CHANNEL_COUNT];
	float* eqOutput[CHANNEL_COUNT];
	float* ifftOutput[CHANNEL_COUNT];
            
    float smoothedVol[CHANNEL_COUNT];
    float scaledVol[CHANNEL_COUNT];
    
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
    vector<ClassicFftBars> classicBars;    
    vector<Octaves> octaveEqs;    
    
    MidiBlocks midiVis;
    
};

#endif	

