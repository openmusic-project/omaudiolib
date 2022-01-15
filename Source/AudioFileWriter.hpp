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

#ifndef AudioFileWriter_hpp
#define AudioFileWriter_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"

enum AUDIO_FORMAT {WAVE, AIFF};
typedef AUDIO_FORMAT audio_format_t;

class OMAudioFileWriter
{
    
protected:
    
    File file;
    audio_format_t audio_format ;
    
    int WRITE_BUFFER_SIZE = 4096;
    AudioFormat* getAudioFormat();

public:
    
    OMAudioFileWriter( String path, audio_format_t format ) ;
    ~OMAudioFileWriter() = default ;
    
    bool writeSamplesToFile(float** src_buffer, int n_channels, int64 size, double sr, int ss);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OMAudioFileWriter)
};

#endif /* OMAudioFileWriter_hpp */
