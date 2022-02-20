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

#include "AudioFileSource.h"

#include <cfloat>


AudioFileSource::AudioFileSource(String path)
{
  AudioFormatManager format_manager;
  
  format_manager.registerBasicFormats();

  AudioFormatReader* reader =
    format_manager.createReaderFor(File(path).createInputStream());

  if (reader != nullptr)
  {
    const auto out_channels = reader->numChannels <= 1
                                ? 2
                                : reader->numChannels;

    m_reader_source = std::make_unique<AudioFormatReaderSource>(reader, true);

    setSource(m_reader_source.get(),
              0,
              nullptr,
              reader->sampleRate,
              out_channels);

    m_sample_rate = static_cast<int>(reader->sampleRate);
    m_num_channels = reader->numChannels;
    m_size = reader->lengthInSamples;
  }
}


AudioFileSource::~AudioFileSource()
{
  setSource(nullptr);

  m_reader_source = nullptr;
}


void AudioFileSource::getNextAudioBlock(const AudioSourceChannelInfo& info)
{
  if (m_reader_source == nullptr)
  {
    info.clearActiveBufferRegion();

    return;
  }

  juce::AudioTransportSource::getNextAudioBlock(info);
}


void AudioFileSource::setPlayheadPos(int64 newPosition)
{
  setNextReadPosition(newPosition);
}


int64 AudioFileSource::getPlayheadPos() const
{
  return getNextReadPosition();
}


void AudioFileSource::playOnPlayer(Player& p)
{
  p.addAudioCallback(&m_player);

  start();
}


void AudioFileSource::pauseOnPlayer(Player& p)
{
  IgnoreUnused(p);

  stop();
}


void AudioFileSource::stopOnPlayer (Player & p)
{
  stop();

  setPosition(0);

  p.removeAudioCallback(&m_player);
}
