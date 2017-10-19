/************************************************************************************/
/*!
 *  @file       OMAudioBuffer.hpp
 *  @brief      OMAudioBuffer header
 *  @author     Dimitri Bouche
 *  @date       04/11/2016
 *
 */
/************************************************************************************/
#ifndef OMAudioBuffer_hpp
#define OMAudioBuffer_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "OMSoundHandler.hpp"
#include "OMJucePlayer.hpp"

/************************************************************************************/
//@brief           OMAudioBuffer class definition
//@comment         A structure to play foreign allocated buffer with an OMJucePlayer
/************************************************************************************/


class OMAudioBuffer : public OMSoundHandler
{
private:
    
    //foreign buffer to refer to (separate, not interleaved !)
    AudioSampleBuffer buffer;
    int64 position = 0;
    OMJucePlayer::State bufferstate = OMJucePlayer::State::Stopped;
    
    std::vector<int> * routing; // a pointer to the players' channel routing vector

public:
    OMAudioBuffer(float** audio_buffer, int numChannels, int numSamples, int sampleRate);
    ~OMAudioBuffer() = default;
    
    //method to collect the next buffer to send to the sound card
    void getNextAudioBlock (const AudioSourceChannelInfo& info) override final;
    //method automatically called before starting playback (to prepare data if needed)
    void prepareToPlay(int, double) override final;
    //method called when playback is stopped (to free data if needed)
    void releaseResources() override final;
    
    //set playback position in sample
    //void setNextReadPosition (long long newPosition);
    //get playback position in sample
    //long long getNextReadPosition() const;
    
    void setPlayheadPos (int64 pos) override final;
    int64 getPlayheadPos () const override final;
    
    //get buffer size
    //long long getTotalLength() const;
    //returns the repeat attribute value
    //bool isLooping() const;
    //set the repeat attribute value
    //void setLooping (bool shouldLoop);
    
    //returns true if playing or paused
    bool bufferplaying();
    //returns true if paused
    bool bufferpaused();
    //returns true if stopped
    bool bufferstopped();
    
    
    void playOnPlayer (OMJucePlayer & p) override final;
    void pauseOnPlayer (OMJucePlayer & p) override final;
    void stopOnPlayer (OMJucePlayer & p) override final;

    
    //set the pointer to the channel routing vector
    void setRouting (const std::vector<int> & routingPtr);
    
    //set the foreign buffer to refer to
    void setBuffer (float** audio_buffer, int numChannels, int numSamples);
    
    //starts playback (affects the state)
    void bufferplay();
    //pauses playback (affects the state)
    void bufferpause();
    //stops playback (affects the state and the NextReadPosition)
    void bufferstop();
    
    //registers/unregister a buffer (when playback is requested)
    int registerInPlayer(OMJucePlayer & p);
    int unregisterInPlayer(OMJucePlayer & p);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OMAudioBuffer)
};

#endif /* OMAudioBuffer_hpp */
