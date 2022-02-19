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
public:

  AudioBufferSource(float** audio_buffer,
                    int num_channels,
                    int num_samples,
                    int sample_rate);

  ~AudioBufferSource() = default;

  // Overriding AudioTransportSource virtual methods:
  // collect the next buffer to send to the sound output
  void getNextAudioBlock(const AudioSourceChannelInfo& info) override final;
  // called before starting playback
  void prepareToPlay(int samplesPerBlockExpected,
                     double sampleRate) override final;
  // called when playback is stopped
  void releaseResources() override final;

  void setPlayheadPos(int64 pos) override final;
  int64 getPlayheadPos() const override final;

  void playOnPlayer(Player& p) override final;
  void pauseOnPlayer(Player& p) override final;
  void stopOnPlayer(Player& p) override final;

  void setBuffer(float** audio_buffer, int num_channels, int num_samples);

  int registerInPlayer(Player& p);
  int unregisterInPlayer(Player& p);

private:

  // foreign buffer (non-interleaved !)
  AudioSampleBuffer m_buffer;
  int64 m_position = 0;
  Player::State m_buffer_state = Player::State::Stopped;

  // a pointer to the players' channel routing vector
  std::vector<int>* m_routing;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioBufferSource)
};
