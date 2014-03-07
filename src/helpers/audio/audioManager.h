//
//  soundManager.h
//  Rackabeat2013
//
//  Created by James Acres on 12-08-20.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxFft.h"
#include "FFTOctaveAnalyzer.h"
#include "audio.h"

class AudioManager: public ofBaseSoundInput {

public:
    static const int    MAX_CHANNELS = 8;
    static const int    BUFFER_SIZE = 256;
    
    struct AudioData {
        int numChannels;
        int binSize;
        int bufferSize;
        
        vector <float*> audioInput;
        vector <float> smoothedVol;
        vector <float*> fftOutput;
        vector <float> eqFunction;
        vector <float*> eqOutput;
        vector <float*> ifftOutput;
        
        vector <float> chn[MAX_CHANNELS];

    };
    
    AudioManager() : audioMult(1.0f), chnStart(2), chnCount(5)
    {
        for (int i = 0; i < MAX_CHANNELS; i++) {
            chnTweak[i] = 1.0f;
        }
    }
    
    ~AudioManager();
    
    void setupAudioInput( int deviceId=0 );
    void setupAudioInput( string deviceName, bool debugList );
    void stop();

    const AudioData* getDataPtr();
    const AudioData getData();

    const FFTOctaveAnalyzer* getFFTOctaveAnalyzer( int channelId=0 );
    const vector<FFTOctaveAnalyzer>* getFFTOctaveAnalyzers();

    void analyzeFFTOctaves();

    void    setAudioMultiplier( float mult ) { audioMult = mult; };
    void    setChannelTweak( int index, float val );

    float   getAudioMultiplier() { return audioMult; };
    int     getNumChannels() { return numChannels; };
    float   getChannelTweak( int index ) { return chnTweak[index]; };
     
    // audio input callback
    void audioReceived( float* input, int bufferSize, int nChannels ); 
    
private:
    AudioData   audioData;
    
    vector< ofPtr<ofxFft> > fft;
    vector<FFTOctaveAnalyzer> FFTanalyzer;
    
    ofSoundStream soundStream;

    int numChannels;
        
    // For looping through usable channels
    int chnStart;
    int chnCount;

    float audioMult;
    float chnTweak[MAX_CHANNELS];
    
    ofMutex mutex;
};

