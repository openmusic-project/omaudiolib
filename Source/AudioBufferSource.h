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

#include "SourceHandler.h"

#include "Player.h"


class AudioBufferSource : public SourceHandler
{
private:
  
  // foreign buffer (non-interleaved !)
  AudioSampleBuffer buffer;
  int64 position = 0;
  Player::State bufferstate = Player::State::Stopped;

  // a pointer to the players' channel routing vector
  std::vector<int>* routing;

public:

  AudioBufferSource(float** audio_buffer,
                    int numChannels,
                    int numSamples,
                    int sampleRate);

  ~AudioBufferSource() = default;

  // Overriding AudioTransportSource virtual methods:
  // collect the next buffer to send to the sound output
  void getNextAudioBlock(const AudioSourceChannelInfo& info) override final;
  // called before starting playback
  void prepareToPlay(int, double) override final;
  // called when playback is stopped
  void releaseResources() override final;

  void setPlayheadPos(int64 pos) override final;
  int64 getPlayheadPos() const override final;

  bool bufferplaying();
  bool bufferpaused();
  bool bufferstopped();


  void playOnPlayer(Player& p) override final;
  void pauseOnPlayer(Player& p) override final;
  void stopOnPlayer(Player& p) override final;


  // set the pointer to the channel routing vector
  void setRouting (const std::vector<int>& routingPtr);

  void setBuffer(float** audio_buffer, int numChannels, int numSamples);

  void bufferplay();
  void bufferpause();
  void bufferstop();

  int registerInPlayer(Player& p);
  int unregisterInPlayer(Player& p);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioBufferSource)
};
