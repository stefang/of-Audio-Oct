//
//  AudioManager.cpp
//  queen2012
//
//  Created by James Acres on 12-08-20.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "AudioManager.h"

AudioManager::~AudioManager() {
}

void AudioManager::setupAudioInput( string deviceName, bool debugList ) {
    
    cout << "Setting audio device to :" << deviceName << endl;
    
    if (debugList) soundStream.listDevices();
    
    unsigned int deviceId = utils::audio::getDeviceId( deviceName );
    cout << "setup audio input: " << deviceName << ", device id: " << deviceId << endl;
    setupAudioInput( deviceId );
}

void AudioManager::setupAudioInput( int deviceId ) {
    
    if ( deviceId != -1 ) {
        
        mutex.lock();
        
        soundStream.stop();
        soundStream.close();
        
        soundStream.setDeviceID( deviceId );
        RtAudio::DeviceInfo deviceInfo = utils::audio::getDeviceInfo( deviceId );
        
        numChannels = deviceInfo.inputChannels;
        
        if ( numChannels > MAX_CHANNELS) {
            numChannels = MAX_CHANNELS;
        }
        
        // x output channels, 
        // x input channels
        // x samples per second
        // x samples per buffer
        // x num buffers (latency)        
        soundStream.setup( 0, numChannels, 44100, BUFFER_SIZE, 2 );
        soundStream.setInput( this );
        
        audioData.bufferSize = BUFFER_SIZE;
        
        // free all memory that was allocated via "new"
        vector<float*>::iterator it;
        for ( it=audioData.audioInput.begin(); it<audioData.audioInput.end(); ++it ) delete [] *it;
        for ( it=audioData.fftOutput.begin(); it<audioData.fftOutput.end(); ++it ) delete [] *it;
        for ( it=audioData.eqOutput.begin(); it<audioData.eqOutput.end(); ++it ) delete [] *it;
        for ( it=audioData.ifftOutput.begin(); it<audioData.ifftOutput.end(); ++it ) delete [] *it;
        
        audioData.audioInput.clear();
        audioData.fftOutput.clear();
        audioData.eqFunction.clear();
        audioData.eqOutput.clear();
        audioData.ifftOutput.clear();
        
        // clear values
        audioData.smoothedVol.clear();
        
        fft.clear();
        
        FFTanalyzer.clear();
        
        int binSize;
        
        // Build them again
        for (int c = 0; c < numChannels; c++) {
            
            audioData.chn[c].assign( BUFFER_SIZE, 0.0f );
            
            audioData.smoothedVol.push_back(0.0f);

            fft.push_back( ofPtr<ofxFft>( ofxFft::create( BUFFER_SIZE, OF_FFT_WINDOW_BARTLETT ) ) );            
            binSize = fft[c]->getBinSize();
            
            // Need to clean up new float[] arrays with delete [] when done (memory leak)
            audioData.audioInput.push_back( new float[BUFFER_SIZE] );
            audioData.fftOutput.push_back( new float[binSize] );
            audioData.eqOutput.push_back( new float[binSize] );
            audioData.ifftOutput.push_back( new float[BUFFER_SIZE] );
            
            for(int i = 0; i < fft[c]->getBinSize(); i++) {
                audioData.eqFunction.push_back((float)(fft[c]->getBinSize() - i) / (float) (fft[c]->getBinSize()));
            }
            
            // Octave Analyzer
            FFTOctaveAnalyzer fftOctAnalyzer;

            fftOctAnalyzer.setup(44100, int(BUFFER_SIZE/numChannels), 1);
            fftOctAnalyzer.peakHoldTime = 0; // hold longer
            fftOctAnalyzer.peakDecayRate = 0.95f; // decay slower
            fftOctAnalyzer.linearEQIntercept = 0.3f; // reduced gain at lowest frequency
            fftOctAnalyzer.linearEQSlope = 0.4f; // increasing gain at higher frequencies

            FFTanalyzer.push_back( fftOctAnalyzer );
        }
        
        audioData.binSize = binSize;

        mutex.unlock();
    }      
}

void AudioManager::stop() {
    soundStream.stop();
    cout << "AudioManager::stop()" << endl;
}

//--------------------------------------------------------------
void AudioManager::audioReceived( float *input, int bufferSize, int nChannels ) {	
	
    mutex.lock();
    
    for (int c = 0; c < nChannels; c++) {
        float curVol = 0.0f;
        
        // samples are "interleaved"
        int numCounted = 0;	
        
        for (int i = 0; i < bufferSize; i++){
            audioData.audioInput[c][i] = input[i*nChannels+c] *.35 * chnTweak[c] * audioMult;
            audioData.chn[c][i] = input[i*nChannels+c]*0.26f;
            curVol += audioData.audioInput[c][i] * audioData.audioInput[c][i];
        }
        
		numCounted+=nChannels;
        //this is how we get the mean of rms :) 
        curVol /= (float)numCounted;
        
        // this is how we get the root of rms :) 
        // curVol = sqrt( curVol );
        
        audioData.smoothedVol[c] *= 0.93f;
        audioData.smoothedVol[c] += 0.07f * curVol * chnTweak[c] * audioMult;
        
        // FFT
        fft[c]->setSignal( audioData.audioInput[c] );
        memcpy( audioData.fftOutput[c], fft[c]->getAmplitude(), sizeof(float) * fft[c]->getBinSize() );
        
        for(int i = 0; i < fft[c]->getBinSize(); i++) {
            audioData.eqOutput[c][i] = audioData.fftOutput[c][i] * audioData.eqFunction[i] * 2;
        }
        
        fft[c]->setPolar( audioData.eqOutput[c], fft[c]->getPhase() );
        fft[c]->clampSignal();
    }	
    
    mutex.unlock();
}

const AudioManager::AudioData* AudioManager::getDataPtr() {
    return &audioData;
}

const AudioManager::AudioData AudioManager::getData() {
    return audioData;
}

const FFTOctaveAnalyzer* AudioManager::getFFTOctaveAnalyzer( int channelId ) {
    return &FFTanalyzer[channelId];
}

const vector<FFTOctaveAnalyzer>* AudioManager::getFFTOctaveAnalyzers() {
    return &FFTanalyzer;
}

void AudioManager::setChannelTweak( int index, float val ) {
    if ( index >=0 && index < MAX_CHANNELS ) {
        chnTweak[index] = val;
    }
}

void AudioManager::analyzeFFTOctaves() {
    for (int c = 0; c < numChannels; c++) {
        FFTanalyzer[c].calculate( audioData.eqOutput[c] );
    }

}