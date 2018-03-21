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

#include "OMPlayer.hpp"
#include "OMAudioBufferSource.hpp"
#include <cstring>


OMPlayer::OMPlayer() {
    
	//std::cout << std::endl << "Allocating audio player" << std::endl;
    outputChannelsRouting.resize( 0 );
}


OMPlayer::~OMPlayer()
{
    closeAudioDevice();
}


////////////////////////
////////////////////////
////////////////////////

// Number of different types of devices
int OMPlayer::getDevicesTypeCount() {
    return getAvailableDeviceTypes().size();
}

// Name of a given device type
String OMPlayer::getDeviceTypeName(int i) {
    AudioIODeviceType* device_type;
    if (i < getAvailableDeviceTypes().size()) {
        device_type = getAvailableDeviceTypes()[i];
        return device_type->getTypeName();
    } else {
        return "Error";
    }
}

// e.g. "CoreAudio", "ASIO", etc.
void OMPlayer::setDeviceType(String type) {
    setCurrentAudioDeviceType(type, false);
}

String OMPlayer::getCurrentDeviceType(){
    return getCurrentAudioDeviceType();
}


// Number of input devices for a given device type
int OMPlayer::getInputDevicesCountForType(int num_device_type) {
    AudioIODeviceType* device_type = getAvailableDeviceTypes()[num_device_type];
    device_type->scanForDevices();
    return device_type->getDeviceNames(true).size();
}

// Number of output devices for a given device type
int OMPlayer::getOutputDevicesCountForType(int num_device_type) {
    AudioIODeviceType* device_type = getAvailableDeviceTypes()[num_device_type];
    device_type->scanForDevices();
    return device_type->getDeviceNames().size();
}


// Number of input devices -- any device type
int OMPlayer::getInputDevicesCount() {
    int n = 0;
    for (int i = 0; i < getDevicesTypeCount() ; ++i) {
        n += getInputDevicesCountForType(i);
    }
    return n;
}

// Number of output devices -- any device type
int OMPlayer::getOutputDevicesCount() {
    int n = 0;
    for (int i = 0; i < getDevicesTypeCount() ; ++i) {
        n += getOutputDevicesCountForType(i);
    }
    return n;
}

String OMPlayer::getNthInputDeviceName(int device_type_num, int device_num){
    if (device_type_num < getAvailableDeviceTypes().size()) {
        const StringArray InputdeviceNames = getAvailableDeviceTypes()[device_type_num]->getDeviceNames(true);
        if (device_num < getInputDevicesCountForType(device_type_num)) {
            return InputdeviceNames[device_num];
        } else {
            return "Error accessing device type";
        }
    } else {
        return "Error accessing input device name";
    }
}

