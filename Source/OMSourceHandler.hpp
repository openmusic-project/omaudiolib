/*
 ==============================================================================
 
 This file is part of the OM audio library:
 an audio IO and playback utiloity based on the Juce framework
 
 Copyright (C) 2016-2018 Dimitri Bouche, Jean Bresson (IRCAM)
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 
 ==============================================================================
 */

#ifndef OMSourceHandler_hpp
#define OMSourceHandler_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "OMPlayer.hpp"

/************************************************************************************/
/*!
 *  @brief          Handy function for avoiding unused variables warning
 *
 */
/************************************************************************************/
template< typename Type >
void IgnoreUnused(const Type &) noexcept {}


// SUPERCLASS FOR AUDIOFILE AND AUDIOBUFFER
class OMSourceHandler : public AudioTransportSource
{
protected:
    
    float gain = 1.0;
    int channels = 0;
    int sr = 44100;
    long long size = 0;
    bool repeat = false;
    AudioSourcePlayer player;
    
public:
    OMSourceHandler();
    virtual ~OMSourceHandler() = default;
    
    int getChannels() const;
    long long getNumSamples() const;
    int getSampleRate() const;
    
    // OMAudioFileSource and OMAudioBufferSource deal differently with gain
    virtual void setGain(float new_gain);
    float getGain() const;
    
    // THESE TWO ARE DIFFERENT DEPENDING ON THE CLASS
	virtual void setPlayheadPos(int64 pos) = 0;
    virtual int64 getPlayheadPos() const = 0;
    
    virtual void playOnPlayer(OMPlayer & player_) = 0;
    virtual void pauseOnPlayer(OMPlayer & player_) = 0;
    virtual void stopOnPlayer(OMPlayer & player_) = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OMSourceHandler)
};

#endif /* OMSourceHandler_hpp */
