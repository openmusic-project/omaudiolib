/************************************************************************************/
/*!
 *  @file       OMJuceBuffer.hpp
 *  @brief      OMJuceBuffer header
 *  @author     Dimitri Bouche
 *  @date       04/11/2016
 *
 */
/************************************************************************************/
#ifndef OMAudioBuffer_hpp
#define OMAudioBuffer_hpp

/************************************************************************************/
//@brief           Custom transport states definitions
//@comment         AudioTransportSource has private states attributes but it
//                 is clearer to have a custom management
/************************************************************************************/
#define PLAYING 0
#define PAUSED 1
#define STOPPED 2

#include "../JuceLibraryCode/JuceHeader.h"
#include "OMSoundHandler.hpp"

#include <vector>
using std::vector;

/************************************************************************************/
//@brief           OMAudioBuffer class definition
//@comment         A structure to play foreign allocated buffer with an OMJucePlayer
/************************************************************************************/


class OMAudioBuffer : public OMSoundHandler {

private:
    
    //foreign buffer to refer to (separate, not interleaved !)
    AudioSampleBuffer *buffer;
    int64 position = 0;
    int bufferstate = STOPPED;
    
    std::vector<int> * routing; // a pointer to the players' channel routing vector


public:
    
    OMAudioBuffer(float** audio_buffer, int numChannels, int numSamples, int sampleRate);
    ~OMAudioBuffer();
    
    //method to collect the next buffer to send to the sound card
    void getNextAudioBlock (const AudioSourceChannelInfo& info) override;
    //method automatically called before starting playback (to prepare data if needed)
    void prepareToPlay(int, double) override;
    //method called when playback is stopped (to free data if needed)
    void releaseResources() override;
    
    //set playback position in sample
    //void setNextReadPosition (long long newPosition);
    //get playback position in sample
    //long long getNextReadPosition() const;
    
    void setPlayheadPos (int64 pos) override;
    int64 getPlayheadPos () override;
    
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
    
    
    void playOnPlayer (OMJucePlayer *p) override;
    void pauseOnPlayer (OMJucePlayer *p) override;
    void stopOnPlayer (OMJucePlayer *p) override;

    
    //set the pointer to the channel routing vector
    void setRouting (std::vector<int>* routingPtr);
    
    //set the foreign buffer to refer to
    void setBuffer (float** audio_buffer, int numChannels, int numSamples);
    
    //starts playback (affects the state)
    void bufferplay();
    //pauses playback (affects the state)
    void bufferpause();
    //stops playback (affects the state and the NextReadPosition)
    void bufferstop();
    
    //registers/unregister a buffer (when playback is requested)
    int registerInPlayer(OMJucePlayer *p);
    int unregisterInPlayer(OMJucePlayer *p);
    
};

#endif /* OMAudioBuffer_hpp */
