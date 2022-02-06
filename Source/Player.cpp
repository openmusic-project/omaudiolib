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

#include "Player.h"

#include "AudioBufferSource.h"

#include <cstring>


void omaudiolib::initTestEnv()
{
  MessageManager::getInstance();
}

void omaudiolib::killTestEnv()
{
  MessageManager::deleteInstance();
}


Player::Player()
{
  m_output_channels_routing.resize(0);
}


Player::~Player()
{
  closeAudioDevice();
}


int Player::getDevicesTypeCount()
{
  return getAvailableDeviceTypes().size();
}


String Player::getDeviceTypeName(int device_type_index)
{
  const auto& device_types = getAvailableDeviceTypes();

  return device_type_index < device_types.size()
          ? device_types[device_type_index]->getTypeName()
          : "Error";
}


void Player::setDeviceType(String type)
{
  setCurrentAudioDeviceType(type, false);
}


String Player::getCurrentDeviceType()
{
  return getCurrentAudioDeviceType();
}


int Player::getInputDevicesCountForType(int device_type_index)
{
  const auto device_type = getAvailableDeviceTypes()[device_type_index];

  device_type->scanForDevices();

  return device_type->getDeviceNames(true).size();
}


int Player::getOutputDevicesCountForType(int device_type_index)
{
  const auto device_type = getAvailableDeviceTypes()[device_type_index];

  device_type->scanForDevices();

  return device_type->getDeviceNames().size();
}


String Player::getNthInputDeviceName(int device_type_index, int n)
{
  const auto& device_types = getAvailableDeviceTypes();

  return device_type_index < device_types.size()
          ? n < getInputDevicesCountForType(device_type_index)
            ? device_types[device_type_index]->getDeviceNames(true)[n]
            : "Error accessing device type"
          : "Error accessing input device name";
}


String Player::getNthOutputDeviceName(int device_type_index, int n)
{
  const auto& device_types = getAvailableDeviceTypes();

  return device_type_index < device_types.size()
          ? n < getOutputDevicesCountForType(device_type_index)
            ? device_types[device_type_index]->getDeviceNames()[n]
            : "Error accessing device type"
          : "Error accessing output device name";
}


String Player::getCurrentDeviceName()
{
  const auto device = getCurrentAudioDevice();

  return device != nullptr ? device->getName() : "";
}


int Player::getAvailableSampleRatesCount()
{
  const auto device = getCurrentAudioDevice();

  return device != nullptr ? device->getAvailableSampleRates().size() : 0;
}


int Player::getNthAvailableSampleRate(int n)
{
  return static_cast<int>(getCurrentAudioDevice()->getAvailableSampleRates()[n]);
}


int Player::getCurrentSampleRate()
{
  const auto device = getCurrentAudioDevice();

  return device != nullptr
          ? static_cast<int>(device->getCurrentSampleRate())
          : 0;
}


int Player::setSampleRate(int sample_rate)
{
  AudioDeviceSetup res;

  getAudioDeviceSetup(res);

  res.sampleRate = static_cast<double>(sample_rate);

  setAudioDeviceSetup(res, true);

  return sample_rate;
}


int Player::getAvailableBufferSizesCount()
{
  const auto device = getCurrentAudioDevice();

  return device != nullptr ? device->getAvailableBufferSizes().size() : 0;
}


int Player::getNthAvailableBufferSize(int n)
{
  return getCurrentAudioDevice()->getAvailableBufferSizes()[n];
}


int Player::getDefaultBufferSize()
{
  const auto device = getCurrentAudioDevice();

  return device != nullptr ? device->getDefaultBufferSize() : 0;
}


int Player::getCurrentBufferSize()
{
  const auto device = getCurrentAudioDevice();

  return device != nullptr ? device->getCurrentBufferSizeSamples() : 0;
}


int Player::setBufferSize(int size)
{
  AudioDeviceSetup res;

  getAudioDeviceSetup(res);

  res.bufferSize = size;

  setAudioDeviceSetup(res, true);

  return size;
}


int Player::getOutputChannelsCount()
{
  const auto device = getCurrentAudioDevice();

  return device != nullptr ? device->getOutputChannelNames().size() : 0;
}


int Player::getInputChannelsCount()
{
  const auto device = getCurrentAudioDevice();

  return device != nullptr ? device->getInputChannelNames().size() : 0;
}


void Player::initializeAudioChannels(int n_inputs, int n_outputs)
{
  AudioDeviceSetup res;
  getAudioDeviceSetup(res);

  closeAudioDevice();

  std::cout << "Initializing audio channels [" << n_inputs << "x"
            << n_outputs << "]" << std::endl;

  initialiseWithDefaultDevices(n_inputs, n_outputs);

  setAudioDeviceSetup(res, true);

  std::cout << "Selected device = " << getCurrentDeviceName() << std::endl;
}


int Player::setOutputChannelsMapping(int n, int* map)
{
  int error = 0;
  int dest_channel = -1;
  int n_outs = getOutputChannelsCount();

  m_output_channels_routing.resize(n);
  std::fill(m_output_channels_routing.begin(), m_output_channels_routing.end(), -1);

  // std::cout << "Start Channel Mapping (" << n_outs << " channels open)" << std::endl;

  for (int i = 0; i < n ; i++)
  {
    dest_channel = map[i];

    // std::cout << "Routing channel " << i << " to output " << dest_channel << std::endl;

    if (dest_channel >= 0 && dest_channel >= n_outs)
    {
      // std::cout << "ERROR: Output channel " << dest_channel
      //           << " not available !" << std::endl;
      dest_channel = -2;
      error = -2;
    }

    std::cout << "=> " << dest_channel << std::endl;

    m_output_channels_routing.operator[](i) = dest_channel ;
  }

  return error;
}


int Player::setInputDevice(int device_index)
{
  AudioDeviceSetup s;

  getAudioDeviceSetup(s);

  s.inputDeviceName = getCurrentDeviceTypeObject()->getDeviceNames(true)[device_index];

  setAudioDeviceSetup(s, true);

  return device_index;
}


int Player::setOutputDevice(int device_index)
{
  AudioDeviceSetup s;

  getAudioDeviceSetup(s);

  s.outputDeviceName = getCurrentDeviceTypeObject()->getDeviceNames()[device_index];

  setAudioDeviceSetup(s, true);

  return device_index;
}


int Player::registerBuffer(AudioSourcePlayer* sp)
{
  IgnoreUnused(sp);

  return ++m_buffer_register_count;
}


int Player::unregisterBuffer(AudioSourcePlayer* sp)
{
  IgnoreUnused(sp);

  return --m_buffer_register_count;
}
