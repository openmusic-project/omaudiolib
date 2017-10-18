/************************************************************************************/
/*!
 *  @file       OMJucePlayer.hpp
 *  @brief      OMJucePlayer implementation
 *  @author     Dimitri Bouche
 *  @date       04/11/2016
 *
 */
/************************************************************************************/
#include "OMJucePlayer.hpp"
#include "OMAudioBuffer.hpp"
#include <cstring>


OMJucePlayer::OMJucePlayer() {
    
	//std::cout << std::endl << "Allocating audio player" << std::endl;
    outputChannelsRouting = new std::vector<int>(0,-1);
}


OMJucePlayer::~OMJucePlayer()
{
    delete outputChannelsRouting;
    closeAudioDevice();
}


////////////////////////
////////////////////////
////////////////////////

// Number of different types of devices
int OMJucePlayer::getDevicesTypeCount() {
    return getAvailableDeviceTypes().size();
}

// Name of a given device type
const char *OMJucePlayer::getDeviceTypeName(int i) {
    AudioIODeviceType* device_type;
    if (i < getAvailableDeviceTypes().size()) {
        device_type = getAvailableDeviceTypes()[i];
        return static_cast<const char*>(device_type->getTypeName().toUTF8());
    } else {
        return "Error";
    }
}

// e.g. "CoreAudio", "ASIO", etc.
void OMJucePlayer::setDeviceType(String type) {
    setCurrentAudioDeviceType(type, false);
}

const char * OMJucePlayer::getCurrentDeviceType(){
    return static_cast<const char*>(getCurrentAudioDeviceType().toUTF8());
}


// Number of input devices for a given device type
int OMJucePlayer::getInputDevicesCountForType(int num_device_type) {
    AudioIODeviceType* device_type = getAvailableDeviceTypes()[num_device_type];
    device_type->scanForDevices();
    return device_type->getDeviceNames(true).size();
}

// Number of output devices for a given device type
int OMJucePlayer::getOutputDevicesCountForType(int num_device_type) {
    AudioIODeviceType* device_type = getAvailableDeviceTypes()[num_device_type];
    device_type->scanForDevices();
    return device_type->getDeviceNames().size();
}


// Number of input devices -- any device type
int OMJucePlayer::getInputDevicesCount() {
    int n = 0;
    for (int i = 0; i < getDevicesTypeCount() ; ++i) {
        n += getInputDevicesCountForType(i);
    }
    return n;
}

// Number of output devices -- any device type
int OMJucePlayer::getOutputDevicesCount() {
    int n = 0;
    for (int i = 0; i < getDevicesTypeCount() ; ++i) {
        n += getOutputDevicesCountForType(i);
    }
    return n;
}

const char* OMJucePlayer::getNthInputDeviceName(int device_type_num, int device_num){
    if (device_type_num < getAvailableDeviceTypes().size()) {
        const StringArray InputdeviceNames = getAvailableDeviceTypes()[device_type_num]->getDeviceNames(true);
        if (device_num < getInputDevicesCountForType(device_type_num)) {
            return static_cast<const char*>(InputdeviceNames[device_num].toUTF8());
        } else {
            return "Error accessing device type";
        }
    } else {
        return "Error accessing input device name";
    }
}

