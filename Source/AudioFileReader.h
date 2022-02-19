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


// Todo: See if AudioFileSource could help/replace this

class AudioFileReader
{
public:

  AudioFileReader(String path);
  ~AudioFileReader();

  bool isValid();

  int getNumChannels() const;
  long long getNumSamples() const;
  double getSampleRate() const;
  int getSampleSize() const;
  bool usesFloatSamples() const;
  String getFileFormat() const;

  bool getSamples (float** dest_buffer, int64 start_sample, int n_samples);

protected:

  File m_file;

  AudioFormatManager m_format_manager;
  std::unique_ptr<AudioFormatReader> m_reader;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFileReader)
};
