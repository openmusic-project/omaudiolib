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

#include "API.h"

#include "Player.h"
#include "AudioBufferSource.h"
#include "AudioFileSource.h"
#include "AudioFileReader.h"
#include "AudioFileWriter.h"

#include <assert.h>


namespace
{
Player& getPlayer(void* player)
{
  Player* p = static_cast<Player*>(player);

  assert(p != nullptr);

  return *p;
}


SourceHandler& getSourceHandler(void* reader)
{
  SourceHandler* handler = static_cast<SourceHandler*>(reader);

  assert(handler != nullptr);

  return *handler;
}


AudioFileReader& getAudioFileReader(void* pointer)
{
  AudioFileReader* reader = static_cast<AudioFileReader*>(pointer);

  assert(reader != nullptr);

  return *reader;
}


AudioFileWriter& getAudioFileWriter(void* pointer)
{
  AudioFileWriter* writer = static_cast<AudioFileWriter*>(pointer);

  assert(writer != nullptr);

  return *writer;
}
} // namespace


void* openAudioManager()
{
  return static_cast<void*>(new Player());
}


void closeAudioManager(void* player)
{
  delete static_cast<Player*>(player);
}


void initializeAudioChannels(void* player, int ninputs, int noutputs)
{
  getPlayer(player).initializeAudioChannels(ninputs, noutputs);
}


int getInputChannelsCount(void* player)
{
  return getPlayer(player).getInputChannelsCount();
}


int getOutputChannelsCount(void* player)
{
  return getPlayer(player).getOutputChannelsCount();
}


int setOutputChannelsMapping(void* player, int n, int* map)
{
  return getPlayer(player).setOutputChannelsMapping(n, map);
}


int getDevicesTypeCount(void* player)
{
  return getPlayer(player).getDevicesTypeCount();
}


const char* getDeviceTypeName(void* player, int i)
{
  return getPlayer(player).getDeviceTypeName(i).toUTF8();
}


void setDeviceType(void* player, const char* type)
{
  return getPlayer(player).setDeviceType(type);
}


const char* getCurrentDeviceType(void* player)
{
  return getPlayer(player).getCurrentDeviceType().toUTF8();
}


const char* getCurrentDeviceName(void* player)
{
  return getPlayer(player).getCurrentDeviceName().toUTF8();
}


int getInputDevicesCount(void* player)
{
  return getPlayer(player).getInputDevicesCount();
}


int getOutputDevicesCount(void* player)
{
  return getPlayer(player).getOutputDevicesCount();
}


int getInputDevicesCountForType(void* player, int device_type_num)
{
  return getPlayer(player).getInputDevicesCountForType(device_type_num);
}


int getOutputDevicesCountForType(void* player, int device_type_num)
{
  return getPlayer(player).getOutputDevicesCountForType(device_type_num);
}


const char* getNthInputDeviceName(void* player,
                                  int device_type_num,
                                  int device_num)
{
  return getPlayer(player).getNthInputDeviceName(device_type_num, device_num).toUTF8();
}

const char* getNthOutputDeviceName(void* player,
                                   int device_type_num,
                                   int device_num)
{
  return getPlayer(player).getNthOutputDeviceName(device_type_num, device_num).toUTF8();
}


int setInputDevice(void* player, int deviceNum)
{
  return getPlayer(player).setInputDevice(deviceNum);
}


int setOutputDevice(void* player, int deviceNum)
{
  return getPlayer(player).setOutputDevice(deviceNum);
}


int getAvailableSampleRatesCount(void* player)
{
  return getPlayer(player).getAvailableSampleRatesCount();
}


int getNthAvailableSampleRate(void* player, int n)
{
  return getPlayer(player).getNthAvailableSampleRate(n);
}


int getCurrentSampleRate(void* player)
{
  return getPlayer(player).getCurrentSampleRate();
}


int setSampleRate(void* player, int sr)
{
  return getPlayer(player).setSampleRate(sr);
}


int getAvailableBufferSizesCount(void* player)
{
  return getPlayer(player).getAvailableBufferSizesCount();
}


