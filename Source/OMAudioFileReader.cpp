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

#include <stdio.h>
#include "OMAudioFileReader.hpp"

OMAudioFileReader::OMAudioFileReader( String path )
{
    // std::cout << path << std::endl;
    file = File( path );
    
    fm.registerBasicFormats();

    // !! will work only if the path has a recognized extension !
    reader = std::unique_ptr<AudioFormatReader> ( fm.createReaderFor (file) );
    
}

OMAudioFileReader::~OMAudioFileReader() {
    // delete reader;
}

bool OMAudioFileReader::isValid() {
    return (reader.get() != nullptr) ;
}


int OMAudioFileReader::getNumChannels() const {
    return reader->numChannels;
}

long long OMAudioFileReader::getNumSamples() const {
    return reader->lengthInSamples;
}

double OMAudioFileReader::getSampleRate() const {
    return reader->sampleRate;
}

int OMAudioFileReader::getSampleSize() const {
    return reader->bitsPerSample;
}

bool OMAudioFileReader::usesFloatSamples() const {
    return reader->usesFloatingPointData ;
}

String OMAudioFileReader::getFileFormat() const {
    return reader->getFormatName();
}


/*
 * TEST GET MARKERS OUT OF WAV/AIFF
 */
int OMAudioFileReader::getNumMarkers() const {
    StringPairArray mdv = reader->metadataValues;
    std::cout << "METADATA:" << std::endl;
    std::cout << mdv.getDescription() << std::endl;
    return 0;
}

long long OMAudioFileReader::getNthMarkerPos(int n) const {
    return 0;
}

String OMAudioFileReader::getNthMarkerLabel(int n) const {
    return String("???");
}


bool OMAudioFileReader::getSamples (float** dest_buffer, int64 start_sample, int n_samples){
    
    if ( isValid() )
    {
        AudioBuffer< float > b = AudioBuffer< float >( dest_buffer, reader->numChannels, n_samples );
        reader->read (&b, 0, n_samples, start_sample, true, true);
        return 1;
    }
    return 0;
}



