/************************************************************************************/
/*!
 *  @file       OMJuceAudioLib.cpp
 *  @brief      OMJuceAudioLib interface implementation
 *  @author     Dimitri Bouche
 *  @date       04/11/2016
 *
 */
/************************************************************************************/
#include "OMAudioLib.hpp"

#include "OMJucePlayer.hpp"
#include "OMAudioBuffer.hpp"
#include "OMAudioFile.hpp"

//================//
//  PLAYER
//================//

void* openAudioManager () {
    return static_cast<void*>(new OMJucePlayer());
}

void closeAudioManager (void* player) {
    delete static_cast<OMJucePlayer*>(player);
}

int getDevicesTypeCount (void* player) {
    return static_cast<OMJucePlayer*>(player)->getDevicesTypeCount();
}

const char* getDeviceTypeName(void* player, int i) {
    return static_cast<OMJucePlayer*>(player)->getDeviceTypeName(i);
}

void setDeviceType(void* player, const char* type) {
    return static_cast<OMJucePlayer*>(player)->setDeviceType(static_cast<String>(type));
}

const char* getCurrentDeviceType(void* player){
    return static_cast<OMJucePlayer*>(player)->getCurrentDeviceType();
}



int getInputDevicesCountForType (void* player, int device_type_num) {
    return static_cast<OMJucePlayer*>(player)->getInputDevicesCountForType(device_type_num);
}

int getOutputDevicesCountForType (void* player, int device_type_num) {
    return static_cast<OMJucePlayer*>(player)->getOutputDevicesCountForType(device_type_num);
}

const char* getNthInputDeviceName(void* player, int device_type_num, int device_num){
    return static_cast<OMJucePlayer*>(player)->getNthInputDeviceName(device_type_num, device_num);
}

const char* getNthOutputDeviceName(void* player, int device_type_num, int device_num){
    return static_cast<OMJucePlayer*>(player)->getNthOutputDeviceName(device_type_num, device_num);
}

int getInputDevicesCount (void* player) {
    return static_cast<OMJucePlayer*>(player)->getInputDevicesCount();
}

int getOutputDevicesCount (void* player) {
    return static_cast<OMJucePlayer*>(player)->getOutputDevicesCount();
}

int setOutputChannelsMapping(void* player, int n, int *map){
    return static_cast<OMJucePlayer*>(player)->setOutputChannelsMapping(n, map);
}

//////////

int getInputChannelsCount (void* player) {
    return static_cast<OMJucePlayer*>(player)->getInputChannelsCount();
}

int getOutputChannelsCount (void* player) {
    return static_cast<OMJucePlayer*>(player)->getOutputChannelsCount();
}

int getAvailableSampleRatesCount (void* player) {
    return static_cast<OMJucePlayer*>(player)->getAvailableSampleRatesCount();
}

int getNthAvailableSampleRate (void* player, int n) {
    return static_cast<OMJucePlayer*>(player)->getNthAvailableSampleRate(n);
}

int getCurrentSampleRate (void* player) {
    return static_cast<OMJucePlayer*>(player)->getCurrentSampleRate();
}

int setSampleRate (void* player, int sr) {
    return static_cast<OMJucePlayer*>(player)->setSampleRate(sr);
}


int getAvailableBufferSizesCount (void* player) {
    return static_cast<OMJucePlayer*>(player)->getAvailableBufferSizesCount();
}

int getNthAvailableBufferSize (void* player, int n) {
    return static_cast<OMJucePlayer*>(player)->getNthAvailableBufferSize(n);
}

int getDefaultBufferSize (void* player) {
    return static_cast<OMJucePlayer*>(player)->getDefaultBufferSize();
}

int getCurrentBufferSize (void* player) {
    return static_cast<OMJucePlayer*>(player)->getCurrentBufferSize();
}

int setBufferSize (void* player, int size) {
    return static_cast<OMJucePlayer*>(player)->setBufferSize(size);
}

void setAudioDevice (void* player, int inputDevice, int outputDevice, int ninputs, int noutputs, int samplerate, int buffer_size) {
    static_cast<OMJucePlayer*>(player)->audioSetup(inputDevice, outputDevice, ninputs, noutputs, static_cast<double>(samplerate), buffer_size);
}


//================//
//  BUFFERS
//================//

void* MakeDataReader (float** audio_buffer, int numChannels, int numSamples, int sampleRate) {
    return new OMAudioBuffer(audio_buffer, numChannels, numSamples, sampleRate);
}

void* MakeFileReader (const char* path) {
    return new OMAudioFile(path);
}

void FreeReader (void* reader) {
    delete static_cast<OMSoundHandler*>(reader);
}

void StartReader (void* player, void* reader) {
    OMSoundHandler* handler = static_cast<OMSoundHandler*>(reader);
    OMJucePlayer* master_player = static_cast<OMJucePlayer*>(player);
    return handler->playOnPlayer(master_player);
}

void StopReader (void* player, void* reader) {
    OMSoundHandler* handler = static_cast<OMSoundHandler*>(reader);
    OMJucePlayer* master_player = static_cast<OMJucePlayer*>(player);
    return handler->stopOnPlayer(master_player);
}

void PauseReader (void* player, void* reader) {
    OMSoundHandler* handler = static_cast<OMSoundHandler*>(reader);
    OMJucePlayer* master_player = static_cast<OMJucePlayer*>(player);
    return handler->pauseOnPlayer(master_player);
}


void SetPosReader (void* reader, long long pos) {
    OMSoundHandler* handler = static_cast<OMSoundHandler*>(reader);
    return handler->setPlayheadPos(pos);
}

long long GetPosReader (void* reader) {
    OMSoundHandler* handler = static_cast<OMSoundHandler*>(reader);
    return handler->getPlayheadPos();
}

float GetGainReader(void* reader){
    OMSoundHandler* handler = static_cast<OMSoundHandler*>(reader);
    return handler->getGain();
}

void SetGainReader (void* reader, float gain){
    OMSoundHandler* handler = static_cast<OMSoundHandler*>(reader);
    handler->setGain(gain);
}


//void LoopReader (void* reader, bool looper) {
//    static_cast<OMJuceBuffer*>(static_cast<OMJuceHandler*>(buffer)->internalpointer)->setLooping(looper);
//}






