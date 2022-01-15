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

#include "../JuceLibraryCode/JuceHeader.h"

#include <string>
#include <vector>


class Player : public juce::AudioDeviceManager
{
public:

  Player();
  ~Player();

  enum class State
  {
    Playing,
    Paused,
    Stopped,
  };

  std::vector<int> m_output_channels_routing;

  int getDevicesTypeCount();

  // Device Types = e.g. "CoreAudio", "ASIO", etc.
  String getDeviceTypeName(int device_type_index);
  void setDeviceType(String type);
  String getCurrentDeviceType();

  int getInputDevicesCountForType(int device_type_index);
  int getOutputDevicesCountForType(int device_type_index);
  int getInputDevicesCount();
  int getOutputDevicesCount();
  String getNthInputDeviceName(int device_type_index, int n);
  String getNthOutputDeviceName(int device_type_index, int n);
  String getCurrentDeviceName();
  int setInputDevice(int device_index);
  int setOutputDevice(int device_index);

  int getOutputChannelsCount();
  int getInputChannelsCount();
  void initializeAudioChannels(int n_inputs, int n_outputs);
  int setOutputChannelsMapping(int n_channels, int* map);

  int getAvailableSampleRatesCount();
  int getNthAvailableSampleRate(int n);
  int getCurrentSampleRate();
  int setSampleRate (int sample_rate);

  int getAvailableBufferSizesCount();
  int getNthAvailableBufferSize(int n);
  int getDefaultBufferSize();
  int getCurrentBufferSize();
  int setBufferSize (int size);

  void audioSetup(int n_inputs,
                  int n_outputs,
                  double sample_rate,
                  int buffer_size);

  int registerBuffer(AudioSourcePlayer *sp);
  int unregisterBuffer(AudioSourcePlayer *sp);

private:

  int m_buffer_register_count = 0;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Player)
};
