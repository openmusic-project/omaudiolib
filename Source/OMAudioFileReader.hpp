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

#ifndef OMAudioFileReader_hpp
#define OMAudioFileReader_hpp

#include "../JuceLibraryCode/JuceHeader.h"

// probably a lot to share with OMAudioFileSource...
class OMAudioFileReader
{
    
protected:
    
    File file;
    
    AudioFormatManager fm;
    std::unique_ptr<AudioFormatReader> reader;
    
public:
    
    OMAudioFileReader( String path );
    ~OMAudioFileReader();
    
    bool isValid();
    
    int getNumChannels() const;
    long long getNumSamples() const;
    double getSampleRate() const;
    int getSampleSize() const;
    bool usesFloatSamples() const;
    String getFileFormat() const;

    int getNumMarkers() const;
    long long getNthMarkerPos(int n) const;
    String getNthMarkerLabel(int n) const;

    bool getSamples (float** dest_buffer, int64 start_sample, int n_samples);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OMAudioFileReader)
};

#endif /* OMAudioFileReader_h */
