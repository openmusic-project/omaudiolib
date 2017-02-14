//
//  OMJuceHandler.hpp
//  OMJucePlayground
//
//  Created by Dimitri Bouche on 18/11/2016.
//
//

#ifndef OMSoundHandler_hpp
#define OMSoundHandler_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "OMJucePlayer.hpp"


// SUPERCLASS FOR AUDIOFILE AND AUDIOBUFFER
class OMSoundHandler : public AudioTransportSource {

protected:
    
    float gain = 1.0;
    int channels = 0;
    int sr = 44100;
    long long size = 0;
    bool repeat = false;
    AudioSourcePlayer* player;
    
public:
    
    //OMSoundHandler(float** audio_buffer, int numChannels, int numSamples, int sampleRate);
    //OMSoundHandler(const char* path);
    OMSoundHandler();
    virtual ~OMSoundHandler();
    //bool from_file = 0;
    
    int getChannels();
    long long getNumSamples();
    int getSampleRate();
    
    void setGain(float new_gain);
    float getGain();
    
    // THESE TWO ARE DIFFERENT DEPENDING ON THE CLASS
	virtual void setPlayheadPos(int64 pos) { (void) pos; };
    virtual int64 getPlayheadPos () { return 0; };
    
    virtual void playOnPlayer (OMJucePlayer* player_) { (void)player_; };
    virtual void pauseOnPlayer (OMJucePlayer *player_) { (void)player_; };
    virtual void stopOnPlayer (OMJucePlayer *player_) { (void)player_; };
    
};

#endif /* OMSoundHandler_hpp */
