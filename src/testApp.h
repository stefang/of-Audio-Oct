#ifndef _TEST_APP
#define _TEST_APP

#define BUFFER_SIZE 128
#define MAX_CHANNEL_COUNT 6

#include "ofMain.h"
#include "ofxUI.h"

#include "RtAudio.h"

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
    
    void exit();
    void guiEvent(ofxUIEventArgs &e);
    
    ofxUICanvas *gui;
            
    ofPtr<RtAudio> audioTemp; 

    int channelCount;
    void audioIn(float * input, int bufferSize, int nChannels); 
    void audioInputSetup();
    
    vector <float> chn[MAX_CHANNEL_COUNT];

    // vector< vector<float> > chn;
    // vector< vector<float> > chn;
    // vector <float*> chn;
    
    vector <ofxFft*> fft;
    FFTOctaveAnalyzer FFTanalyzer[MAX_CHANNEL_COUNT];
    // vector <FFTOctaveAnalyzer*> FFTanalyzer;
    
    vector <float*> audioInput;
	vector <float*> fftOutput;
	vector <float*> eqFunction;
	vector <float*> eqOutput;
	vector <float*> ifftOutput;
            
    vector <float> smoothedVol;
    vector <float> scaledVol;
    
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

