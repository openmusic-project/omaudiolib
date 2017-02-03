//
//  OMJuceAudioFile.cpp
//  OMJucePlayground
//
//  Created by Dimitri Bouche on 09/11/2016.
//
//

#include "OMAudioFile.hpp"
#include <cfloat>

OMAudioFile::OMAudioFile(const char* path) {
    filepath = path;
    File file (path);
    formatManager.registerBasicFormats();
    AudioFormatReader* reader = formatManager.createReaderFor (file);
    if (reader != nullptr)
    {
        ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource (reader, true);
        transportSource.setSource (newSource, 0, nullptr, reader->sampleRate);
        sr = reader->sampleRate;
        channels = reader->numChannels;
        size = reader->lengthInSamples;
        readerSource = newSource.release();
    }
}

OMAudioFile::~OMAudioFile() {}


//method to collect the next buffer to send to the sound card
void OMAudioFile::getNextAudioBlock (const AudioSourceChannelInfo& info) {
    if (readerSource == nullptr)
    {
        info.clearActiveBufferRegion();
        return;
    }
    transportSource.getNextAudioBlock (info);
    //position = getPosition();
};

//method automatically called before starting playback (to prepare data if needed)
void OMAudioFile::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
};

//method called when playback is stopped (to free data if needed)
void OMAudioFile::releaseResources() {
    transportSource.releaseResources();
};

//get buffer size
//long long OMAudioFile::getTotalLength() const {
//    return 1;
//};

//returns the repeat attribute value
//bool OMJuceAudioFile::isLooping() const {
//    return repeat;
//};

void OMAudioFile::setGain(float new_gain) {
    OMSoundHandler::setGain (new_gain);
    transportSource.setGain(gain);
}

//set playback position in sample
void OMAudioFile::setPlayheadPos(long long newPosition) {
    transportSource.setPosition((float)newPosition/sr);
};

//get playback position in sample
long long OMAudioFile::getPlayheadPos() {
    return (long long)sr * transportSource.getCurrentPosition();
};


void OMAudioFile::playaudiofile () {
    transportSource.start();
}

void OMAudioFile::pauseaudiofile () {
    transportSource.stop();
}

void OMAudioFile::stopaudiofile () {
    transportSource.stop();
    transportSource.setPosition(0);
}


void  OMAudioFile::playOnPlayer (OMJucePlayer* p){
        p->addAudioCallback(player);
        playaudiofile();
}

void OMAudioFile::pauseOnPlayer(OMJucePlayer* p){
    pauseaudiofile();
}

void OMAudioFile::stopOnPlayer (OMJucePlayer* p){
    stopaudiofile();
    p->removeAudioCallback(player);
}

void OMAudioFile::getSamples (float** dest_buffer, long long start_sample, long long end_sample){
    File file (filepath);
    int** tab = (int**)malloc(channels*sizeof(int*));
    for (int c = 0; c < channels; c++)
        tab[c]=(int*)malloc((end_sample - start_sample)*sizeof(int));
    if (readerSource->getAudioFormatReader() != nullptr)
    {
        (readerSource->getAudioFormatReader())->readSamples(tab, channels, 0, start_sample, (end_sample - start_sample));
    }
    
    
    for (int c = 0; c < channels; c++) {
        for (int smp = 0; smp < (end_sample - start_sample); smp++) {
            dest_buffer[c][smp] = static_cast<float>(tab[c][smp]);
        }
    }

}



