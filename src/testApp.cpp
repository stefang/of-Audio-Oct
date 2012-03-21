#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 
	
	ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);	
    
    midiIn.listPorts();
	midiIn.openPort(0);
	midiIn.addListener(this);
	
	soundStream.listDevices();
	soundStream.setDeviceID(0);
	
    channelCount = 2;
    int sizeBuffer = 128;

    for (int c = 0; c < channelCount; c++) {
        chn[c].assign(sizeBuffer, 0.0);
        smoothedVol[c] = 0.0;
        scaledVol[c] = 0.0;
        
        // Create Visualisers
        averageVolumes.push_back( AverageVolume(ofVec2f(562,(c*100+50))));
        spectrums.push_back( Spectrum(ofVec2f(50,(c*100+50)), c));
        
    }
    
    // x output channels, 
	// x input channels
	// x samples per second
	// x samples per buffer
	// x num buffers (latency)

	soundStream.setup(this, 0, channelCount, 44100, sizeBuffer, 2);

}

//--------------------------------------------------------------
void testApp::update(){
	 
    for (int c = 0; c < channelCount; c++) {
        scaledVol[c] = ofMap(smoothedVol[c], 0.0, 0.17, 0.0, 1.0, true); // scale the vol up to a 0-1 range
        // Update Visualisers with audio data
        averageVolumes[c].update( scaledVol[c] ) ;
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofSetColor(225);
    ofNoFill();
    
    // Audio Visualisers

    for (int c = 0; c < channelCount; c++) {
        averageVolumes[c].draw( scaledVol[c] );
        spectrums[c].draw( chn[c] );
    }
    
}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){	
	
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
    for (int c = 0; c < channelCount; c++) {
        float curVol = 0.0;
        
        // samples are "interleaved"
        int numCounted = 0;	
        
        for (int i = 0; i < bufferSize; i++){
            chn[c][i]	= input[i*channelCount+c]*0.5;
            curVol += chn[c][i] * chn[c][i];
        }
		numCounted+=channelCount;
        //this is how we get the mean of rms :) 
        curVol /= (float)numCounted;
        
        // this is how we get the root of rms :) 
        curVol = sqrt( curVol );
        
        smoothedVol[c] *= 0.93;
        smoothedVol[c] += 0.07 * curVol;
        // smoothedVol = curVol;
        
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

