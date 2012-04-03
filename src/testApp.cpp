#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 
	
	ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);
	
    gui = new ofxUICanvas(10,ofGetViewportHeight()-55,ofGetViewportWidth()-20,55);
    
    vector<string> audio_inputs; 
    audio_inputs.push_back("0");
    audio_inputs.push_back("1");
    audio_inputs.push_back("2");
    audio_inputs.push_back("3");
    audio_inputs.push_back("4");
    audio_inputs.push_back("5");
    audio_inputs.push_back("6");
    
    vector<string> midi_inputs; 
    for (int i = 0; i < midiIn.portNames.size(); i++) {
        midi_inputs.push_back(ofToString(i));
    }
    
    gui->addWidgetRight(new ofxUIRadio( 10, 10, "Audio Input", audio_inputs, OFX_UI_ORIENTATION_HORIZONTAL));
    gui->addWidgetRight(new ofxUIRadio( 10, 10, "MIDI Input", midi_inputs, OFX_UI_ORIENTATION_HORIZONTAL));
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent); 
    gui->loadSettings("GUI/guiSettings.xml"); // this triggers the audio and midi setup as well as the setting load
        
    // Setup default colour
    for (int c = 0; c < CHANNEL_COUNT; c++) {
        spectrums[c].setup();
        classicBars[c].setup();
        averageVolumes[c].setup();
        octaveEqs[c].setup();
    }

    midiVis.setup();
        
}

void testApp::audioInputSetup() {
    
    // x output channels, 
	// x input channels
	// x samples per second
	// x samples per buffer
	// x num buffers (latency)
    
    soundStream.setup(this, 0, CHANNEL_COUNT, 44100, BUFFER_SIZE, 2);

    // Clear all vectors...
    
    // First the visualizers to try and prevent glitches
    
    spectrums.clear();
    classicBars.clear();
    averageVolumes.clear();
    octaveEqs.clear();

    // Then everything else
    
    smoothedVol.clear();
    scaledVol.clear();

    fft.clear();
    audioInput.clear();
    fftOutput.clear();
    eqFunction.clear();
    eqOutput.clear();
    ifftOutput.clear();
    
    // Build them again
    
    for (int c = 0; c < CHANNEL_COUNT; c++) {

        // chn.push_back(new float[BUFFER_SIZE]);
        
        chn[c].assign(BUFFER_SIZE, 0.0f);
        
        smoothedVol.push_back(0.0);
        scaledVol.push_back(0.0);
        
        fft.push_back(ofxFft::create(BUFFER_SIZE, OF_FFT_WINDOW_BARTLETT));

        audioInput.push_back(new float[BUFFER_SIZE]);
        fftOutput.push_back(new float[fft[c]->getBinSize()]);
        eqFunction.push_back(new float[fft[c]->getBinSize()]);
        eqOutput.push_back(new float[fft[c]->getBinSize()]);
        ifftOutput.push_back(new float[BUFFER_SIZE]);
        
        for(int i = 0; i < fft[c]->getBinSize(); i++)
            eqFunction[c][i] = (float) (fft[c]->getBinSize() - i) / (float) (fft[c]->getBinSize());
        
        // Octave
        
        FFTanalyzer[c].FFTOctaveAnalyzer::setup(44100, BUFFER_SIZE/CHANNEL_COUNT, 1);
        FFTanalyzer[c].peakHoldTime = 15; // hold longer
        FFTanalyzer[c].peakDecayRate = 0.95f; // decay slower
        FFTanalyzer[c].linearEQIntercept = 0.3f; // reduced gain at lowest frequency
        FFTanalyzer[c].linearEQSlope = 0.4f; // increasing gain at higher frequencies
        
        // Create Visualisers
        spectrums.push_back( Spectrum(ofVec2f(20,(c*100+50)), c));
        classicBars.push_back( ClassicFftBars(ofVec2f(296,(c*100+50)), c));
        averageVolumes.push_back( AverageVolume(ofVec2f(856,(c*100+50))));
        octaveEqs.push_back( Octaves(ofVec2f(1276,(c*100+50)), c));

    }
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    for (int c = 0; c < CHANNEL_COUNT; c++) {
        scaledVol[c] = ofMap(smoothedVol[c], 0.0, 0.17, 0.0, 1.0, true); // scale the vol up to a 0-1 range
        // Update Visualisers with audio data if required
        averageVolumes[c].update( scaledVol[c] );
        
        FFTanalyzer[c].calculate(eqOutput[c]);
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
        octaveEqs[c].draw(FFTanalyzer[c].nAverages, FFTanalyzer[c].averages, FFTanalyzer[c].peaks);
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
}

//--------------------------------------------------------------
void testApp::newMidiMessage(ofxMidiEventArgs& eventArgs) {
	
	// store some data from midi message in variables
	value = eventArgs.byteOne;
	id = eventArgs.channel;
	port = eventArgs.port;
	timestamp = eventArgs.timestamp;
    midiVis.update(value);
    midiVis.colour.setHue(value);
    
    for (int c = 0; c < CHANNEL_COUNT; c++) {
        spectrums[c].colour.setHue(value);
        classicBars[c].colour.setHue(value);
        averageVolumes[c].colour.setHue(value);
        octaveEqs[c].colour.setHue(value);
    }

    
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

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    
    if (e.widget->getParent()->getName() == "Audio Input") {
        soundStream.stop();
        soundStream.close();
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        if (toggle->getValue() == 1)
            soundStream.setDeviceID(ofToInt(e.widget->getName()));
        audioInputSetup();
    }
    
    if (e.widget->getParent()->getName() == "MIDI Input") {
        midiIn.removeListener(this);
        midiIn.closePort();
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        if (toggle->getValue() == 1)
            midiIn.openPort(ofToInt(e.widget->getName()));
        midiIn.addListener(this);
    }
    
}

//--------------------------------------------------------------
void testApp::exit()
{
    gui->saveSettings("GUI/guiSettings.xml");     
    delete gui; 	
}