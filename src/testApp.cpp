#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 
	
	ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);	
    
    midiIn.listPorts();
	midiIn.openPort(0);
	midiIn.addListener(this);
	
	// 0 output channels, 
	// 6 input channels
	// 44100 samples per second
	// 128 samples per buffer
	// 4 num buffers (latency)
	
	soundStream.listDevices();
	
	//if you want to set a different device id 
	soundStream.setDeviceID(4); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
	
	int sizeBuffer = 128;
	
	
	chn[0].assign(sizeBuffer, 0.0);
	chn[1].assign(sizeBuffer, 0.0);
	chn[2].assign(sizeBuffer, 0.0);
	chn[3].assign(sizeBuffer, 0.0);
	chn[4].assign(sizeBuffer, 0.0);
	chn[5].assign(sizeBuffer, 0.0);
    
	volHistory.assign(400, 0.0);
	
	bufferCounter	= 0;
	drawCounter		= 0;
	smoothedVol     = 0.0;
	scaledVol		= 0.0;

	soundStream.setup(this, 0, 6, 44100, sizeBuffer, 2);

}

//--------------------------------------------------------------
void testApp::update(){
	//lets scale the vol up to a 0-1 range 
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);

	//lets record the volume into an array
	volHistory.push_back( scaledVol );
	
	//if we are bigger the the size we want to record - lets drop the oldest value
	if( volHistory.size() >= 400 ){
		volHistory.erase(volHistory.begin(), volHistory.begin()+1);
	}
    
    // Midi input recieved
    sprintf(msg,"value: %i, received from port: %i, id: %i \n\nwith %f milliseconds difference from last message",value,port,id,timestamp);

    
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(225);
	
	ofNoFill();
	
	// draw the six channels:
    for (int c = 0; c < 6; c++) {
        int y = (c * 100);
        ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 50 + y, 0);
        
		ofSetColor(225);
        sprintf(title,"Channel: %i",c);

		ofDrawBitmapString(title, 4, 18);
		
		ofSetLineWidth(1);	
		ofRect(0, 0, 512, 100);
        
		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
        
        ofBeginShape();
        for (int i = 0; i < chn[c].size(); i++){
            ofVertex(i*4, 50 -chn[c][i]*90.0f);
        }
        ofEndShape(false);
        
		ofPopMatrix();
        ofPopStyle();
        
    }
	
	// draw the average volume:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(565, 170, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
		ofRect(0, 0, 400, 400);
		
		ofSetColor(245, 58, 135);
		ofFill();		
		ofCircle(200, 200, scaledVol * 190.0f);
		
		//lets draw the volume history as a graph
		ofBeginShape();
		for (int i = 0; i < volHistory.size(); i++){
			if( i == 0 ) ofVertex(i, 400);

			ofVertex(i, 400 - volHistory[i] * 70);
			
			if( i == volHistory.size() -1 ) ofVertex(i, 400);
		}
		ofEndShape(false);		
			
		ofPopMatrix();
	ofPopStyle();
	
	drawCounter++;
	
	ofSetColor(225);
	string reportString = "buffers received: "+ofToString(bufferCounter)+"\ndraw routines called: "+ofToString(drawCounter)+"\nticks: " + ofToString(soundStream.getTickCount());
	ofDrawBitmapString(reportString, 565, 589);
    ofDrawBitmapString(msg,565,659);
		
}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){	
	
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;	

	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
	for (int i = 0; i < bufferSize; i++){
        for (int c = 0; c < 6; c++) {
            chn[c][i]	= input[i*6+c]*0.5;
            curVol += chn[c][i] * chn[c][i];
        }
		numCounted+=6;
	}
	
	//this is how we get the mean of rms :) 
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :) 
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
    // smoothedVol = curVol;
	
	bufferCounter++;
	
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

