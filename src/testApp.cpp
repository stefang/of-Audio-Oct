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
	
	//if you want to set a different device id 
	soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
	
    channelCount = 2;
    int sizeBuffer = 128;

    for (int c = 0; c < channelCount; c++) {
        chn[c].assign(sizeBuffer, 0.0);
        volHistory[c].assign(400, 0.0);
        smoothedVol[c] = 0.0;
        scaledVol[c] = 0.0;
    }
	
	bufferCounter	= 0;
	drawCounter		= 0;
    
    // x output channels, 
	// x input channels
	// x samples per second
	// x samples per buffer
	// x num buffers (latency)

	soundStream.setup(this, 0, channelCount, 44100, sizeBuffer, 2);

}

//--------------------------------------------------------------
void testApp::update(){
	//lets scale the vol up to a 0-1 range 
    for (int c = 0; c < channelCount; c++) {
        scaledVol[c] = ofMap(smoothedVol[c], 0.0, 0.17, 0.0, 1.0, true);
        //lets record the volume into an array
        volHistory[c].push_back( scaledVol[c] );
        
        //if we are bigger the the size we want to record - lets drop the oldest value
        if( volHistory[c].size() >= 400 ){
            volHistory[c].erase(volHistory[c].begin(), volHistory[c].begin()+1);
        }
        
    }
    // Midi input recieved
    sprintf(msg,"value: %i, received from port: %i, id: %i \n\nwith %f milliseconds difference from last message",value,port,id,timestamp);

    
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(225);
	
	ofNoFill();
	
	// draw the X channels:
    for (int c = 0; c < channelCount; c++) {
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
            ofVertex((i*4)+2, 50 -chn[c][i]*90.0f);
        }
        ofEndShape(false);
        
		ofPopMatrix();
        ofPopStyle();
        
        // draw the average volume:
        
        ofPushStyle();
		ofPushMatrix();
		ofTranslate(543, 50+y, 0);
        
		ofSetColor(225);
		ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol[c] * 100.0, 0), 4, 18);
		ofRect(0, 0, 400, 100);
		
		ofSetColor(245, 58, 135);
		ofFill();		
		ofCircle(50, 50, scaledVol[c] * 25.0f);
		
		//lets draw the volume history as a graph
		ofBeginShape();
		for (int i = 0; i < volHistory[c].size(); i++){
			if( i == 0 ) ofVertex(i+1, 99);
            
			ofVertex(i+1, 101 - volHistory[c][i] * 70);
			
			if( i == volHistory[c].size() -1 ) ofVertex(i+1, 99);
		}
		ofEndShape(false);		
        
		ofPopMatrix();
        ofPopStyle();
        

    
    }
	
	drawCounter++;
	
	ofSetColor(225);
	string reportString = "buffers received: "+ofToString(bufferCounter)+"\ndraw routines called: "+ofToString(drawCounter)+"\nticks: " + ofToString(soundStream.getTickCount());
	ofDrawBitmapString(reportString, 565, 589);
    ofDrawBitmapString(msg,565,659);
		
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
        
        bufferCounter++;
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

