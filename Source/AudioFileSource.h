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

#include "AbstractAudioSource.h"


class AudioFileSource : public AbstractAudioSource
{
public:

  AudioFileSource(String path);
  ~AudioFileSource();

  // Overriding AudioTransportSource virtual methods:
  // collect the next buffer to send to the sound output
  void getNextAudioBlock(const AudioSourceChannelInfo& info) override final;

  void setPlayheadPos(int64 pos) override final;
  int64 getPlayheadPos() const override final;

  void playOnPlayer(Player& player) override final;
  void pauseOnPlayer(Player& player) override final;
  void stopOnPlayer(Player& player) override final;

private:

  std::unique_ptr<AudioFormatReaderSource> m_reader_source;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFileSource)
};
