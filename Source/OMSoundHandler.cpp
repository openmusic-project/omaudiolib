//
//  OMSoundHandler.cpp
//  Created by Dimitri Bouche on 18/11/2016.
//  Revision by Jean Bresson
//

#include "../JuceLibraryCode/JuceHeader.h"
#include "OMSoundHandler.hpp"

// Initialize with a buffer
/*
OMSoundHandler::OMSoundHandler(float** audio_buffer, int numChannels, int numSamples, int sampleRate){
    //from_file = 0;
    //buffer = new OMJuceBuffer (audio_buffer, numChannels, numSamples, sampleRate);
}
*/

/*
// Initialise from file
OMSoundHandler::OMSoundHandler(const char* path){
    from_file = 1;
    file = new OMJuceAudioFile(path);
}
*/

OMSoundHandler::OMSoundHandler()
: AudioTransportSource()
{
    player.setSource( this );
}

//////////////////////
// ACCESSORS
//////////////////////

int OMSoundHandler::getChannels() const { return channels; }
int64 OMSoundHandler::getNumSamples() const { return size; }
int OMSoundHandler::getSampleRate() const { return sr; }
float OMSoundHandler::getGain() const { return gain; }
void OMSoundHandler::setGain(float new_gain){ gain = new_gain; }





