#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 
	
	ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);	
    
    midiIn.listPorts();
	midiIn.openPort(1);
	midiIn.addListener(this);
	
	soundStream.listDevices();
	soundStream.setDeviceID(1);
    // x output channels, 
	// x input channels
	// x samples per second
	// x samples per buffer
	// x num buffers (latency)
    soundStream.setup(this, 0, CHANNEL_COUNT, 44100, BUFFER_SIZE, 2);
    	
    for (int c = 0; c < CHANNEL_COUNT; c++) {
        chn[c].assign(BUFFER_SIZE, 0.0);
        smoothedVol[c] = 0.0;
        scaledVol[c] = 0.0;
                
        fft[c] = ofxFft::create(BUFFER_SIZE, OF_FFT_WINDOW_BARTLETT);
        // To use with FFTW, try:
        // fft = ofxFft::create(BUFFER_SIZE, OF_FFT_WINDOW_BARTLETT, OF_FFT_FFTW);
        
        audioInput[c] = new float[BUFFER_SIZE];
        fftOutput[c] = new float[fft[c]->getBinSize()];
        eqFunction[c] = new float[fft[c]->getBinSize()];
        eqOutput[c] = new float[fft[c]->getBinSize()];
        ifftOutput[c] = new float[BUFFER_SIZE];
        
        for(int i = 0; i < fft[c]->getBinSize(); i++)
            eqFunction[c][i] = (float) (fft[c]->getBinSize() - i) / (float) (fft[c]->getBinSize());
        
        // Create Visualisers
        spectrums.push_back( Spectrum(ofVec2f(20,(c*100+50)), c));
        classicBars.push_back( ClassicFftBars(ofVec2f(296,(c*100+50)), c));
        averageVolumes.push_back( AverageVolume(ofVec2f(856,(c*100+50))));
    }

    midiVis.setup();
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    for (int c = 0; c < CHANNEL_COUNT; c++) {
        scaledVol[c] = ofMap(smoothedVol[c], 0.0, 0.17, 0.0, 1.0, true); // scale the vol up to a 0-1 range
        // Update Visualisers with audio data if required
        averageVolumes[c].update( scaledVol[c] ) ;
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofSetColor(225);
    ofNoFill();
    
    // Audio Visualisers

    for (int c = 0; c < CHANNEL_COUNT; c++) {
        averageVolumes[c].draw( scaledVol[c] );
        spectrums[c].draw( chn[c] );
        classicBars[c].draw(fftOutput[c], eqOutput[c], fft[c]->getBinSize());
    }
    
    midiVis.draw();
        
    // Keep an eye on frameRate

    string msg = ofToString((int) ofGetFrameRate()) + " fps";
	ofDrawBitmapString(msg, ofGetWidth() - 60, ofGetHeight() - 10);
    
}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){	
	
    for (int c = 0; c < nChannels; c++) {
        float curVol = 0.0;
        
        // samples are "interleaved"
        int numCounted = 0;	
        
        for (int i = 0; i < bufferSize; i++){
            chn[c][i] = input[i*nChannels+c]*0.26;
            curVol += chn[c][i] * chn[c][i];
            
            // fft
            
            audioInput[c][i] = input[i*nChannels+c]*0.48;
            
        }
        
		numCounted+=nChannels;
        //this is how we get the mean of rms :) 
        curVol /= (float)numCounted;
        
        // this is how we get the root of rms :) 
        // curVol = sqrt( curVol );
        
        smoothedVol[c] *= 0.83;
        smoothedVol[c] += 0.07 * curVol;
        
        // FFT
        
//        memcpy(audioInput[c], &input[nChannels+c], sizeof(float) * bufferSize);
        fft[c]->setSignal(audioInput[c]);
        memcpy(fftOutput[c], fft[c]->getAmplitude(), sizeof(float) * fft[c]->getBinSize());
        for(int i = 0; i < fft[c]->getBinSize(); i++)
            eqOutput[c][i] = fftOutput[c][i] * eqFunction[c][i];
        fft[c]->setPolar(eqOutput[c], fft[c]->getPhase());
        fft[c]->clampSignal();

    }	
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	if( key == 's' ){
		soundStream.start();
	}
	
	if( key == 'e' ){
		soundStream.stop();
	}
}

//--------------------------------------------------------------
void testApp::newMidiMessage(ofxMidiEventArgs& eventArgs) {
	
	// store some data from midi message in variables
	value = eventArgs.byteOne;
	id = eventArgs.channel;
	port = eventArgs.port;
	timestamp = eventArgs.timestamp;
    midiVis.update(value);
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

