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

#include "AudioFileWriter.hpp"


OMAudioFileWriter::OMAudioFileWriter( String path, audio_format_t format )
{
    file = File( path );
    audio_format = format;
}

AudioFormat *OMAudioFileWriter::getAudioFormat()
{
    switch (audio_format) {
        case WAVE : return new WavAudioFormat();
            break;
        case AIFF : return new AiffAudioFormat();
            break;
        default: return new WavAudioFormat();
    }
}


bool OMAudioFileWriter::writeSamplesToFile (float** src_buffer, int n_channels, int64 size, double sr, int ss)
{
    if (file.existsAsFile()) file.deleteFile() ;
    
    std::unique_ptr<OutputStream> out(file.createOutputStream());

    ScopedPointer <AudioFormat> af(getAudioFormat());
    AudioBuffer<float> buffer;
    
    if (out != nullptr)
    {
        ScopedPointer <AudioFormatWriter> writer ( af->createWriterFor( out.get(), sr, n_channels, ss , StringPairArray() , 0) ) ;
        
        int startSample = 0;
        int64 restSamples = size;
        
        while (restSamples > 0)
        {
            const int numToDo = (WRITE_BUFFER_SIZE < restSamples) ? WRITE_BUFFER_SIZE : (int) restSamples;
            buffer.setDataToReferTo( src_buffer, n_channels, startSample, numToDo );
            
            writer->writeFromAudioSampleBuffer ( buffer, 0 , numToDo ) ;
            
            startSample += numToDo;
            restSamples  -= numToDo;
        }
        out.release();
        return 1;
    }
    return 0;
}
