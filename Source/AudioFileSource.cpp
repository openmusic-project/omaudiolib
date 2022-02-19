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


AudioFileSource::AudioFileSource(String path) : SourceHandler()
{
  m_sound_file = File(path);
  m_format_manager.registerBasicFormats();

  AudioFormatReader* reader =
    m_format_manager.createReaderFor(m_sound_file.createInputStream());

  if (reader != nullptr)
  {
    const auto out_channels = reader->numChannels <= 1
                                ? 2
                                : reader->numChannels;

    m_reader_source = std::make_unique<AudioFormatReaderSource>(reader, true);

    m_transport_source.setSource(m_reader_source.get(),
                                 0,
                                 nullptr,
                                 reader->sampleRate,
                                 out_channels);

    m_sample_rate = (int)reader->sampleRate;
    m_num_channels = reader->numChannels;
    m_size = reader->lengthInSamples;
  }
}


void AudioFileSource::getNextAudioBlock(const AudioSourceChannelInfo& info)
{
  if (m_reader_source == nullptr)
  {
    info.clearActiveBufferRegion();

    return;
  }

  m_transport_source.getNextAudioBlock(info);
}


void AudioFileSource::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
  m_transport_source.prepareToPlay(samplesPerBlockExpected, sampleRate);
}


void AudioFileSource::releaseResources()
{
  m_transport_source.releaseResources();
}


void AudioFileSource::setGain(float new_gain)
{
  SourceHandler::setGain(new_gain);

  m_transport_source.setGain(new_gain);
}


void AudioFileSource::setPlayheadPos(int64 newPosition)
{
  m_transport_source.setNextReadPosition(newPosition);
}


int64 AudioFileSource::getPlayheadPos() const
{
  return m_transport_source.getNextReadPosition();
}


void AudioFileSource::playaudiofile()
{
  m_transport_source.start();
}

void AudioFileSource::pauseaudiofile()
{
  m_transport_source.stop();
}

void AudioFileSource::stopaudiofile()
{
  m_transport_source.stop();

  m_transport_source.setPosition(0);
}


void AudioFileSource::playOnPlayer(Player& p)
{
  p.addAudioCallback(&m_player);

  playaudiofile();
}


void AudioFileSource::pauseOnPlayer(Player& p)
{
  IgnoreUnused(p);

  pauseaudiofile();
}


void AudioFileSource::stopOnPlayer (Player & p)
{
  stopaudiofile();

  p.removeAudioCallback(&m_player);
}
