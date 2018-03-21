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

#include "../JuceLibraryCode/JuceHeader.h"
#include "OMSourceHandler.hpp"


OMSourceHandler::OMSourceHandler() : AudioTransportSource()
{
    player.setSource( this );
}

int OMSourceHandler::getChannels() const { return channels; }

int64 OMSourceHandler::getNumSamples() const { return size; }

int OMSourceHandler::getSampleRate() const { return sr; }

float OMSourceHandler::getGain() const { return gain; }

void OMSourceHandler::setGain(float new_gain){ gain = new_gain; }





