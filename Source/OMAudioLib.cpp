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

/// dirty hack : it seems that it's not good to pass const char * to Lisp,
/// so we rather write all strings into this static variable.
/// really not nice, so be careful
static juce::String str;

//================//
//  PLAYER
//================//

static OMJucePlayer & getOMJucePlayer(void * player)
{
    OMJucePlayer * p = static_cast< OMJucePlayer* >( player );
    assert( p != nullptr );
    return *p;
}

static OMSoundHandler & getOMSoundHandler(void * reader)
{
    OMSoundHandler * handler = static_cast< OMSoundHandler * >( reader );
    assert( handler != nullptr );
    return *handler;
}

void* openAudioManager () {
    return static_cast<void*>(new OMJucePlayer());
}

void closeAudioManager (void* player) {
    delete static_cast<OMJucePlayer*>(player);
}

////////////////
// CHANNELS
////////////////

void initializeAudioChannels(void* player, int ninputs, int noutputs) {
	getOMJucePlayer( player ).initializeAudioChannels(ninputs, noutputs);
}

int getInputChannelsCount(void* player) {
	return getOMJucePlayer( player ).getInputChannelsCount();
}

int getOutputChannelsCount(void* player) {
	return getOMJucePlayer( player ).getOutputChannelsCount();
}

int setOutputChannelsMapping(void* player, int n, int *map) {
	return getOMJucePlayer( player ).setOutputChannelsMapping(n, map);
}

////////////////////////////////
// DEVICE TYPES (AKA DRIVERS)
////////////////////////////////

int getDevicesTypeCount (void* player) {
    return getOMJucePlayer( player ).getDevicesTypeCount();
}

const char* getDeviceTypeName(void* player, int i) {
    str =  getOMJucePlayer( player ).getDeviceTypeName(i);
    return str.toUTF8();
}

void setDeviceType(void* player, const char* type) {
    return getOMJucePlayer( player ).setDeviceType( String(type) );
}

const char* getCurrentDeviceType(void* player){
    str =  getOMJucePlayer( player ).getCurrentDeviceType();
    return str.toUTF8();
}

const char* getCurrentDeviceName(void* player) {
	str = getOMJucePlayer( player ).getCurrentDeviceName();
    return str.toUTF8();
}

////////////////////
// IN/OUT DEVICES
////////////////////

int getInputDevicesCount(void* player) {
	return getOMJucePlayer( player ).getInputDevicesCount();
}

int getOutputDevicesCount(void* player) {
	return getOMJucePlayer( player ).getOutputDevicesCount();
}

int getInputDevicesCountForType (void* player, int device_type_num) {
    return getOMJucePlayer( player ).getInputDevicesCountForType(device_type_num);
}

int getOutputDevicesCountForType (void* player, int device_type_num) {
    return getOMJucePlayer( player ).getOutputDevicesCountForType(device_type_num);
}

const char* getNthInputDeviceName(void* player, int device_type_num, int device_num){
    str = getOMJucePlayer( player ).getNthInputDeviceName(device_type_num, device_num);
    return str.toUTF8();
}

const char* getNthOutputDeviceName(void* player, int device_type_num, int device_num){
    str = getOMJucePlayer( player ).getNthOutputDeviceName(device_type_num, device_num);
    return str.toUTF8();
}


int setInputDevice(void* player, int deviceNum) {
	return getOMJucePlayer( player ).setInputDevice(deviceNum);
}

int setOutputDevice(void* player, int deviceNum){
	return getOMJucePlayer( player ).setOutputDevice(deviceNum);
}


////////////////
// SAMPLE RATE
////////////////

int getAvailableSampleRatesCount (void* player) {
    return getOMJucePlayer( player ).getAvailableSampleRatesCount();
}

int getNthAvailableSampleRate (void* player, int n) {
    return getOMJucePlayer( player ).getNthAvailableSampleRate(n);
}

int getCurrentSampleRate (void* player) {
    return getOMJucePlayer( player ).getCurrentSampleRate();
}

int setSampleRate (void* player, int sr) {
    return getOMJucePlayer( player ).setSampleRate(sr);
}

////////////////
// BUFFER SIZES
////////////////

int getAvailableBufferSizesCount (void* player) {
    return getOMJucePlayer( player ).getAvailableBufferSizesCount();
}

int getNthAvailableBufferSize (void* player, int n) {
    return getOMJucePlayer( player ).getNthAvailableBufferSize(n);
}

int getDefaultBufferSize (void* player) {
    return getOMJucePlayer( player ).getDefaultBufferSize();
}

int getCurrentBufferSize (void* player) {
    return getOMJucePlayer( player ).getCurrentBufferSize();
}

int setBufferSize (void* player, int size) {
    return getOMJucePlayer( player ).setBufferSize(size);
}


////////////////////
// MISC
////////////////////

void setAudioDevice (void* player, int inputDevice, int outputDevice, int ninputs, int noutputs, int samplerate, int buffer_size) {
    getOMJucePlayer( player ).audioSetup( inputDevice, outputDevice, ninputs, noutputs, static_cast<double>(samplerate), buffer_size );
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
    OMSoundHandler & handler = getOMSoundHandler( reader );
    OMJucePlayer* master_player = static_cast<OMJucePlayer*>(player);
    return handler.playOnPlayer( *master_player );
}

void StopReader (void* player, void* reader) {
    OMSoundHandler & handler = getOMSoundHandler( reader );
    OMJucePlayer* master_player = static_cast<OMJucePlayer*>(player);
    return handler.stopOnPlayer( *master_player );
}

void PauseReader (void* player, void* reader) {
    OMSoundHandler & handler = getOMSoundHandler( reader );
    OMJucePlayer* master_player = static_cast<OMJucePlayer*>(player);
    return handler.pauseOnPlayer( *master_player );
}

long long GetPosReader(void* reader) {
	OMSoundHandler & handler = getOMSoundHandler( reader );
	return (long long) handler.getPlayheadPos();
}

void SetPosReader (void* reader, long long pos) {
    OMSoundHandler & handler = getOMSoundHandler( reader );
    return handler.setPlayheadPos(pos);
}

float GetGainReader(void* reader){
    OMSoundHandler & handler = getOMSoundHandler( reader );
    return handler.getGain();
}

void SetGainReader (void* reader, float gain){
    OMSoundHandler & handler = getOMSoundHandler( reader );
    handler.setGain( gain );
}







