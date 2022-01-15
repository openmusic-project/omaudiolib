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
template <typename T>
T& CastPointer(void* pointer)
{
  auto p = static_cast<T*>(pointer);

  assert(p != nullptr);

  return *p;
}
}


void* openAudioManager()
{
  return new Player();
}


void closeAudioManager(void* player)
{
  delete static_cast<Player*>(player);
}


void initializeAudioChannels(void* player, int ninputs, int noutputs)
{
  CastPointer<Player>(player).initializeAudioChannels(ninputs, noutputs);
}


int getInputChannelsCount(void* player)
{
  return CastPointer<Player>(player).getInputChannelsCount();
}


int getOutputChannelsCount(void* player)
{
  return CastPointer<Player>(player).getOutputChannelsCount();
}


int setOutputChannelsMapping(void* player, int n, int* map)
{
  return CastPointer<Player>(player).setOutputChannelsMapping(n, map);
}


int getDevicesTypeCount(void* player)
{
  return CastPointer<Player>(player).getDevicesTypeCount();
}


const char* getDeviceTypeName(void* player, int i)
{
  return CastPointer<Player>(player).getDeviceTypeName(i).toUTF8();
}


void setDeviceType(void* player, const char* type)
{
  return CastPointer<Player>(player).setDeviceType(type);
}


const char* getCurrentDeviceType(void* player)
{
  return CastPointer<Player>(player).getCurrentDeviceType().toUTF8();
}


const char* getCurrentDeviceName(void* player)
{
  return CastPointer<Player>(player).getCurrentDeviceName().toUTF8();
}


int getInputDevicesCount(void* player)
{
  return CastPointer<Player>(player).getInputDevicesCount();
}


int getOutputDevicesCount(void* player)
{
  return CastPointer<Player>(player).getOutputDevicesCount();
}


int getInputDevicesCountForType(void* player, int device_type_num)
{
  return CastPointer<Player>(player)
           .getInputDevicesCountForType(device_type_num);
}


int getOutputDevicesCountForType(void* player, int device_type_num)
{
  return CastPointer<Player>(player)
           .getOutputDevicesCountForType(device_type_num);
}


const char* getNthInputDeviceName(void* player,
                                  int device_type_num,
                                  int device_num)
{
  return CastPointer<Player>(player)
           .getNthInputDeviceName(device_type_num, device_num).toUTF8();
}

const char* getNthOutputDeviceName(void* player,
                                   int device_type_num,
                                   int device_num)
{
  return CastPointer<Player>(player)
           .getNthOutputDeviceName(device_type_num, device_num).toUTF8();
}


int setInputDevice(void* player, int deviceNum)
{
  return CastPointer<Player>(player).setInputDevice(deviceNum);
}


int setOutputDevice(void* player, int deviceNum)
{
  return CastPointer<Player>(player).setOutputDevice(deviceNum);
}


int getAvailableSampleRatesCount(void* player)
{
  return CastPointer<Player>(player).getAvailableSampleRatesCount();
}


int getNthAvailableSampleRate(void* player, int n)
{
  return CastPointer<Player>(player).getNthAvailableSampleRate(n);
}


int getCurrentSampleRate(void* player)
{
  return CastPointer<Player>(player).getCurrentSampleRate();
}


int setSampleRate(void* player, int sr)
{
  return CastPointer<Player>(player).setSampleRate(sr);
}


int getAvailableBufferSizesCount(void* player)
{
  return CastPointer<Player>(player).getAvailableBufferSizesCount();
}


int getNthAvailableBufferSize(void* player, int n)
{
  return CastPointer<Player>(player).getNthAvailableBufferSize(n);
}


int getDefaultBufferSize(void* player)
{
  return CastPointer<Player>(player).getDefaultBufferSize();
}


int getCurrentBufferSize(void* player)
{
  return CastPointer<Player>(player).getCurrentBufferSize();
}


int setBufferSize(void* player, int size)
{
  return CastPointer<Player>(player).setBufferSize(size);
}


void setAudioDevice(void* player,
                    int inputDevice,
                    int outputDevice,
                    int ninputs,
                    int noutputs,
                    int samplerate,
                    int buffer_size)
{
  CastPointer<Player>(player).audioSetup(inputDevice,
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
  return CastPointer<SourceHandler>(source)
           .playOnPlayer(CastPointer<Player>(player));
}


void stopAudioSource(void* player, void* source)
{
  return CastPointer<SourceHandler>(source)
           .stopOnPlayer(CastPointer<Player>(player));
}


void pauseAudioSource(void* player, void* source)
{
  return CastPointer<SourceHandler>(source)
           .pauseOnPlayer(CastPointer<Player>(player));
}


long long getAudioSourcePos(void* source)
{
  return (long long) CastPointer<SourceHandler>(source).getPlayheadPos();
}


void setAudioSourcePos(void* source, long long pos)
{
  return CastPointer<SourceHandler>(source).setPlayheadPos(pos);
}


float getAudioSourceGain(void* source)
{
  return CastPointer<SourceHandler>(source).getGain();
}


void setAudioSourceGain(void* source, float gain)
{
  CastPointer<SourceHandler>(source).setGain(gain);
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
  return CastPointer<AudioFileReader>(filereader).getNumChannels();
}


long long getAudioFileNumSamples(void* filereader)
{
  return CastPointer<AudioFileReader>(filereader).getNumSamples();
}


double getAudioFileSampleRate(void* filereader)
{
  return CastPointer<AudioFileReader>(filereader).getSampleRate();
}


int getAudioFileSampleSize(void* filereader)
{
  return CastPointer<AudioFileReader>(filereader).getSampleSize();
}


bool getAudioFileFloatFormat(void* filereader)
{
  return CastPointer<AudioFileReader>(filereader).usesFloatSamples();
}


const char* getAudioFileFormat(void* filereader)
{
  return CastPointer<AudioFileReader>(filereader).getFileFormat().toUTF8();
}


bool getAudioFileSamples(void* filereader,
                         float** buffer,
                         long long startPos,
                         int n_samples)
{
  return CastPointer<AudioFileReader>(filereader).getSamples(buffer,
                                                             startPos,
                                                             n_samples);
}


void* makeAudioFileWriter(const char* path, int format)
{
  return new AudioFileWriter(CharPointer_UTF8(path),
                             static_cast<audio_format_t>(format));
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
  return CastPointer<AudioFileWriter>(filewriter)
           .writeSamplesToFile(buffer,
                               n_channels,
                               size,
                               sr,
                               ss);
}
