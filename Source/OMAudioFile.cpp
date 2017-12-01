//
//  OMJuceAudioFile.cpp
//  OMJucePlayground
//
//  Created by Dimitri Bouche on 09/11/2016.
//
//

#include "OMAudioFile.hpp"
#include "OMAudioBuffer.hpp"
#include <cfloat>

OMAudioFile::OMAudioFile( String path )
: OMSoundHandler()
{
    // std::cout << path << std::endl;
    soundfile = File( path );
    formatManager.registerBasicFormats();
    
    // !! will work only if the path has a recognized extension !
    AudioFormatReader* reader = formatManager.createReaderFor (soundfile);
    
    if (reader != nullptr)
    {
        int outChannels;
        if (reader->numChannels <= 1) outChannels = 2;
        else outChannels = reader->numChannels;
        
        ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource (reader, true);
        transportSource.setSource(newSource, 0, nullptr, reader->sampleRate, outChannels);
        sr = (int)reader->sampleRate;
        channels = reader->numChannels;
        size = reader->lengthInSamples;
        readerSource = newSource.release();
    }
}

//method to collect the next buffer to send to the sound card
void OMAudioFile::getNextAudioBlock (const AudioSourceChannelInfo& info) {
    
    if (readerSource == nullptr)
    {
        info.clearActiveBufferRegion();
        return;
    }
    
    transportSource.getNextAudioBlock (info);
};


//method automatically called before starting playback (to prepare data if needed)
void OMAudioFile::prepareToPlay(int samplesPerBlockExpected, double sr_) {
    transportSource.prepareToPlay (samplesPerBlockExpected, sr_);
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
    transportSource.setGain(new_gain);
}

//set playback position in sample
void OMAudioFile::setPlayheadPos(int64 newPosition) {
    transportSource.setPosition((float)newPosition/sr);
};

//get playback position in sample
int64 OMAudioFile::getPlayheadPos() const {
    return (int64)(sr * transportSource.getCurrentPosition());
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


void OMAudioFile::playOnPlayer (OMJucePlayer & p){
    p.addAudioCallback( &player );
    playaudiofile();
}

void OMAudioFile::pauseOnPlayer(OMJucePlayer & p){
	IgnoreUnused( p );
    pauseaudiofile();
}

void OMAudioFile::stopOnPlayer (OMJucePlayer & p){
    stopaudiofile();
    p.removeAudioCallback( &player );
}

// NEVER USED ?
void OMAudioFile::getSamples (float** dest_buffer, int64 start_sample, int64 end_sample){
    
    int** tab = (int**)malloc( channels * sizeof(int*) );
    for (int c = 0; c < channels; c++)
    {
        tab[c]=(int*)malloc((int)(end_sample - start_sample)*sizeof(int));
    }
    
    if (readerSource->getAudioFormatReader() != nullptr)
    {
        (readerSource->getAudioFormatReader())->readSamples(tab, channels, 0, start_sample, (int) (end_sample - start_sample));
    }
    
    
    for (int c = 0; c < channels; c++)
    {
        for (int smp = 0; smp < (end_sample - start_sample); smp++)
        {
            dest_buffer[c][smp] = static_cast< float >( tab[c][smp] );
        }
    }

}



