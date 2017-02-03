//
//  OMJuceAudioFile.hpp
//  OMJucePlayground
//
//  Created by Dimitri Bouche on 09/11/2016.
//
//

#ifndef OMAudioFile_hpp
#define OMAudioFile_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "OMSoundHandler.hpp"

class OMAudioFile : public OMSoundHandler {

private :
    
    const char* filepath;
    AudioFormatManager formatManager;
    ScopedPointer<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    
public :
    
    
    OMAudioFile(const char* path);
    ~OMAudioFile();
    
    // JUCE METHODS
    //method to collect the next buffer to send to the sound card
    void getNextAudioBlock (const AudioSourceChannelInfo& info) override;
    //method automatically called before starting playback (to prepare data if needed)
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    //method called when playback is stopped (to free data if needed)
    void releaseResources() override;
    
    // NOT NECESSARY ?
    //get buffer size
    //long long getTotalLength() const override;
    //returns the repeat attribute value
    //bool isLooping() const override;
    
    void setPlayheadPos (long long pos) override;
    long long getPlayheadPos () override;
    void setGain(float new_gain);
    
    void playaudiofile ();
    void pauseaudiofile ();
    void stopaudiofile ();
    
    void playOnPlayer (OMJucePlayer* player) override;
    void pauseOnPlayer (OMJucePlayer *player) override;
    void stopOnPlayer (OMJucePlayer *player) override;

    void getSamples (float** dest_buffer, int64 start_sample, int64 end_sample);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OMAudioFile)
};

#endif /* OMAudioFile_hpp */
