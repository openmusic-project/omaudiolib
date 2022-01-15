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

#include "AudioBufferSource.hpp"

#include "Player.hpp"

#include <assert.h>


AudioBufferSource::AudioBufferSource(float** audio_buffer,
                                     int numChannels,
                                     int numSamples,
                                     int sampleRate)
: SourceHandler()
{
  buffer.setDataToReferTo(audio_buffer, numChannels, numSamples);
  channels = numChannels;
  size = numSamples;
  sr = sampleRate;
  position = 0;
  repeat = false;
}

/// Careful with casts from int64 to int here...
void AudioBufferSource::getNextAudioBlock(const AudioSourceChannelInfo& info)
{
  int64 buffer_samples = size;

  unsigned int buffer_channels = channels;
  int output_channels = info.buffer->getNumChannels();
  int routedChannel;

  // clear to avoid input leak
  info.buffer->clear(info.startSample, info.numSamples);

  if(bufferplaying())
  {
    if (bufferpaused())
    {
      // just stopped playing: fade out the last block..
      for (int i = output_channels; --i >= 0;)
      {
        info.buffer->applyGainRamp (i, info.startSample, jmin (256, info.numSamples), 1.0f, 0.0f);
      }

      if (info.numSamples > 256)
      {
        info.buffer->clear (info.startSample + 256, info.numSamples - 256);
      }
    }
    else if (info.numSamples > 0)
    {
      int startp = (int) position;
      int number_to_copy = 0;
      bool loopguard = false;
      bool stopguard = false;
      int number_before_end = 0;
      int number_after_start = 0;

      if (startp + info.numSamples < buffer_samples)
      {
        number_to_copy = info.numSamples;
      }
      else if (startp > buffer_samples)
      {
        number_to_copy = 0;
      }
      else if (buffer_samples - startp > 0)
      {
        if (!isLooping())
        {
          number_to_copy = (int) (buffer_samples - startp);
          bufferstate = Player::State::Stopped;
          stopguard = true;
        }
        else
        {
          number_to_copy = info.numSamples;
          number_before_end = (int) (buffer_samples - startp);
          number_after_start = info.numSamples + (int) (startp - buffer_samples);
          loopguard = true;
        }
      }
      else
      {
        number_to_copy = 0;
      }

      if (number_to_copy > 0)
      {
        if (!loopguard)
        {
          // Normal play (no loop)
          if (buffer_channels <= 1)
          {
            // MONO: Copy source to all output channels
            for (int out_channel = 0; out_channel < output_channels; out_channel++)
            {
              info.buffer->copyFrom(out_channel,
                                    info.startSample,
                                    this->buffer,
                                    0,
                                    startp,
                                    number_to_copy);
            }
          }
          else
          {
            assert(routing != nullptr);

            for (unsigned int ch = 0; ch < buffer_channels ; ch++)
            {
              if (ch >= routing->size())
              {
                routedChannel = ch; // do nothing
              }
              else
              {
                routedChannel = (*routing)[ch];
              }

              if (routedChannel == -1)
              {
                routedChannel = ch; // I think this can never happen anymore..
              }

              if (routedChannel != -2 // code for 'mute channel'
                  && routedChannel < output_channels)
              {
                // => Route here
                info.buffer->addFrom(routedChannel,
                                     info.startSample,
                                     this->buffer,
                                     ch,
                                     startp,
                                     number_to_copy);
              }
            }
          }
          if (stopguard)
          {
            position = 0;
          }
          else
          {
            position += number_to_copy;
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
                                     this->buffer,
                                     0,
                                     startp,
                                     number_before_end);

              info.buffer->copyFrom (out_channel,
                                     info.startSample + number_before_end,
                                     this->buffer,
                                     0,
                                     0,
                                     number_after_start);
            }
          }
          else
          {
            for (int out_channel = 0; out_channel < output_channels; out_channel++)
            {
              if ((*routing)[out_channel] != -2)
              {
                if ((*routing)[out_channel] < 0)
                {
                  routedChannel = out_channel;
                }
                else
                {
                  routedChannel = (*routing)[out_channel];
                }

                info.buffer->addFrom(routedChannel,
                                     info.startSample,
                                     this->buffer,
                                     out_channel,
                                     startp,
                                     number_before_end);

                info.buffer->addFrom(routedChannel,
                                     info.startSample + number_before_end,
                                     this->buffer,
                                     out_channel,
                                     0,
                                     number_after_start);
              }
            }
          }

          position = number_after_start;
        }

        info.buffer->applyGain(info.startSample, number_to_copy, ext_gain);
      }
    }
  }
}


void AudioBufferSource::prepareToPlay(int samplesPerBlockExpected,
                                      double sampleRate_)
{
  IgnoreUnused(sampleRate_);

  IgnoreUnused(samplesPerBlockExpected);

//  if (fromfile)
//  {
//    AudioTransportSource::prepareToPlay(samplesPerBlockExpected, sampleRate);
//  }
}


void AudioBufferSource::releaseResources()
{
//  if (fromfile)
//  {
//    AudioTransportSource::releaseResources();
//  }
}


void AudioBufferSource::setPlayheadPos(int64 pos)
{
  if (pos >= 0 && pos < size)
  {
    position = pos;
  }
}


int64 AudioBufferSource::getPlayheadPos() const
{
  return position;
}


void AudioBufferSource::setBuffer(float** audio_buffer,
                                  int numChannels,
                                  int numSamples)
{
  buffer.setDataToReferTo(audio_buffer, numChannels, numSamples);

  setNextReadPosition(0);
}

void AudioBufferSource::setRouting(const std::vector<int>& routingPtr)
{
  routing = (std::vector<int>*) &routingPtr;
}


void AudioBufferSource::bufferplay()
{
  bufferstate = Player::State::Playing;

  this->start();
}


void AudioBufferSource::bufferpause()
{
  bufferstate = Player::State::Paused;

  this->stop();
}


void AudioBufferSource::bufferstop()
{
  bufferstate = Player::State::Stopped;

  this->setPosition(0.0);
}


bool AudioBufferSource::bufferplaying()
{
  return bufferstate == Player::State::Playing
         || bufferstate == Player::State::Paused;
}


bool AudioBufferSource::bufferpaused()
{
  return bufferstate == Player::State::Paused;
}


bool AudioBufferSource::bufferstopped()
{
  return bufferstate == Player::State::Stopped;
}


void AudioBufferSource::playOnPlayer(Player& p)
{
  registerInPlayer(p);

  bufferplay();

  p.addAudioCallback(&player);
}


void AudioBufferSource::pauseOnPlayer(Player& p)
{
  IgnoreUnused(p);

  bufferpause();
}


void AudioBufferSource::stopOnPlayer(Player& p)
{
  unregisterInPlayer(p);

  bufferstop();

  p.removeAudioCallback(&player);
}


int AudioBufferSource::registerInPlayer(Player& p)
{
  setRouting(p.outputChannelsRouting);

  return p.registerBuffer(&player);
}


int AudioBufferSource::unregisterInPlayer(Player& p)
{
  return p.unregisterBuffer(&player);
}