const char* OMJucePlayer::getNthOutputDeviceName(int device_type_num, int device_num){
    if (device_type_num < getAvailableDeviceTypes().size()) {
        const StringArray OutputdeviceNames = getAvailableDeviceTypes()[device_type_num]->getDeviceNames();
        if (device_num < getOutputDevicesCountForType(device_type_num)) {
            return static_cast<const char*>(OutputdeviceNames[device_num].toUTF8());
        } else {
            return "Error accessing device type";
        }
    } else {
        return "Error accessing output device name";
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

const char* OMJucePlayer::getCurrentDeviceName() {
	if (getCurrentAudioDevice() == nullptr) return "";
	else return getCurrentAudioDevice()->getName().toUTF8();
}

int OMJucePlayer::getAvailableSampleRatesCount() {
	if (getCurrentAudioDevice() == nullptr) return 0;
	else return getCurrentAudioDevice()->getAvailableSampleRates().size();
}

int OMJucePlayer::getNthAvailableSampleRate(int n) {
    return (int)getCurrentAudioDevice()->getAvailableSampleRates()[n];
}

int OMJucePlayer::getCurrentSampleRate() {
	if (getCurrentAudioDevice() == nullptr) return 0;
	else return (int)getCurrentAudioDevice()->getCurrentSampleRate();
}

int OMJucePlayer::setSampleRate(int sr) {
    AudioDeviceSetup res;
    getAudioDeviceSetup(res);
    res.sampleRate = (double)sr;
    setAudioDeviceSetup(res,true);
    return sr;
}

int OMJucePlayer::getAvailableBufferSizesCount() {
	if (getCurrentAudioDevice() == nullptr) return 0;
	else return getCurrentAudioDevice()->getAvailableBufferSizes().size();
}

int OMJucePlayer::getNthAvailableBufferSize(int n) {
    return getCurrentAudioDevice()->getAvailableBufferSizes()[n];
}

int OMJucePlayer::getDefaultBufferSize() {
	if (getCurrentAudioDevice() == nullptr) return 0;
	else return getCurrentAudioDevice()->getDefaultBufferSize();
}

int OMJucePlayer::getCurrentBufferSize() {
	if (getCurrentAudioDevice() == nullptr) return 0;
	else return getCurrentAudioDevice()->getCurrentBufferSizeSamples();
}

int OMJucePlayer::setBufferSize(int size) {
    AudioDeviceSetup res;
    getAudioDeviceSetup(res);
    res.bufferSize = size;
    setAudioDeviceSetup(res,true);
    return size;
}

int OMJucePlayer::getOutputChannelsCount() {
	if (getCurrentAudioDevice() == nullptr) return 0;
	else return getCurrentAudioDevice()->getOutputChannelNames().size();
}

int OMJucePlayer::getInputChannelsCount() {
	if (getCurrentAudioDevice() == nullptr) return 0;
	else return getCurrentAudioDevice()->getInputChannelNames().size();
}


void OMJucePlayer::initializeAudioChannels(int inChannels, int outChannels) {
	AudioDeviceSetup res;
	getAudioDeviceSetup(res);
	closeAudioDevice();
	std::cout << "Initializing audio channels [" << inChannels << "x" << outChannels << "]" << std::endl;
	initialiseWithDefaultDevices(inChannels, outChannels);
	//outputChannelsRouting->resize(outChannels);
	//std::fill(outputChannelsRouting->begin(), outputChannelsRouting->end(), -1);
	setAudioDeviceSetup(res, true);
	std::cout << "Selected device = " << getCurrentDeviceName() << std::endl;
}


int OMJucePlayer::setOutputChannelsMapping(int n, int *map) {
    
    int error = 0;
    int destChannel = -1;
    int nOuts = getOutputChannelsCount();
    
    outputChannelsRouting->resize(n);
    std::fill(outputChannelsRouting->begin(), outputChannelsRouting->end(), -1);
    
    std::cout << "Start Channel Mapping (" << nOuts << " channels open)" << std::endl;
    for (int i = 0; i < n ; i++) {
        destChannel = map[i];
        std::cout << "Routing channel " << i << " to output " << destChannel << std::endl;
        //if (i >= outputChannelsRouting->size()) {
        //    std::cout << "ERROR: Input channel " << i << " not available !" << std::endl;
        //    error = -1;
        // } else {
        if (destChannel >= 0 && destChannel >= nOuts ) {
                std::cout << "ERROR: Output channel " << destChannel << " not available !" << std::endl;
                destChannel = -2;
                error = -2;
            }
        
        std::cout << "=> " << destChannel << std::endl;
        outputChannelsRouting->operator[](i) = destChannel ;
    }
    
    return error;
}


int OMJucePlayer::setInputDevice(int deviceNum) {
	AudioDeviceSetup _s;
	getAudioDeviceSetup(_s);
	_s.inputDeviceName = getCurrentDeviceTypeObject()->getDeviceNames()[deviceNum];
	setAudioDeviceSetup(_s, true);
	return deviceNum;
}

int OMJucePlayer::setOutputDevice(int deviceNum) {
	AudioDeviceSetup _s;
	getAudioDeviceSetup(_s);
	_s.outputDeviceName = getCurrentDeviceTypeObject()->getDeviceNames()[deviceNum];
	setAudioDeviceSetup(_s, true);
	return deviceNum;
}


// stop using this ?
void OMJucePlayer::audioSetup(int inputDevice, int outputDevice, int inChannels, int outChannels, double sr, int buffer_size) {
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

	// outputChannelsRouting->resize(outChannels);
    // std::fill(outputChannelsRouting->begin(), outputChannelsRouting->end(), -1);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

int OMJucePlayer::registerBuffer(AudioSourcePlayer *sp) {
	(void)sp;
    return ++bufferRegisterCount;
}

int OMJucePlayer::unregisterBuffer(AudioSourcePlayer *sp) {
	(void)sp;
    return --bufferRegisterCount;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


/*
 int OMJucePlayer::setActiveOutputChannels(int n, int *mask) {
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
 OMJucePlayer::OMJucePlayer(int nIn, int nOut)
 {
 std::cout << std::endl << "ALLOCATING AUDIO PLAYER (with " << nIn << "x" << nOut << " channels)" << std::endl;
 initialise(nIn,nOut,0,true);
 outputChannelsRouting = new std::vector<int>(nOut,-1);
 }
 */

 /*
 OMJucePlayer::OMJucePlayer(String inputDeviceName, String outputDeviceName, int nIn, int nOut, double sr) {

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
