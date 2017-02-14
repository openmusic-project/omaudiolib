/************************************************************************************/
/*!
 *  @file       OMJuceBuffer.cpp
 *  @brief      OMJuceBuffer implementation
 *  @author     Dimitri Bouche
 *  @date       04/11/2016
 *
 */
/************************************************************************************/
#include "OMAudioBuffer.hpp"
#include "OMJucePlayer.hpp"

OMAudioBuffer::OMAudioBuffer(float** audio_buffer, int numChannels, int numSamples, int sampleRate)
              : OMSoundHandler() {
    buffer = new AudioSampleBuffer;
    buffer->setDataToReferTo(audio_buffer, numChannels, numSamples);
    channels = numChannels;
    size = numSamples;
    sr = sampleRate;
    position = 0;
    repeat = false;

};


OMAudioBuffer::~OMAudioBuffer(){
    delete buffer;
};

/// Careful with casts from int64 to int here... 
void OMAudioBuffer::getNextAudioBlock (const AudioSourceChannelInfo& info){
    
    //Get size and block channels to stereo
    int64 buffer_samples = size;
    int buffer_channels = channels;  // std::min(2,channels);
    int routedChannel;
        
    //clear to avoid input leak
    info.buffer->clear (info.startSample, info.numSamples);
    
    if (buffer != nullptr && bufferplaying()) {
        
        if (bufferpaused()) {
            // just stopped playing, so fade out the last block..
            for (int i = info.buffer->getNumChannels(); --i >= 0;)
                info.buffer->applyGainRamp (i, info.startSample, jmin (256, info.numSamples), 1.0f, 0.0f);
            
            if (info.numSamples > 256)
                info.buffer->clear (info.startSample + 256, info.numSamples - 256);
        }
        
        else if (info.numSamples > 0) {
            int startp = (int) position;
            int number_to_copy = 0;
            bool loopguard = false;
            bool stopguard = false;
            int number_before_end = 0;
            int number_after_start = 0;
            
            if (startp + info.numSamples < buffer_samples){
                number_to_copy = info.numSamples;
            }
            else if (startp > buffer_samples){
                number_to_copy = 0;
            }
            else if (buffer_samples - startp > 0){
                if (! isLooping()) {
                    number_to_copy = (int) (buffer_samples - startp);
                    bufferstate = STOPPED;
                    stopguard = true;
                }
                else {
                    number_to_copy = info.numSamples;
                    number_before_end = (int) (buffer_samples - startp);
                    number_after_start = info.numSamples + (int) (startp - buffer_samples);
                    loopguard = true;
                }
            }
            else{
                number_to_copy = 0;
            }
            
            if (number_to_copy > 0){
                
                if (! loopguard) {
                    if (buffer_channels <= 1) {
                        for (int channel = 0; channel < info.buffer->getNumChannels(); channel++) {
                            info.buffer->copyFrom (channel, info.startSample, *buffer, 0, startp, number_to_copy);
                        }
                    }
                    else {
                        for (int channel = 0; channel < std::min(info.buffer->getNumChannels(),buffer_channels); channel++) {
                            if (routing->at(channel) < 0) {
                                routedChannel = channel;
                            } else {
                                routedChannel = routing->at(channel);
                            }
                            info.buffer->addFrom (routedChannel, info.startSample, *buffer, channel, startp, number_to_copy);
                        }
                    }
                    if (stopguard)
                        position = 0;
                    else
                        position += number_to_copy;
                }
                else {
                    if (buffer_channels == 1) {
                        for (int channel = 0; channel < info.buffer->getNumChannels(); channel++) {
                            info.buffer->copyFrom (channel, info.startSample, *buffer, 0, startp, number_before_end);
                            info.buffer->copyFrom (channel, info.startSample + number_before_end, *buffer, 0, 0, number_after_start);
                        }
                    }
                    else {
                        for (int channel = 0; channel < info.buffer->getNumChannels(); channel++) {
                            if (routing->at(channel) < 0) {
                                routedChannel = channel;
                            } else {
                                routedChannel = routing->at(channel);
                            }
                            
                            info.buffer->addFrom (routedChannel, info.startSample, *buffer, channel, startp, number_before_end);
                            info.buffer->addFrom (routedChannel, info.startSample + number_before_end, *buffer, channel, 0, number_after_start);
                        }
                    }
                    position = number_after_start;
                }
                
                info.buffer->applyGain(info.startSample,number_to_copy,gain);
                
            }
            
            
        }
        
    }

}


void OMAudioBuffer::prepareToPlay(int samplesPerBlockExpected, double sampleRate_) {
	(void)sampleRate_;
	(void)samplesPerBlockExpected;
   // if (fromfile)
   //     AudioTransportSource::prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void OMAudioBuffer::releaseResources() {
   // if (fromfile)
   //     AudioTransportSource::releaseResources();
}

void OMAudioBuffer::setPlayheadPos (int64 pos) {
    if (pos >= 0 && pos < size)
        position = pos;
}

int64 OMAudioBuffer::getPlayheadPos () {
    return position;
}


/*
 long long OMAudioBuffer::getTotalLength() const{
    return size;
}

bool OMAudioBuffer::isLooping() const{
    return repeat;}

void OMAudioBuffer::setLooping (bool shouldLoop){
    repeat = shouldLoop;}
*/


void OMAudioBuffer::setBuffer (float** audio_buffer, int numChannels, int numSamples){
    buffer->setDataToReferTo(audio_buffer, numChannels, numSamples);
    setNextReadPosition(0);
}

void OMAudioBuffer::setRouting (std::vector<int> *routingPtr){
    routing = routingPtr;
    //std::cout << "BUFFER SET ROUTING [" << routing->size() << " channels]" << std::endl;
    //for (int i = 0 ; i < routing->size(); i++) { std::cout << routing->at(i) << " " ; }
    //std::cout << std::endl;
}

void OMAudioBuffer::bufferplay(){
    bufferstate = PLAYING;
    this->start();
}

void OMAudioBuffer::bufferpause(){
    bufferstate = PAUSED;
    this->stop();
}

void OMAudioBuffer::bufferstop(){
    //this->stop();
    bufferstate = STOPPED;
    this->setPosition(0.0);
}

bool OMAudioBuffer::bufferplaying() {
    return (bufferstate == PLAYING) || (bufferstate == PAUSED);
}

bool OMAudioBuffer::bufferpaused() {
    return bufferstate == PAUSED;
}

bool OMAudioBuffer::bufferstopped() {
    return bufferstate == STOPPED;
}


//////////////////////
// PLAYER
//////////////////////

void OMAudioBuffer::playOnPlayer (OMJucePlayer* p){
        registerInPlayer(p);
        bufferplay();
        p->addAudioCallback(player);
}


void OMAudioBuffer::pauseOnPlayer (OMJucePlayer* p){
	(void)p;
	bufferpause();
}

void OMAudioBuffer::stopOnPlayer(OMJucePlayer *p){
    unregisterInPlayer(p);
    bufferstop();
    p->removeAudioCallback(player);
}


int OMAudioBuffer::registerInPlayer(OMJucePlayer *p) {
    setRouting(p->outputChannelsRouting);
    return p->registerBuffer(player);
}

int OMAudioBuffer::unregisterInPlayer(OMJucePlayer *p) {
    return p->unregisterBuffer(player);
}






