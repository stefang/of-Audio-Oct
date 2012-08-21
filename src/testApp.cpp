#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 
	
	ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);
	
    gui = new ofxUICanvas();
    
    vector<string> audio_inputs_ui; 
    
    try {  
        audioTemp = ofPtr<RtAudio>(new RtAudio());  
    } catch (RtError &error) {  
        error.printMessage();  
        return;  
    }
    
    for (int i=0; i< audioTemp->getDeviceCount(); i++) {  
        try {  
            if (audioTemp->getDeviceInfo(i).inputChannels > 0) {
                audio_inputs_ui.push_back(audioTemp->getDeviceInfo(i).name);
            }
        } catch (RtError &error) {  
            error.printMessage();  
            break;  
        }  
    }  
    
    vector<string> midi_inputs_ui; 
    
    for (int i = 0; i < midiIn.getPortList().size(); i++) {
        midi_inputs_ui.push_back(midiIn.getPortName(i));
    }
    
    midiIn.setVerbose(true);
    
    // Setup GUI
    
    ofxUIDropDownList *ai = (ofxUIDropDownList *) gui->addWidgetRight(new ofxUIDropDownList(200, "AUDIO INPUT", audio_inputs_ui, OFX_UI_FONT_MEDIUM)); 
    ai->setAutoClose(true);
    ai->setAllowMultiple(false);
    ofxUIDropDownList *mi = (ofxUIDropDownList *) gui->addWidgetRight(new ofxUIDropDownList(200, "MIDI INPUT", midi_inputs_ui, OFX_UI_FONT_MEDIUM)); 
    mi->setAutoClose(true);
    mi->setAllowMultiple(false);
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent); 
    gui->loadSettings("GUI/guiSettings.xml"); // this triggers the audio and midi setup as well as the setting load
        
    // Setup default colour
    for (int c = 0; c < channelCount; c++) {
        spectrums[c].setup();
        classicBars[c].setup();
        averageVolumes[c].setup();
        octaveEqs[c].setup();
    }

    midiVis.setup();
    audioInputSetup();   
        
    fbo.allocate(1460,720);
    glow.allocate(1460,720);
    glow.setRadius(5);
}

void testApp::audioInputSetup() {
    
    // x output channels, 
	// x input channels
	// x samples per second
	// x samples per buffer
	// x num buffers (latency)
        
    soundStream.setup(this, 0, channelCount, 44100, BUFFER_SIZE, 2);

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
    
    for (int c = 0; c < channelCount; c++) {

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
        
        FFTanalyzer[c].FFTOctaveAnalyzer::setup(44100, BUFFER_SIZE/channelCount, 1);
        FFTanalyzer[c].peakHoldTime = 15; // hold longer
        FFTanalyzer[c].peakDecayRate = 0.95f; // decay slower
        FFTanalyzer[c].linearEQIntercept = 0.3f; // reduced gain at lowest frequency
        FFTanalyzer[c].linearEQSlope = 0.4f; // increasing gain at higher frequencies
        
        // Create Visualisers
        spectrums.push_back( Spectrum(ofVec2f(20,(c*100+100)), c));
        classicBars.push_back( ClassicFftBars(ofVec2f(296,(c*100+100)), c));
        averageVolumes.push_back( AverageVolume(ofVec2f(856,(c*100+100))));
        octaveEqs.push_back( Octaves(ofVec2f(1276,(c*100+100)), c));

    }
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    for (int c = 0; c < channelCount; c++) {
        scaledVol[c] = ofMap(smoothedVol[c], 0.0, 0.17, 0.0, 1.0, true); // scale the vol up to a 0-1 range
        // Update Visualisers with audio data if required
        averageVolumes[c].update( scaledVol[c] );
        
        FFTanalyzer[c].calculate(eqOutput[c]);
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    fbo.begin();
    ofBackground(0, 0, 0);
    
    ofSetColor(225);
    ofNoFill();
    
    // Audio Visualisers

    for (int c = 0; c < channelCount; c++) {
        averageVolumes[c].draw( scaledVol[c] );
        spectrums[c].draw( chn[c] );
        classicBars[c].draw(fftOutput[c], eqOutput[c], fft[c]->getBinSize());
        octaveEqs[c].draw(FFTanalyzer[c].nAverages, FFTanalyzer[c].averages, FFTanalyzer[c].peaks);
    }
    
    midiVis.draw();
    
    fbo.end();
    glow.setTexture(fbo.getTextureReference());
    glow.update();

    glow.draw(0,0);
        
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
    gui->toggleVisible();
}

//--------------------------------------------------------------
void testApp::newMidiMessage(ofxMidiMessage& msg) {
    
    midiMessage = msg;
    
    // midiMessage.channel
    // midiMessage.pitch
    // midiMessage.velocity
    // midiMessage.control
    // midiMessage.value
	
    midiVis.update(midiMessage.pitch);
    midiVis.colour.setHue(midiMessage.pitch);
    
    for (int c = 0; c < channelCount; c++) {
        spectrums[c].colour.setHue(midiMessage.pitch);
        classicBars[c].colour.setHue(midiMessage.pitch);
        averageVolumes[c].colour.setHue(midiMessage.pitch);
        octaveEqs[c].colour.setHue(midiMessage.pitch);
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
    
    string inputName;
    
    if (e.widget->getName() == "AUDIO INPUT" || e.widget->getParent()->getName() == "AUDIO INPUT" ) {

        soundStream.stop();
        soundStream.close();
        
        if (e.widget->getName() == "AUDIO INPUT") {
            ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
            vector<ofxUIWidget *> &selected = ddlist->getSelected(); 
            for(int i = 0; i < selected.size(); i++)
            {
                inputName = selected[i]->getName();
            }
        } else {
            ofxUIButton *button = (ofxUIButton *) e.widget;
            if (button->getValue() == 1) {
                inputName = button->getName();
            }
        }

        for (int j=0; j< audioTemp->getDeviceCount(); j++) {
            if (audioTemp->getDeviceInfo(j).name == inputName) {
                soundStream.setDeviceID(j);
                channelCount = audioTemp->getDeviceInfo(j).inputChannels;
                if ( channelCount > MAX_CHANNEL_COUNT)
                    channelCount = MAX_CHANNEL_COUNT;
                audioInputSetup();
            }
        }
    }
    
    if (e.widget->getName() == "MIDI INPUT" || e.widget->getParent()->getName() == "MIDI INPUT") {
        midiIn.removeListener(this);
        midiIn.closePort();
        if (e.widget->getName() == "MIDI INPUT") {
            ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
            vector<ofxUIWidget *> &selected = ddlist->getSelected(); 
            for(int i = 0; i < selected.size(); i++)
            {
                inputName = selected[i]->getName();
                
            }
        } else {
            ofxUIButton *button = (ofxUIButton *) e.widget;
            if (button->getValue() == 1) {
                inputName = button->getName();
            }
        }
        midiIn.openPort(inputName);
        midiIn.addListener(this);
    }
}

//--------------------------------------------------------------
void testApp::exit()
{
    gui->saveSettings("GUI/guiSettings.xml");     
    delete gui; 	
}