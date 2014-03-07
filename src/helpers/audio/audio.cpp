//
//  audio.cpp
//  queen2012
//
//  Created by James Acres on 12-08-19.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "audio.h"

namespace utils { 
namespace audio {
    
    vector<string> getDeviceNames() {
        vector<string> deviceNames;
        vector<RtAudio::DeviceInfo> deviceInfo = getDeviceInfo();

        for (int i=0; i< deviceInfo.size(); i++) {  
            if (deviceInfo[i].inputChannels > 0) {
                deviceNames.push_back( deviceInfo[i].name );
            }
        }

        return deviceNames;
    } 
    
    unsigned int getDeviceId( string deviceName ) {
        vector<RtAudio::DeviceInfo> deviceInfo = getDeviceInfo();
        
        for (int i=0; i< deviceInfo.size(); i++) {
            if ( deviceInfo[i].name == deviceName ) {
                return i;
            }
        }
        
        cout << "deviceName: " << deviceName << endl;
        ofLogWarning( "utils::audio", ("could not find device id for " + deviceName) );

        return -1;
    }

    vector<RtAudio::DeviceInfo> getDeviceInfo() {
        RtAudio *rtAudio = NULL;          
        vector<RtAudio::DeviceInfo> deviceInfo;
        
        try { 
            rtAudio = new RtAudio();
        } catch (RtError &error) {  
            error.printMessage(); 
            
            delete rtAudio;
            return deviceInfo;  
        }
        
        for (int i=0; i< rtAudio->getDeviceCount(); i++) {  
            try {  
                deviceInfo.push_back( rtAudio->getDeviceInfo(i) );
            } catch (RtError &error) {  
                error.printMessage();  
                break;  
            }  
        }
        
        delete rtAudio;
        
        return  deviceInfo;
    }
    
    RtAudio::DeviceInfo getDeviceInfo( unsigned int deviceId ) {
        RtAudio::DeviceInfo deviceInfo;
        vector<RtAudio::DeviceInfo> allDevicesInfo = getDeviceInfo();
        
        if ( deviceId < allDevicesInfo.size() ) {
            deviceInfo = allDevicesInfo[deviceId];
        }
        
        return getDeviceInfo()[deviceId];
    }
}
}