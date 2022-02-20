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

#pragma once

#ifdef _MSC_VER
#define OM_JUCE_VISIBILITY_DEFAULT	__declspec( dllexport )
#else
#define OM_JUCE_VISIBILITY_DEFAULT	__attribute__ ((visibility ("default")))
#endif

#ifdef __cplusplus
#define OM_JUCE_C_EXPORTS   extern "C"
#else
#define OM_JUCE_C_EXPORTS
#endif

#define OM_JUCE_API OM_JUCE_C_EXPORTS OM_JUCE_VISIBILITY_DEFAULT

OM_JUCE_API void InitTestEnv();
OM_JUCE_API void KillTestEnv();


// PLAYER

#define OMAUDIOLIB_VERSION 0x10100

OM_JUCE_API const char* versionString();

// PLAYER

OM_JUCE_API void* openAudioManager();

OM_JUCE_API void closeAudioManager (void* player);


// DEVICES

OM_JUCE_API int getDevicesTypeCount(void* player);

OM_JUCE_API const char* getDeviceTypeName(void* player, int device_type_index);

OM_JUCE_API void setDeviceType(void* player, const char* type);

OM_JUCE_API const char* getCurrentDeviceType(void* player);

OM_JUCE_API int getInputDevicesCountForType(void* player,
                                            int device_type_index);

OM_JUCE_API int getOutputDevicesCountForType(void* player,
                                             int device_type_index);

OM_JUCE_API const char* getNthInputDeviceName(void* player,
                                              int device_type_index,
                                              int n);

OM_JUCE_API const char* getNthOutputDeviceName(void* player,
                                               int device_type_index,
                                               int n);

OM_JUCE_API int setInputDevice(void* player, int device_index);

OM_JUCE_API int setOutputDevice(void* player, int device_index);

OM_JUCE_API const char* getCurrentDeviceName(void* player);


// CHANNELS

OM_JUCE_API void initializeAudioChannels(void* player,
                                         int n_inputs,
                                         int n_outputs);

OM_JUCE_API int getInputChannelsCount(void* player);

OM_JUCE_API int getOutputChannelsCount(void* player);

OM_JUCE_API int setOutputChannelsMapping(void* player, int n_channels, int *map);


// SETTINGS

OM_JUCE_API int getAvailableSampleRatesCount(void* player);

OM_JUCE_API int getNthAvailableSampleRate(void* player, int n);

OM_JUCE_API int getCurrentSampleRate(void* player);

OM_JUCE_API int setSampleRate(void* player, int sample_rate);

OM_JUCE_API int getAvailableBufferSizesCount(void* player);

OM_JUCE_API int getNthAvailableBufferSize(void* player, int n);

OM_JUCE_API int getDefaultBufferSize(void* player);

OM_JUCE_API int getCurrentBufferSize(void* player);

OM_JUCE_API int setBufferSize(void* player, int size);


// SOURCES

OM_JUCE_API void* makeAudioSourceFromBuffer(float** audio_buffer,
                                            int num_channels,
                                            int num_samples,
                                            int sample_rate);

OM_JUCE_API void* makeAudioSourceFromFile(const char* path);

OM_JUCE_API void freeAudioSource(void* source);

OM_JUCE_API void startAudioSource(void* player, void* source);

OM_JUCE_API void pauseAudioSource(void* player, void* source);

OM_JUCE_API void stopAudioSource(void* player, void* source);

OM_JUCE_API long long getAudioSourcePos(void* source);

OM_JUCE_API void setAudioSourcePos(void* source, long long pos);

OM_JUCE_API float getAudioSourceGain(void* source);

OM_JUCE_API void setAudioSourceGain(void* source, float gain);

// FILE I/O

OM_JUCE_API void* makeAudioFileReader(const char* path);

OM_JUCE_API void freeAudioFileReader(void* file_reader);

OM_JUCE_API int getAudioFileNumChannels(void* file_reader);

OM_JUCE_API long long getAudioFileNumSamples(void* file_reader);

OM_JUCE_API double getAudioFileSampleRate(void* file_reader);

OM_JUCE_API int getAudioFileSampleSize(void* file_reader);

OM_JUCE_API bool getAudioFileFloatFormat(void* file_reader);

OM_JUCE_API const char* getAudioFileFormat(void* file_reader);

OM_JUCE_API bool getAudioFileSamples(void* file_reader,
                                     float** buffer,
                                     long long start_pos,
                                     int n_samples);

OM_JUCE_API void* makeAudioFileWriter(const char* path, int format);

OM_JUCE_API void freeAudioFileWriter(void* file_writer);

OM_JUCE_API bool writeSamplesToAudioFile(void* file_writer,
                                         float** buffer,
                                         int n_channels,
                                         long long size,
                                         double sample_rate,
                                         int sample_size);
