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


#ifndef AudioBufferSource_hpp
#define AudioBufferSource_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "SourceHandler.hpp"
#include "Player.hpp"

/************************************************************************************/
//@brief           OMAudioBufferSource class definition
//@comment         A structure to play foreign allocated buffer with an OMPlayer
/************************************************************************************/


class OMAudioBufferSource : public OMSourceHandler
{
private:

  //foreign buffer to refer to (separate, not interleaved !)
  AudioSampleBuffer buffer;
  int64 position = 0;
  OMPlayer::State bufferstate = OMPlayer::State::Stopped;

  std::vector<int> * routing; // a pointer to the players' channel routing vector

public:
  OMAudioBufferSource(float** audio_buffer, int numChannels, int numSamples, int sampleRate);
  ~OMAudioBufferSource() = default;

  //method to collect the next buffer to send to the sound card
  void getNextAudioBlock (const AudioSourceChannelInfo& info) override final;
  //method automatically called before starting playback (to prepare data if needed)
  void prepareToPlay(int, double) override final;
  //method called when playback is stopped (to free data if needed)
  void releaseResources() override final;

  //set playback position in sample
  //void setNextReadPosition (long long newPosition);
  //get playback position in sample
  //long long getNextReadPosition() const;

  void setPlayheadPos (int64 pos) override final;
  int64 getPlayheadPos () const override final;

  //get buffer size
  //long long getTotalLength() const;
  //returns the repeat attribute value
  //bool isLooping() const;
  //set the repeat attribute value
  //void setLooping (bool shouldLoop);

  //returns true if playing or paused
  bool bufferplaying();
  //returns true if paused
  bool bufferpaused();
  //returns true if stopped
  bool bufferstopped();


  void playOnPlayer (OMPlayer & p) override final;
  void pauseOnPlayer (OMPlayer & p) override final;
  void stopOnPlayer (OMPlayer & p) override final;


  //set the pointer to the channel routing vector
  void setRouting (const std::vector<int> & routingPtr);

  //set the foreign buffer to refer to
  void setBuffer (float** audio_buffer, int numChannels, int numSamples);

  //starts playback (affects the state)
  void bufferplay();
  //pauses playback (affects the state)
  void bufferpause();
  //stops playback (affects the state and the NextReadPosition)
  void bufferstop();

  //registers/unregister a buffer (when playback is requested)
  int registerInPlayer(OMPlayer & p);
  int unregisterInPlayer(OMPlayer & p);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OMAudioBufferSource)
};

#endif /* AudioBufferSource_hpp */
