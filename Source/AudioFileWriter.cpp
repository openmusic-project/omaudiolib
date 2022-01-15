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

#include "AudioFileWriter.h"


AudioFileWriter::AudioFileWriter(String path, audio_format_t format)
{
  m_file = File(path);

  m_audio_format = format;
}


AudioFormat* AudioFileWriter::getAudioFormat()
{
  switch(m_audio_format)
  {
    case WAVE:
      return new WavAudioFormat();
      break;
    case AIFF:
      return new AiffAudioFormat();
      break;
    default:
      return new WavAudioFormat();
  }
}


bool AudioFileWriter::writeSamplesToFile(float** src_buffer,
                                         int n_channels,
                                         int64 size,
                                         double sample_rate,
                                         int sample_size)
{
  if (m_file.existsAsFile())
  {
    m_file.deleteFile();
  }

  std::unique_ptr<OutputStream> out(m_file.createOutputStream());

  ScopedPointer<AudioFormat> af(getAudioFormat());

  AudioBuffer<float> buffer;

  if (out != nullptr)
  {
    ScopedPointer<AudioFormatWriter>writer(af->createWriterFor(out.get(),
                                                               sample_rate,
                                                               n_channels,
                                                               sample_size,
                                                               StringPairArray(),
                                                               0));

    int start_sample = 0;
    int64 rest_samples = size;

    while (rest_samples > 0)
    {
      const auto num_to_do = M_WRITE_BUFFER_SIZE < rest_samples
                              ? M_WRITE_BUFFER_SIZE
                              : static_cast<int>(rest_samples);

      buffer.setDataToReferTo(src_buffer, n_channels, start_sample, num_to_do);

      writer->writeFromAudioSampleBuffer(buffer, 0, num_to_do);

      start_sample += num_to_do;
      rest_samples -= num_to_do;
    }

    out.release();

    return 1;
  }

  return 0;
}
