//
//  audio.h
//  Rackabeat2013
//
//  Created by James Acres on 12-08-19.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef queen2012_audio_h
#define queen2012_audio_h

#include "ofMain.h"
#include "RtAudio.h"

namespace utils { 
namespace audio {

    vector<string> getDeviceNames();
    unsigned int getDeviceId( string deviceName );
    
    vector<RtAudio::DeviceInfo> getDeviceInfo();
    RtAudio::DeviceInfo getDeviceInfo( unsigned int deviceId );
}
}
    
#endif
