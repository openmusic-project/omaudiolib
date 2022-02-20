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

#include "Player.h"


// Handy function for avoiding unused variables warning
template<typename Type>
void IgnoreUnused(const Type&) noexcept {}


// Superclass of AudioBufferSource and AudioFileSource
class SourceHandler : public juce::AudioTransportSource
{
public:

  SourceHandler();

  int getNumChannels() const;
  long long getNumSamples() const;
  int getSampleRate() const;

  virtual void setPlayheadPos(int64 pos) = 0;
  virtual int64 getPlayheadPos() const = 0;

  virtual void playOnPlayer(Player& player) = 0;
  virtual void pauseOnPlayer(Player& player) = 0;
  virtual void stopOnPlayer(Player& player) = 0;

protected:

  int m_num_channels = 0;
  int m_sample_rate = 44100;
  long long m_size = 0;
  bool m_repeat = false;
  AudioSourcePlayer m_player;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SourceHandler)
};
