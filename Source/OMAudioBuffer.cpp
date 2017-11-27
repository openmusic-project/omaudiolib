/************************************************************************************/
/*!
 *  @file       OMAudioBuffer.cpp
 *  @brief      OMAudioBuffer implementation
 *  @author     Dimitri Bouche
 *  @date       04/11/2016
 *
 */
/************************************************************************************/
#include "OMAudioBuffer.hpp"
#include "OMJucePlayer.hpp"

OMAudioBuffer::OMAudioBuffer(float** audio_buffer, int numChannels, int numSamples, int sampleRate)
: OMSoundHandler()
{
    buffer.setDataToReferTo(audio_buffer, numChannels, numSamples);
    channels = numChannels;
    size = numSamples;
    sr = sampleRate;
    position = 0;
    repeat = false;
}

/// Careful with casts from int64 to int here... 
void OMAudioBuffer::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
    
    // Get size and block channels to stereo
    int64 buffer_samples = size;
    
    int buffer_channels = channels;  // std::min(2,channels);
    int output_channels = info.buffer->getNumChannels();
    int routedChannel;
        
    // clear to avoid input leak
    info.buffer->clear( info.startSample, info.numSamples );
    
    if( bufferplaying() )
    {        
        if (bufferpaused())
        {
            // just stopped playing: fade out the last block..
            for (int i = output_channels; --i >= 0;)
            {
                info.buffer->applyGainRamp (i, info.startSample, jmin (256, info.numSamples), 1.0f, 0.0f);
            }
            if (info.numSamples > 256)
            {
                info.buffer->clear (info.startSample + 256, info.numSamples - 256);
            }
        }
        else if (info.numSamples > 0)
        
        {
            int startp = (int) position;
            int number_to_copy = 0;
            bool loopguard = false;
            bool stopguard = false;
            int number_before_end = 0;
            int number_after_start = 0;
            
            if (startp + info.numSamples < buffer_samples)
            {
                number_to_copy = info.numSamples;
            }
            else if (startp > buffer_samples)
            {
                number_to_copy = 0;
            }
            else if (buffer_samples - startp > 0)
            {
                if (! isLooping())
                {
                    number_to_copy = (int) (buffer_samples - startp);
                    bufferstate = OMJucePlayer::State::Stopped;
                    stopguard = true;
                }
                else
                {
                    number_to_copy = info.numSamples;
                    number_before_end = (int) (buffer_samples - startp);
                    number_after_start = info.numSamples + (int) (startp - buffer_samples);
                    loopguard = true;
                }
            }
            else
            {
                number_to_copy = 0;
            }
            
            if (number_to_copy > 0)
            {
                if (! loopguard)
                {
                    // NORMAL PLAY (NO LOOP)
                    if (buffer_channels <= 1)
                    {
                        // MONO: COPY SOURCE IN ALL OUTPUT CHANNELS
                        for (int out_channel = 0; out_channel < output_channels; out_channel++)
                        {
                            info.buffer->copyFrom  (out_channel, info.startSample, this->buffer, 0, startp, number_to_copy);
                        }
                    }
                    else
                    {
                        
                        assert( routing != nullptr );
                        
                        for (int ch = 0; ch < buffer_channels ; ch++)
                        {
                            
                            if ( ch >= routing->size() ) routedChannel = ch; // do nothing
                            else routedChannel = (*routing)[ ch ];
                            
                            if (routedChannel == -1 ) routedChannel = ch; // i think this can never happen anymore...
                            
                            if ( ( routedChannel != -2 ) // -2 = code for 'mute channel'
                                &&
                                ( routedChannel < output_channels ) )
                            {
                                // => MAIN LINE IS HERE !!
                                info.buffer->addFrom( routedChannel, info.startSample, this->buffer, ch, startp, number_to_copy );
                            }
                        }
                    }
                    if (stopguard)
                        position = 0;
                    else
                        position += number_to_copy;
                }
                else // LOOP MODE
                {
                    if (buffer_channels == 1)
                    {
                        for (int out_channel = 0; out_channel < output_channels; out_channel++)
                        {
                            info.buffer->copyFrom (out_channel, info.startSample, this->buffer, 0, startp, number_before_end);
                            info.buffer->copyFrom (out_channel, info.startSample + number_before_end, this->buffer, 0, 0, number_after_start);
                        }
                    }
                    else
                    {
                        for (int out_channel = 0; out_channel < output_channels; out_channel++)
                        {
                            if ( (*routing)[ out_channel ] != -2 )
                            {
                                if ( (*routing)[ out_channel ] < 0 )
                                {
                                    routedChannel = out_channel;
                                }
                                else
                                {
                                    routedChannel = (*routing)[ out_channel ];
                                }
                            
                                info.buffer->addFrom( routedChannel, info.startSample, this->buffer, out_channel, startp, number_before_end );
                                info.buffer->addFrom( routedChannel, info.startSample + number_before_end, this->buffer, out_channel, 0, number_after_start );
                            }
                        }
                    
                    }
                    position = number_after_start;
                }
                
                info.buffer->applyGain( info.startSample, number_to_copy, gain );
            }
        }
    }
}


void OMAudioBuffer::prepareToPlay(int samplesPerBlockExpected, double sampleRate_) {
	IgnoreUnused( sampleRate_ );
	IgnoreUnused( samplesPerBlockExpected );
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

int64 OMAudioBuffer::getPlayheadPos () const {
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
    buffer.setDataToReferTo( audio_buffer, numChannels, numSamples );
    setNextReadPosition(0);
}

void OMAudioBuffer::setRouting (const std::vector<int> & routingPtr)
{
    routing = (std::vector<int>*) &routingPtr;
    //std::cout << "BUFFER SET ROUTING [" << routing->size() << " channels]" << std::endl;
    //for (int i = 0 ; i < routing->size(); i++) { std::cout << routing->at(i) << " " ; }
    //std::cout << std::endl;
}

void OMAudioBuffer::bufferplay(){
    bufferstate = OMJucePlayer::State::Playing;
    this->start();
}

void OMAudioBuffer::bufferpause(){
    bufferstate = OMJucePlayer::State::Paused;
    this->stop();
}

void OMAudioBuffer::bufferstop(){
    //this->stop();
    bufferstate = OMJucePlayer::State::Stopped;
    this->setPosition(0.0);
}

bool OMAudioBuffer::bufferplaying() {
    return (bufferstate == OMJucePlayer::State::Playing) || (bufferstate == OMJucePlayer::State::Paused);
}

bool OMAudioBuffer::bufferpaused() {
    return bufferstate == OMJucePlayer::State::Paused;
}

bool OMAudioBuffer::bufferstopped() {
    return bufferstate == OMJucePlayer::State::Stopped;
}


//////////////////////
// PLAYER
//////////////////////

void OMAudioBuffer::playOnPlayer (OMJucePlayer & p){
    registerInPlayer(p);
    bufferplay();
    p.addAudioCallback( &player );
}


void OMAudioBuffer::pauseOnPlayer (OMJucePlayer & p){
	IgnoreUnused( p );
	bufferpause();
}

void OMAudioBuffer::stopOnPlayer(OMJucePlayer & p){
    unregisterInPlayer(p);
    bufferstop();
    p.removeAudioCallback( &player );
}


int OMAudioBuffer::registerInPlayer(OMJucePlayer & p) {
    setRouting( p.outputChannelsRouting );
    return p.registerBuffer( &player );
}

int OMAudioBuffer::unregisterInPlayer(OMJucePlayer & p) {
    return p.unregisterBuffer( &player );
}






