//
//  OMJuceHandler.hpp
//  OMJucePlayground
//
//  Created by Dimitri Bouche on 18/11/2016.
//
//

#ifndef OMSoundHandler_hpp
#define OMSoundHandler_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "OMJucePlayer.hpp"

/************************************************************************************/
/*!
 *  @brief          Handy function for avoiding unused variables warning
 *
 */
/************************************************************************************/
template< typename Type >
void IgnoreUnused(const Type &) noexcept {}


// SUPERCLASS FOR AUDIOFILE AND AUDIOBUFFER
class OMSoundHandler : public AudioTransportSource
{
protected:
    
    float gain = 1.0;
    int channels = 0;
    int sr = 44100;
    long long size = 0;
    bool repeat = false;
    AudioSourcePlayer player;
    
public:
    OMSoundHandler();
    virtual ~OMSoundHandler() = default;
    
    int getChannels() const;
    long long getNumSamples() const;
    int getSampleRate() const;
    
    // OMAudioFile and OMAudio buffer deal differently with gain
    virtual void setGain(float new_gain);
    float getGain() const;
    
    // THESE TWO ARE DIFFERENT DEPENDING ON THE CLASS
	virtual void setPlayheadPos(int64 pos) = 0;
    virtual int64 getPlayheadPos() const = 0;
    
    virtual void playOnPlayer(OMJucePlayer & player_) = 0;
    virtual void pauseOnPlayer(OMJucePlayer & player_) = 0;
    virtual void stopOnPlayer(OMJucePlayer & player_) = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OMSoundHandler)
};

#endif /* OMSoundHandler_hpp */
