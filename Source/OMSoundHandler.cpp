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

OMSoundHandler::OMSoundHandler(){
    player = new AudioSourcePlayer();
    player->setSource(this);
}

OMSoundHandler::~OMSoundHandler() {
     delete player;
}


//////////////////////
// ACCESSORS
//////////////////////

int OMSoundHandler::getChannels() { return channels; }
int64 OMSoundHandler::getNumSamples() { return size; }
int OMSoundHandler::getSampleRate() { return sr; }
float OMSoundHandler::getGain(){ return gain; }
void OMSoundHandler::setGain(float new_gain){ gain = new_gain; }





