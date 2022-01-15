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

#ifndef API_hpp
#define API_hpp

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

// PLAYER

OM_JUCE_API void* openAudioManager();

OM_JUCE_API void closeAudioManager (void* player);

OM_JUCE_API void initializeAudioChannels(void* player,
                                         int ninputs,
                                         int noutputs);

OM_JUCE_API int getInputChannelsCount(void* player);

OM_JUCE_API int getOutputChannelsCount(void* player);

OM_JUCE_API int setOutputChannelsMapping(void* player, int n, int *map);

// DEVICES

OM_JUCE_API int getDevicesTypeCount(void* player);

OM_JUCE_API const char* getDeviceTypeName(void* player, int i);

OM_JUCE_API void setDeviceType(void* player, const char* type);

OM_JUCE_API const char* getCurrentDeviceType(void* player);

OM_JUCE_API int getInputDevicesCount(void* player);

OM_JUCE_API int getOutputDevicesCount(void* player);

OM_JUCE_API int getInputDevicesCountForType(void* player,
                                            int device_type_num);

OM_JUCE_API int getOutputDevicesCountForType(void* player,
                                             int device_type_num);

OM_JUCE_API const char* getNthInputDeviceName(void* player,
                                              int device_type_num,
                                              int device_num);

OM_JUCE_API const char* getNthOutputDeviceName(void* player,
                                               int device_type_num,
                                               int device_num);

OM_JUCE_API int setInputDevice(void* player, int deviceNum);

OM_JUCE_API int setOutputDevice(void* player, int deviceNum);

OM_JUCE_API const char* getCurrentDeviceName(void* player);

// SETTINGS

OM_JUCE_API int getAvailableSampleRatesCount(void* player);

OM_JUCE_API int getNthAvailableSampleRate(void* player, int n);

OM_JUCE_API int getCurrentSampleRate(void* player);

OM_JUCE_API int setSampleRate(void* player, int sr);

OM_JUCE_API int getAvailableBufferSizesCount(void* player);

OM_JUCE_API int getNthAvailableBufferSize(void* player, int n);

OM_JUCE_API int getDefaultBufferSize(void* player);

OM_JUCE_API int getCurrentBufferSize(void* player);

OM_JUCE_API int setBufferSize(void* player, int size);

OM_JUCE_API void setAudioDevice(void* player,
                                int inputDevice,
                                int outputDevice,
                                int ninputs,
                                int noutputs,
                                int samplerate,
                                int buffer_size);

// SOURCES

OM_JUCE_API void* makeAudioSourceFromBuffer(float** audio_buffer,
                                            int numChannels,
                                            int numSamples,
                                            int sampleRate);

OM_JUCE_API void* makeAudioSourceFromFile(const char* path);

OM_JUCE_API void freeAudioSource (void* source);

OM_JUCE_API void startAudioSource(void* player, void* source);

OM_JUCE_API void pauseAudioSource(void* player, void* source);

OM_JUCE_API void stopAudioSource(void* player, void* source);

OM_JUCE_API long long getAudioSourcePos(void* source);

OM_JUCE_API void setAudioSourcePos(void* source, long long pos);

OM_JUCE_API float getAudioSourceGain(void* source);

OM_JUCE_API void setAudioSourceGain (void* source, float gain);

// FILE I/O

OM_JUCE_API void* makeAudioFileReader(const char* path);

OM_JUCE_API void freeAudioFileReader(void* filereader);

OM_JUCE_API int getAudioFileNumChannels(void* filereader);

OM_JUCE_API long long getAudioFileNumSamples(void* filereader);

OM_JUCE_API double getAudioFileSampleRate(void* filereader);

OM_JUCE_API int getAudioFileSampleSize(void* filereader);

OM_JUCE_API bool getAudioFileFloatFormat(void* filereader);

OM_JUCE_API const char* getAudioFileFormat(void* filereader);

OM_JUCE_API bool getAudioFileSamples(void* filereader,
                                     float** buffer,
                                     long long startPos,
                                     int n_samples);

OM_JUCE_API void* makeAudioFileWriter(const char* path, int format);

OM_JUCE_API void freeAudioFileWriter(void* filewriter);

OM_JUCE_API bool writeSamplesToAudioFile(void* filewriter,
                                         float** buffer,
                                         int n_channels,
                                         long long size,
                                         double sr,
                                         int ss);

#endif /* API_hpp */