int getNthAvailableBufferSize(void* player, int n)
{
  return getPlayer(player).getNthAvailableBufferSize(n);
}


int getDefaultBufferSize(void* player)
{
  return getPlayer(player).getDefaultBufferSize();
}


int getCurrentBufferSize(void* player)
{
  return getPlayer(player).getCurrentBufferSize();
}


int setBufferSize(void* player, int size)
{
  return getPlayer(player).setBufferSize(size);
}


void setAudioDevice(void* player,
                    int inputDevice,
                    int outputDevice,
                    int ninputs,
                    int noutputs,
                    int samplerate,
                    int buffer_size)
{
  getPlayer(player).audioSetup(inputDevice,
                               outputDevice,
                               ninputs,
                               noutputs,
                               static_cast<double>(samplerate),
                               buffer_size);
}


void* makeAudioSourceFromBuffer(float** audio_buffer,
                                int numChannels,
                                int numSamples,
                                int sampleRate)
{
  return new AudioBufferSource(audio_buffer,
                               numChannels,
                               numSamples,
                               sampleRate);
}


void* makeAudioSourceFromFile(const char* path)
{
  return new AudioFileSource(CharPointer_UTF8(path));
}


void freeAudioSource(void* source)
{
  delete static_cast<SourceHandler*>(source);
}


void startAudioSource(void* player, void* source)
{
  Player* master_player = static_cast<Player*>(player);

  return getSourceHandler(source).playOnPlayer(*master_player);
}


void stopAudioSource(void* player, void* source)
{
  Player* master_player = static_cast<Player*>(player);

  return getSourceHandler(source).stopOnPlayer(*master_player);
}


void pauseAudioSource(void* player, void* source)
{
  Player* master_player = static_cast<Player*>(player);

  return getSourceHandler(source).pauseOnPlayer(*master_player);
}


long long getAudioSourcePos(void* source)
{
  return (long long) getSourceHandler(source).getPlayheadPos();
}


void setAudioSourcePos(void* source, long long pos)
{
  return getSourceHandler(source).setPlayheadPos(pos);
}


float getAudioSourceGain(void* source)
{
  return getSourceHandler(source).getGain();
}


void setAudioSourceGain(void* source, float gain)
{
  getSourceHandler(source).setGain(gain);
}


void* makeAudioFileReader(const char* path)
{
  AudioFileReader *r = new AudioFileReader(CharPointer_UTF8(path));

  if (r->isValid())
  {
    return r ;
  }
  else
  {
    return NULL ;
  }
}


void freeAudioFileReader(void* filereader)
{
  delete static_cast<AudioFileReader*>(filereader);
}


int getAudioFileNumChannels(void* filereader)
{
  return getAudioFileReader(filereader).getNumChannels();
}


long long getAudioFileNumSamples(void* filereader)
{
  return getAudioFileReader(filereader).getNumSamples();
}


double getAudioFileSampleRate(void* filereader)
{
  return getAudioFileReader(filereader).getSampleRate();
}


int getAudioFileSampleSize(void* filereader)
{
  return getAudioFileReader(filereader).getSampleSize();
}


bool getAudioFileFloatFormat(void* filereader)
{
  return getAudioFileReader(filereader).usesFloatSamples();
}


const char* getAudioFileFormat(void* filereader)
{
  return getAudioFileReader(filereader).getFileFormat().toUTF8();
}


bool getAudioFileSamples(void* filereader,
                         float** buffer,
                         long long startPos,
                         int n_samples)
{
  return getAudioFileReader(filereader).getSamples(buffer, startPos, n_samples);
}


void* makeAudioFileWriter(const char* path, int format)
{
  return new AudioFileWriter(CharPointer_UTF8(path), (audio_format_t) format);
}


void freeAudioFileWriter (void* filewriter)
{
  delete static_cast<AudioFileWriter*>(filewriter);
}


bool writeSamplesToAudioFile(void* filewriter,
                             float** buffer,
                             int n_channels,
                             long long size,
                             double sr,
                             int ss)
{
  return getAudioFileWriter(filewriter).writeSamplesToFile(buffer,
                                                           n_channels,
                                                           size,
                                                           sr,
                                                           ss);
}
