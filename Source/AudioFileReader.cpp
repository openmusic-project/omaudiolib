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

#include "AudioFileReader.h"

#include <stdio.h>


AudioFileReader::AudioFileReader(String path)
{
  m_file = File(path);

  m_format_manager.registerBasicFormats();

  m_reader =
    std::unique_ptr<AudioFormatReader>
      (m_format_manager.createReaderFor(m_file.createInputStream()));
}


AudioFileReader::~AudioFileReader()
{
}


bool AudioFileReader::isValid()
{
  return (m_reader.get() != nullptr);
}


int AudioFileReader::getNumChannels() const
{
  return m_reader->numChannels;
}


long long AudioFileReader::getNumSamples() const
{
  return m_reader->lengthInSamples;
}


double AudioFileReader::getSampleRate() const
{
  return m_reader->sampleRate;
}


int AudioFileReader::getSampleSize() const
{
  return m_reader->bitsPerSample;
}


bool AudioFileReader::usesFloatSamples() const
{
  return m_reader->usesFloatingPointData;
}


String AudioFileReader::getFileFormat() const
{
  return m_reader->getFormatName();
}


bool AudioFileReader::getSamples (float** dest_buffer,
                                  int64 start_sample,
                                  int n_samples)
{
  if (isValid())
  {
    AudioBuffer<float> b = AudioBuffer<float>(dest_buffer,
                                              m_reader->numChannels,
                                              n_samples);

    m_reader->read(&b, 0, n_samples, start_sample, true, true);

    return 1;
  }

  return 0;
}
