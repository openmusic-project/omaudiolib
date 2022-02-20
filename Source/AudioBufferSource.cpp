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

#include "AudioBufferSource.h"

#include "Player.h"

#include <assert.h>


AudioBufferSource::AudioBufferSource(float** audio_buffer,
                                     int num_channels,
                                     int num_samples,
                                     int sample_rate)
{
  m_buffer.setDataToReferTo(audio_buffer, num_channels, num_samples);
  m_num_channels = num_channels;
  m_size = num_samples;
  m_sample_rate = sample_rate;
  m_position = 0;
}


/// Careful with casts from int64 to int here...
void AudioBufferSource::getNextAudioBlock(const AudioSourceChannelInfo& info)
{
  int64 buffer_samples = m_size;

  unsigned int buffer_channels = m_num_channels;
  int output_channels = info.buffer->getNumChannels();
  int routed_channel;

  // clear to avoid input leak
  info.buffer->clear(info.startSample, info.numSamples);

  if (m_buffer_state == Player::State::Paused)
  {
    // just stopped playing: fade out the last block..
    for (int i = output_channels; --i >= 0;)
    {
      info.buffer->applyGainRamp(i, info.startSample, jmin(256, info.numSamples), 1.0f, 0.0f);
    }

    if (info.numSamples > 256)
    {
      info.buffer->clear (info.startSample + 256, info.numSamples - 256);
    }
  }
  else if (m_buffer_state == Player::State::Playing && info.numSamples > 0)
  {
    int start_p = static_cast<int>(m_position);
    int number_to_copy = 0;
    bool loop_guard = false;
    bool stop_guard = false;
    int number_before_end = 0;
    int number_after_start = 0;

    if (start_p + info.numSamples < buffer_samples)
    {
      number_to_copy = info.numSamples;
    }
    else if (start_p > buffer_samples)
    {
      number_to_copy = 0;
    }
    else if (buffer_samples - start_p > 0)
    {
      if (!isLooping())
      {
        number_to_copy = static_cast<int>(buffer_samples - start_p);
        m_buffer_state = Player::State::Stopped;
        stop_guard = true;
      }
      else
      {
        number_to_copy = info.numSamples;
        number_before_end = static_cast<int>(buffer_samples - start_p);
        number_after_start = info.numSamples + static_cast<int>(start_p - buffer_samples);
        loop_guard = true;
      }
    }
    else
    {
      number_to_copy = 0;
    }

    if (number_to_copy > 0)
    {
      if (!loop_guard)
      {
        // Normal play (no loop)
        if (buffer_channels <= 1)
        {
          // MONO: Copy source to all output channels
          for (int out_channel = 0; out_channel < output_channels; out_channel++)
          {
            info.buffer->copyFrom(out_channel,
                                  info.startSample,
                                  m_buffer,
                                  0,
                                  start_p,
                                  number_to_copy);
          }
        }
        else
        {
          assert(m_routing != nullptr);

          for (unsigned int ch = 0; ch < buffer_channels ; ch++)
          {
            if (ch >= m_routing->size())
            {
              routed_channel = ch; // do nothing
            }
            else
            {
              routed_channel = (*m_routing)[ch];
            }

            if (routed_channel == -1)
            {
              routed_channel = ch; // I think this can never happen anymore..
            }

            if (routed_channel != -2 // code for 'mute channel'
                && routed_channel < output_channels)
            {
              // => Route here
              info.buffer->addFrom(routed_channel,
                                   info.startSample,
                                   m_buffer,
                                   ch,
                                   start_p,
                                   number_to_copy);
            }
          }
        }
        if (stop_guard)
        {
          m_position = 0;
        }
        else
        {
          m_position += number_to_copy;
        }
      }
      else // Loop mode
      {
        if (buffer_channels == 1)
        {
          for (int out_channel = 0; out_channel < output_channels; out_channel++)
          {
            info.buffer->copyFrom (out_channel,
                                   info.startSample,
                                   m_buffer,
                                   0,
                                   start_p,
                                   number_before_end);

            info.buffer->copyFrom (out_channel,
                                   info.startSample + number_before_end,
                                   m_buffer,
                                   0,
                                   0,
                                   number_after_start);
          }
        }
        else
        {
          for (int out_channel = 0; out_channel < output_channels; out_channel++)
          {
            if ((*m_routing)[out_channel] != -2)
            {
              if ((*m_routing)[out_channel] < 0)
              {
                routed_channel = out_channel;
              }
              else
              {
                routed_channel = (*m_routing)[out_channel];
              }

              info.buffer->addFrom(routed_channel,
                                   info.startSample,
                                   m_buffer,
                                   out_channel,
                                   start_p,
                                   number_before_end);

              info.buffer->addFrom(routed_channel,
                                   info.startSample + number_before_end,
                                   m_buffer,
                                   out_channel,
                                   0,
                                   number_after_start);
            }
          }
        }

        m_position = number_after_start;
      }

      info.buffer->applyGain(info.startSample, number_to_copy, getGain());
    }
  }
}


void AudioBufferSource::prepareToPlay(int samplesPerBlockExpected,
                                      double sampleRate)
{
  IgnoreUnused(sampleRate);
  IgnoreUnused(samplesPerBlockExpected);
}


void AudioBufferSource::releaseResources()
{
}


void AudioBufferSource::setPlayheadPos(int64 pos)
{
  if (pos >= 0 && pos < m_size)
  {
    m_position = pos;
  }
}


int64 AudioBufferSource::getPlayheadPos() const
{
  return m_position;
}


void AudioBufferSource::playOnPlayer(Player& p)
{
  m_routing = &p.m_output_channels_routing;

  m_buffer_state = Player::State::Playing;

  start();

  p.addAudioCallback(&m_player);
}


void AudioBufferSource::pauseOnPlayer(Player& p)
{
  IgnoreUnused(p);

  m_buffer_state = Player::State::Paused;

  stop();
}


void AudioBufferSource::stopOnPlayer(Player& p)
{
  m_buffer_state = Player::State::Stopped;

  m_position = 0;

  setNextReadPosition(0);

  p.removeAudioCallback(&m_player);
}
