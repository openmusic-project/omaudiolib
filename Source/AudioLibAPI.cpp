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


#include "AudioLibAPI.hpp"

#include "OMPlayer.hpp"
#include "OMAudioBufferSource.hpp"
#include "OMAudioFileSource.hpp"
#include "OMAudioFileReader.hpp"
#include "OMAudioFileWriter.hpp"

#include <assert.h>

// dirty hack : it seems that it's not good to pass const char * to Lisp,
// so we rather write all strings into this static variable.
// really not nice, so be careful
static juce::String str;

/* CAST-ACCESSORS */

static OMPlayer & getOMPlayer(void * player)
{
    OMPlayer * p = static_cast< OMPlayer* >( player );
    assert( p != nullptr );
    return *p;
}

static OMSourceHandler & getOMSourceHandler(void * reader)
{
    OMSourceHandler * handler = static_cast< OMSourceHandler * >( reader );
    assert( handler != nullptr );
    return *handler;
}

static OMAudioFileReader & getOMAudioFileReader(void * pointer)
{
    OMAudioFileReader * reader = static_cast< OMAudioFileReader* >( pointer );
    assert( reader != nullptr );
    return *reader;
}

static OMAudioFileWriter & getOMAudioFileWriter(void * pointer)
{
    OMAudioFileWriter * writer = static_cast< OMAudioFileWriter* >( pointer );
    assert( writer != nullptr );
    return *writer;
}

/******************/
//  PLAYER
/******************/

void* openAudioManager () {
    return static_cast<void*>(new OMPlayer());
}

void closeAudioManager (void* player) {
    delete static_cast<OMPlayer*>(player);
}

/******************/
// CHANNELS
/******************/

void initializeAudioChannels(void* player, int ninputs, int noutputs) {
	getOMPlayer( player ).initializeAudioChannels(ninputs, noutputs);
}

int getInputChannelsCount(void* player) {
	return getOMPlayer( player ).getInputChannelsCount();
}

int getOutputChannelsCount(void* player) {
	return getOMPlayer( player ).getOutputChannelsCount();
}

int setOutputChannelsMapping(void* player, int n, int *map) {
	return getOMPlayer( player ).setOutputChannelsMapping(n, map);
}

/*****************************/
// DEVICE TYPES (AKA DRIVERS)
/*****************************/

int getDevicesTypeCount (void* player) {
    return getOMPlayer( player ).getDevicesTypeCount();
}

const char* getDeviceTypeName(void* player, int i) {
    str =  getOMPlayer( player ).getDeviceTypeName(i);
    return str.toUTF8();
}

void setDeviceType(void* player, const char* type) {
    return getOMPlayer( player ).setDeviceType( String(type) );
}

const char* getCurrentDeviceType(void* player){
    str =  getOMPlayer( player ).getCurrentDeviceType();
    return str.toUTF8();
}

const char* getCurrentDeviceName(void* player) {
	str = getOMPlayer( player ).getCurrentDeviceName();
    return str.toUTF8();
}

/******************/
// IN/OUT DEVICES
/******************/

int getInputDevicesCount(void* player) {
	return getOMPlayer( player ).getInputDevicesCount();
}

int getOutputDevicesCount(void* player) {
	return getOMPlayer( player ).getOutputDevicesCount();
}

int getInputDevicesCountForType (void* player, int device_type_num) {
    return getOMPlayer( player ).getInputDevicesCountForType(device_type_num);
}

int getOutputDevicesCountForType (void* player, int device_type_num) {
    return getOMPlayer( player ).getOutputDevicesCountForType(device_type_num);
}

const char* getNthInputDeviceName(void* player, int device_type_num, int device_num){
    str = getOMPlayer( player ).getNthInputDeviceName(device_type_num, device_num);
    return str.toUTF8();
}

const char* getNthOutputDeviceName(void* player, int device_type_num, int device_num){
    str = getOMPlayer( player ).getNthOutputDeviceName(device_type_num, device_num);
    return str.toUTF8();
}


int setInputDevice(void* player, int deviceNum) {
	return getOMPlayer( player ).setInputDevice(deviceNum);
}

int setOutputDevice(void* player, int deviceNum){
	return getOMPlayer( player ).setOutputDevice(deviceNum);
}


/******************/
// SAMPLE RATE
/******************/

int getAvailableSampleRatesCount (void* player) {
    return getOMPlayer( player ).getAvailableSampleRatesCount();
}

int getNthAvailableSampleRate (void* player, int n) {
    return getOMPlayer( player ).getNthAvailableSampleRate(n);
}

int getCurrentSampleRate (void* player) {
    return getOMPlayer( player ).getCurrentSampleRate();
}

int setSampleRate (void* player, int sr) {
    return getOMPlayer( player ).setSampleRate(sr);
}

/******************/
// BUFFER SIZES
/******************/

int getAvailableBufferSizesCount (void* player) {
    return getOMPlayer( player ).getAvailableBufferSizesCount();
}

