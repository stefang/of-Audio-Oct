//
//  breakFree.cpp
//  queen2012
//
//  Created by Stefan Goodchild on 20/06/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "audioVis.h"

AudioVis::AudioVis() {
}

void AudioVis::setup() {
    for (int c = 0; c < audioManager->getNumChannels(); c++) {
        spectrums.push_back( Spectrum(ofVec2f(20,(c*100+20)), c));
        classicBars.push_back( ClassicFftBars(ofVec2f(296,(c*100+20)), c));
        octaveEqs.push_back( Octaves(ofVec2f(578,(c*100+20)), c));
        averageVolumes.push_back( AverageVolume(ofVec2f(803,(c*100+20))));
    }
}

void AudioVis::update() {
}

void AudioVis::draw() {
    ofBackground( 0, 0, 0 );
    ofSetColor(255, 255, 255, 255);

    const vector<FFTOctaveAnalyzer>* fftAnalyzer = audioManager->getFFTOctaveAnalyzers();
    const AudioManager::AudioData audioData = audioManager->getData();
    
    for (int c = 0; c < audioManager->getNumChannels(); c++) {
        spectrums[c].draw( audioData.audioInput[c], audioData.bufferSize );
        classicBars[c].draw(audioData.fftOutput[c], audioData.eqOutput[c], audioData.binSize);
        averageVolumes[c].draw( audioData.smoothedVol[c] );
        octaveEqs[c].draw((*fftAnalyzer)[c].nAverages, (*fftAnalyzer)[c].averages, (*fftAnalyzer)[c].peaks);
    }

}