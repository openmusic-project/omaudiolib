/*
 ==============================================================================
 
 This file is part of the OM audio library:
 an audio IO and playback utiloity based on the Juce framework
 
 Copyright (C) 2016-2018 Dimitri Bouche, Jean Bresson (IRCAM)
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 
 ==============================================================================
 */


#ifndef OMPlayer_hpp
#define OMPlayer_hpp

#define N_MAX_BUFFERS 512 // not used

#include "../JuceLibraryCode/JuceHeader.h"
#include <string>
#include <vector>

using std::vector;


/************************************************************************************/
//@brief           OMPlayer class definition
//@comment         A structure to send audio buffer data to the sound card
/************************************************************************************/
class OMPlayer : public AudioDeviceManager {
    
private:
    int bufferRegisterCount = 0;
    
public:
    OMPlayer();
    ~OMPlayer();
    
    /************************************************************************************/
    //@brief           Custom transport states definitions
    //@comment         AudioTransportSource has private states attributes but it
    //                 is clearer to have a custom management
    /************************************************************************************/
    enum class State
    {
        Playing,
        Paused,
        Stopped,
    };
    
    std::vector<int> outputChannelsRouting;
    
    int getDevicesTypeCount();
    String getDeviceTypeName(int i);
    void setDeviceType(String type);
    String getCurrentDeviceType();
    
    int getInputDevicesCountForType(int num_device_type);
    int getOutputDevicesCountForType(int num_device_type);
    int getInputDevicesCount();
    int getOutputDevicesCount();
    String getNthInputDeviceName(int device_type_num, int device_num);
    String getNthOutputDeviceName(int device_type_num, int device_num);
	String getCurrentDeviceName();
	int setInputDevice(int deviceNum);
	int setOutputDevice(int deviceNum);

    int getOutputChannelsCount();
    int getInputChannelsCount();
	void initializeAudioChannels(int inChannels, int outChannels);
	int setOutputChannelsMapping(int n, int *map);

    int getAvailableSampleRatesCount();
    int getNthAvailableSampleRate(int n);
    int getCurrentSampleRate();
    int setSampleRate (int SR);
    
    int getAvailableBufferSizesCount();
    int getNthAvailableBufferSize(int n);
    int getDefaultBufferSize();
    int getCurrentBufferSize();
    int setBufferSize (int size);
    

	void audioSetup(int inputDevice, int outputDevice, int inChannels, int outChannels, double sr, int buffer_size);

    int registerBuffer(AudioSourcePlayer *sp);
    int unregisterBuffer(AudioSourcePlayer *sp);
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OMPlayer)
};


#endif /* OMPlayer_hpp */

