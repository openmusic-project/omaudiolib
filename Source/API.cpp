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


void initializeAudioChannels(void* player, int n_inputs, int n_outputs)
{
  CastPointer<Player>(player).initializeAudioChannels(n_inputs, n_outputs);
}


int getInputChannelsCount(void* player)
{
  return CastPointer<Player>(player).getInputChannelsCount();
}


int getOutputChannelsCount(void* player)
{
  return CastPointer<Player>(player).getOutputChannelsCount();
}


int setOutputChannelsMapping(void* player, int n_channels, int* map)
{
  return CastPointer<Player>(player).setOutputChannelsMapping(n_channels, map);
}


int getDevicesTypeCount(void* player)
{
  return CastPointer<Player>(player).getDevicesTypeCount();
}


const char* getDeviceTypeName(void* player, int device_type_index)
{
  return CastPointer<Player>(player).getDeviceTypeName(device_type_index).toUTF8();
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


int getInputDevicesCountForType(void* player, int device_type_index)
{
  return CastPointer<Player>(player)
           .getInputDevicesCountForType(device_type_index);
}


int getOutputDevicesCountForType(void* player, int device_type_index)
{
  return CastPointer<Player>(player)
           .getOutputDevicesCountForType(device_type_index);
}


const char* getNthInputDeviceName(void* player,
                                  int device_type_index,
                                  int n)
{
  return CastPointer<Player>(player)
           .getNthInputDeviceName(device_type_index, n).toUTF8();
}

const char* getNthOutputDeviceName(void* player,
                                   int device_type_index,
                                   int n)
{
  return CastPointer<Player>(player)
           .getNthOutputDeviceName(device_type_index, n).toUTF8();
}


int setInputDevice(void* player, int device_index)
{
  return CastPointer<Player>(player).setInputDevice(device_index);
}


int setOutputDevice(void* player, int device_index)
{
  return CastPointer<Player>(player).setOutputDevice(device_index);
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


int setSampleRate(void* player, int sample_rate)
{
  return CastPointer<Player>(player).setSampleRate(sample_rate);
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


void setupAudioDevice(void* player,
                      int n_inputs,
                      int n_outputs,
                      int sample_rate,
                      int buffer_size)
{
  CastPointer<Player>(player).audioSetup(n_inputs,
                                         n_outputs,
                                         static_cast<double>(sample_rate),
                                         buffer_size);
}


void* makeAudioSourceFromBuffer(float** audio_buffer,
                                int num_channels,
                                int num_samples,
                                int sample_rate)
{
  return new AudioBufferSource(audio_buffer,
                               num_channels,
                               num_samples,
                               sample_rate);
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
  auto r = new AudioFileReader(CharPointer_UTF8(path));

  return r->isValid() ? r : nullptr;
}


void freeAudioFileReader(void* file_reader)
{
  delete static_cast<AudioFileReader*>(file_reader);
}


int getAudioFileNumChannels(void* file_reader)
{
  return CastPointer<AudioFileReader>(file_reader).getNumChannels();
}


long long getAudioFileNumSamples(void* file_reader)
{
  return CastPointer<AudioFileReader>(file_reader).getNumSamples();
}


double getAudioFileSampleRate(void* file_reader)
{
  return CastPointer<AudioFileReader>(file_reader).getSampleRate();
}


int getAudioFileSampleSize(void* file_reader)
{
  return CastPointer<AudioFileReader>(file_reader).getSampleSize();
}


bool getAudioFileFloatFormat(void* file_reader)
{
  return CastPointer<AudioFileReader>(file_reader).usesFloatSamples();
}


const char* getAudioFileFormat(void* file_reader)
{
  return CastPointer<AudioFileReader>(file_reader).getFileFormat().toUTF8();
}


bool getAudioFileSamples(void* file_reader,
                         float** buffer,
                         long long start_pos,
                         int n_samples)
{
  return CastPointer<AudioFileReader>(file_reader).getSamples(buffer,
                                                             start_pos,
                                                             n_samples);
}


void* makeAudioFileWriter(const char* path, int format)
{
  return new AudioFileWriter(CharPointer_UTF8(path),
                             static_cast<audio_format_t>(format));
}


void freeAudioFileWriter (void* file_writer)
{
  delete static_cast<AudioFileWriter*>(file_writer);
}


bool writeSamplesToAudioFile(void* file_writer,
                             float** buffer,
                             int n_channels,
                             long long size,
                             double sample_rate,
                             int sample_size)
{
  return CastPointer<AudioFileWriter>(file_writer)
           .writeSamplesToFile(buffer,
                               n_channels,
                               size,
                               sample_rate,
                               sample_size);
}
