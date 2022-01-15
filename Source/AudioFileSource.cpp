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

#include "AudioFileSource.hpp"
#include "AudioBufferSource.hpp"
#include <cfloat>

AudioFileSource::AudioFileSource( String path ) : SourceHandler()
{
  // std::cout << path << std::endl;
  soundfile = File( path );
  formatManager.registerBasicFormats();

  AudioFormatReader* reader = formatManager.createReaderFor (soundfile.createInputStream());

  if (reader != nullptr)
  {
    int outChannels;
    if (reader->numChannels <= 1) outChannels = 2;
    else outChannels = reader->numChannels;

    ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource (reader, true);
    transportSource.setSource(newSource, 0, nullptr, reader->sampleRate, outChannels);
    sr = (int)reader->sampleRate;
    channels = reader->numChannels;
    size = reader->lengthInSamples;
    readerSource = newSource.release();

    // delete reader ;  // ?
  }
}

//method to collect the next buffer to send to the sound card
void AudioFileSource::getNextAudioBlock (const AudioSourceChannelInfo& info) {

  if (readerSource == nullptr)
  {
    info.clearActiveBufferRegion();
    return;
  }

  transportSource.getNextAudioBlock (info);
};


//method automatically called before starting playback (to prepare data if needed)
void AudioFileSource::prepareToPlay(int samplesPerBlockExpected, double sr_) {
  transportSource.prepareToPlay (samplesPerBlockExpected, sr_);
};

//method called when playback is stopped (to free data if needed)
void AudioFileSource::releaseResources() {
  transportSource.releaseResources();
};

//get buffer size
//long long AudioFileSource::getTotalLength() const {
//    return 1;
//};

//returns the repeat attribute value
//bool AudioFileSource::isLooping() const {
//    return repeat;
//};

void AudioFileSource::setGain(float new_gain) {
  SourceHandler::setGain (new_gain);
  transportSource.setGain(new_gain);
}

//set playback position in sample
void AudioFileSource::setPlayheadPos(int64 newPosition) {
  transportSource.setPosition((float)newPosition/sr);
};

//get playback position in sample
int64 AudioFileSource::getPlayheadPos() const {
  return (int64)(sr * transportSource.getCurrentPosition());
};


void AudioFileSource::playaudiofile () {
  transportSource.start();
}

void AudioFileSource::pauseaudiofile () {
  transportSource.stop();
}

void AudioFileSource::stopaudiofile () {
  transportSource.stop();
  transportSource.setPosition(0);
}


void AudioFileSource::playOnPlayer (Player & p){
  p.addAudioCallback( &player );
  playaudiofile();
}

void AudioFileSource::pauseOnPlayer(Player & p){
  IgnoreUnused( p );
  pauseaudiofile();
}

void AudioFileSource::stopOnPlayer (Player & p){
  stopaudiofile();
  p.removeAudioCallback( &player );
}