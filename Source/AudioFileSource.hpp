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

#ifndef AudioFileSource_hpp
#define AudioFileSource_hpp

#include "../JuceLibraryCode/JuceHeader.h"

#include "SourceHandler.hpp"


class AudioFileSource : public SourceHandler
{
private:

  File soundfile;
  AudioFormatManager formatManager;
  ScopedPointer<AudioFormatReaderSource> readerSource;
  AudioTransportSource transportSource;

public:

  AudioFileSource(String path);
  ~AudioFileSource() = default;

  // JUCE METHODS
  //method to collect the next buffer to send to the sound card
  void getNextAudioBlock(const AudioSourceChannelInfo& info) override final;
  //method automatically called before starting playback (to prepare data if needed)
  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override final;
  //method called when playback is stopped (to free data if needed)
  void releaseResources() override final;

  // NOT NECESSARY ?
  //get buffer size
  //long long getTotalLength() const override;
  //returns the repeat attribute value
  //bool isLooping() const override;

  void setPlayheadPos(int64 pos) override final;
  int64 getPlayheadPos() const override final;
  void setGain(float new_gain) override final;

  void playaudiofile();
  void pauseaudiofile();
  void stopaudiofile();

  void playOnPlayer(Player& player) override final;
  void pauseOnPlayer(Player& player) override final;
  void stopOnPlayer(Player& player) override final;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFileSource)
};

#endif /* AudioFileSource_hpp */