String OMPlayer::getNthOutputDeviceName(int device_type_num, int device_num){
    if (device_type_num < getAvailableDeviceTypes().size()) {
        const StringArray OutputdeviceNames = getAvailableDeviceTypes()[device_type_num]->getDeviceNames();
        if (device_num < getOutputDevicesCountForType(device_type_num)) {
            return OutputdeviceNames[device_num];
        } else {
            return "Error accessing device type";
        }
    } else {
        return "Error accessing output device name";
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

String OMPlayer::getCurrentDeviceName() {
	if (getCurrentAudioDevice() == nullptr) return "";
	else return getCurrentAudioDevice()->getName();
}

int OMPlayer::getAvailableSampleRatesCount() {
	if (getCurrentAudioDevice() == nullptr) return 0;
	else return getCurrentAudioDevice()->getAvailableSampleRates().size();
}

int OMPlayer::getNthAvailableSampleRate(int n) {
    return (int)getCurrentAudioDevice()->getAvailableSampleRates()[n];
}

int OMPlayer::getCurrentSampleRate() {
	if (getCurrentAudioDevice() == nullptr) return 0;
	else return (int)getCurrentAudioDevice()->getCurrentSampleRate();
}

int OMPlayer::setSampleRate(int sr) {
    AudioDeviceSetup res;
    getAudioDeviceSetup(res);
    res.sampleRate = (double)sr;
    setAudioDeviceSetup(res,true);
    return sr;
}

int OMPlayer::getAvailableBufferSizesCount() {
	if (getCurrentAudioDevice() == nullptr) return 0;
	else return getCurrentAudioDevice()->getAvailableBufferSizes().size();
}

int OMPlayer::getNthAvailableBufferSize(int n) {
    return getCurrentAudioDevice()->getAvailableBufferSizes()[n];
}

int OMPlayer::getDefaultBufferSize() {
	if (getCurrentAudioDevice() == nullptr) return 0;
	else return getCurrentAudioDevice()->getDefaultBufferSize();
}

int OMPlayer::getCurrentBufferSize() {
	if (getCurrentAudioDevice() == nullptr) return 0;
	else return getCurrentAudioDevice()->getCurrentBufferSizeSamples();
}

int OMPlayer::setBufferSize(int size) {
    AudioDeviceSetup res;
    getAudioDeviceSetup(res);
    res.bufferSize = size;
    setAudioDeviceSetup(res,true);
    return size;
}

int OMPlayer::getOutputChannelsCount() {
	if (getCurrentAudioDevice() == nullptr) return 0;
	else return getCurrentAudioDevice()->getOutputChannelNames().size();
}

int OMPlayer::getInputChannelsCount() {
	if (getCurrentAudioDevice() == nullptr) return 0;
	else return getCurrentAudioDevice()->getInputChannelNames().size();
}


void OMPlayer::initializeAudioChannels(int inChannels, int outChannels) {
	AudioDeviceSetup res;
	getAudioDeviceSetup(res);
	closeAudioDevice();
	std::cout << "Initializing audio channels [" << inChannels << "x" << outChannels << "]" << std::endl;
	initialiseWithDefaultDevices(inChannels, outChannels);
	//outputChannelsRouting.resize(outChannels);
	//std::fill(outputChannelsRouting.begin(), outputChannelsRouting.end(), -1);
	setAudioDeviceSetup(res, true);
	std::cout << "Selected device = " << getCurrentDeviceName() << std::endl;
}


int OMPlayer::setOutputChannelsMapping(int n, int *map) {
    
    int error = 0;
    int destChannel = -1;
    int nOuts = getOutputChannelsCount();
    
    outputChannelsRouting.resize(n);
    std::fill(outputChannelsRouting.begin(), outputChannelsRouting.end(), -1);
    
    std::cout << "Start Channel Mapping (" << nOuts << " channels open)" << std::endl;
    for (int i = 0; i < n ; i++) {
        destChannel = map[i];
        std::cout << "Routing channel " << i << " to output " << destChannel << std::endl;
        //if (i >= outputChannelsRouting.size()) {
        //    std::cout << "ERROR: Input channel " << i << " not available !" << std::endl;
        //    error = -1;
        // } else {
        if (destChannel >= 0 && destChannel >= nOuts ) {
                std::cout << "ERROR: Output channel " << destChannel << " not available !" << std::endl;
                destChannel = -2;
                error = -2;
            }
        
        std::cout << "=> " << destChannel << std::endl;
        outputChannelsRouting.operator[](i) = destChannel ;
    }
    
    return error;
}


int OMPlayer::setInputDevice(int deviceNum) {
	AudioDeviceSetup _s;
	getAudioDeviceSetup(_s);
	_s.inputDeviceName = getCurrentDeviceTypeObject()->getDeviceNames()[deviceNum];
	setAudioDeviceSetup(_s, true);
	return deviceNum;
}

int OMPlayer::setOutputDevice(int deviceNum) {
	AudioDeviceSetup _s;
	getAudioDeviceSetup(_s);
	_s.outputDeviceName = getCurrentDeviceTypeObject()->getDeviceNames()[deviceNum];
	setAudioDeviceSetup(_s, true);
	return deviceNum;
}


// stop using this ?
void OMPlayer::audioSetup(int inputDevice, int outputDevice, int inChannels, int outChannels, double sr, int buffer_size) {
	closeAudioDevice();
	AudioDeviceSetup newSetup;
	if (outputDevice >= 0) { newSetup.outputDeviceName = getCurrentDeviceTypeObject()->getDeviceNames()[outputDevice]; }
	if (inputDevice >= 0) { newSetup.inputDeviceName = getCurrentDeviceTypeObject()->getDeviceNames()[inputDevice]; }
	newSetup.sampleRate = sr;
	newSetup.bufferSize = buffer_size;
	newSetup.useDefaultInputChannels = true;
	newSetup.useDefaultOutputChannels = true;
	//newSetup.inputChannels = static_cast<BigInteger>(numInputChannels);
	//newSetup.outputChannels = static_cast<BigInteger>(numOutputChannels);

	std::cout << "INITIALIZING AUDIO MANAGER FOR '" << newSetup.outputDeviceName << "' (" << outChannels << " channels)" << std::endl;
	initialise(inChannels, outChannels, 0, true, String(), &newSetup);
	std::cout << "Selected device = " << getCurrentDeviceName() << std::endl;

	// outputChannelsRouting.resize(outChannels);
    // std::fill(outputChannelsRouting.begin(), outputChannelsRouting.end(), -1);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

int OMPlayer::registerBuffer(AudioSourcePlayer *sp) {
	IgnoreUnused( sp );
    return ++bufferRegisterCount;
}

int OMPlayer::unregisterBuffer(AudioSourcePlayer *sp) {
	IgnoreUnused( sp );
    return --bufferRegisterCount;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


/*
 int OMPlayer::setActiveOutputChannels(int n, int *mask) {
 BigInteger channelmask;
 channelmask.clear();
 for (int i = 0; i < n; i++) {
 if (mask[i] == 1) channelmask.setBit(i);
 }
 AudioDeviceSetup res;
 getAudioDeviceSetup(res);
 res.outputChannels = channelmask;
 setAudioDeviceSetup(res,true);
 return 1;
 }
 */

 /*
 OMPlayer::OMPlayer(int nIn, int nOut)
 {
 std::cout << std::endl << "ALLOCATING AUDIO PLAYER (with " << nIn << "x" << nOut << " channels)" << std::endl;
 initialise(nIn,nOut,0,true);
 outputChannelsRouting = new std::vector<int>(nOut,-1);
 }
 */

 /*
 OMPlayer::OMPlayer(String inputDeviceName, String outputDeviceName, int nIn, int nOut, double sr) {

 std::cout << std::endl << "ALLOCATING AUDIO PLAYER (with " << inputDeviceName << "/" << outputDeviceName << " and " << nIn << "x" << nOut << " channels)" << std::endl;

 initialise(nIn,nOut,0,true);
 outputChannelsRouting = new std::vector<int>(nOut,-1);

 AudioDeviceSetup newSetup;
 newSetup.outputDeviceName = outputDeviceName;
 newSetup.inputDeviceName = inputDeviceName;
 newSetup.sampleRate = sr;
 setAudioDeviceSetup(newSetup, true);
 }
 */