int getNthAvailableBufferSize (void* player, int n) {
    return getOMPlayer( player ).getNthAvailableBufferSize(n);
}

int getDefaultBufferSize (void* player) {
    return getOMPlayer( player ).getDefaultBufferSize();
}

int getCurrentBufferSize (void* player) {
    return getOMPlayer( player ).getCurrentBufferSize();
}

int setBufferSize (void* player, int size) {
    return getOMPlayer( player ).setBufferSize(size);
}


/******************/
// MISC
/******************/

void setAudioDevice (void* player, int inputDevice, int outputDevice, int ninputs, int noutputs, int samplerate, int buffer_size) {
    getOMPlayer( player ).audioSetup( inputDevice, outputDevice, ninputs, noutputs, static_cast<double>(samplerate), buffer_size );
}


/******************/
//  PLAYER
/******************/

void* makeAudioSourceFromBuffer (float** audio_buffer, int numChannels, int numSamples, int sampleRate) {
    return new OMAudioBufferSource(audio_buffer, numChannels, numSamples, sampleRate);
}

void* makeAudioSourceFromFile (const char* path) {
    return new OMAudioFileSource( String( CharPointer_UTF8(path) ) );
}

void freeAudioSource (void* source) {
    delete static_cast<OMSourceHandler*>(source);
}

void startAudioSource (void* player, void* source) {
    OMSourceHandler & handler = getOMSourceHandler( source );
    OMPlayer* master_player = static_cast<OMPlayer*>(player);
    return handler.playOnPlayer( *master_player );
}

void stopAudioSource (void* player, void* source) {
    OMSourceHandler & handler = getOMSourceHandler( source );
    OMPlayer* master_player = static_cast<OMPlayer*>(player);
    return handler.stopOnPlayer( *master_player );
}

void pauseAudioSource (void* player, void* source) {
    OMSourceHandler & handler = getOMSourceHandler( source );
    OMPlayer* master_player = static_cast<OMPlayer*>(player);
    return handler.pauseOnPlayer( *master_player );
}

long long getAudioSourcePos(void* source) {
	OMSourceHandler & handler = getOMSourceHandler( source );
	return (long long) handler.getPlayheadPos();
}

void setAudioSourcePos (void* source, long long pos) {
    OMSourceHandler & handler = getOMSourceHandler( source );
    return handler.setPlayheadPos(pos);
}

float getAudioSourceGain(void* source){
    OMSourceHandler & handler = getOMSourceHandler( source );
    return handler.getGain();
}

void setAudioSourceGain (void* source, float gain){
    OMSourceHandler & handler = getOMSourceHandler( source );
    handler.setGain( gain );
}


//================//
//  FILE I/O
//================//

void* makeAudioFileReader (const char* path) {
    OMAudioFileReader *r = new OMAudioFileReader( String( CharPointer_UTF8(path) ) );
    if (r->isValid()) return r ;
    else return NULL ;
}

void freeAudioFileReader (void* filereader) {
    delete static_cast<OMAudioFileReader*> (filereader);
}

int getAudioFileNumChannels (void* filereader) {
    OMAudioFileReader & r = getOMAudioFileReader( filereader );
    return r.getNumChannels();
}

long long getAudioFileNumSamples (void* filereader) {
    OMAudioFileReader & r = getOMAudioFileReader( filereader );
    return r.getNumSamples ();
}

double getAudioFileSampleRate (void* filereader) {
    OMAudioFileReader & r = getOMAudioFileReader( filereader );
    return r.getSampleRate();
}

int getAudioFileSampleSize (void* filereader) {
    OMAudioFileReader & r = getOMAudioFileReader( filereader );
    return r.getSampleSize();
}

bool getAudioFileFloatFormat (void* filereader) {
    OMAudioFileReader & r = getOMAudioFileReader( filereader );
    return r.usesFloatSamples();
}

const char* getAudioFileFormat (void* filereader) {
    OMAudioFileReader & r = getOMAudioFileReader( filereader );
    str = r.getFileFormat();
    return str.toUTF8();
}

bool getAudioFileSamples (void* filereader, float** buffer, long long startPos, int n_samples) {
    OMAudioFileReader & handler = getOMAudioFileReader( filereader );
    return handler.getSamples( buffer, startPos, n_samples );
}



void* makeAudioFileWriter (const char* path, int format) {
    return new OMAudioFileWriter( String( CharPointer_UTF8(path) ), (audio_format_t) format );
}

void freeAudioFileWriter (void* filewriter) {
    delete static_cast<OMAudioFileWriter*> (filewriter);
}

bool writeSamplesToAudioFile (void* filewriter, float** buffer, int n_channels, long long size, double sr, int ss) {
    OMAudioFileWriter & handler = getOMAudioFileWriter( filewriter );
    return handler.writeSamplesToFile( buffer, n_channels, size, sr, ss );
}



