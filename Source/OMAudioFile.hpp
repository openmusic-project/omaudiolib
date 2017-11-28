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

class OMAudioFile : public OMSoundHandler
{
private:
    File soundfile;
    AudioFormatManager formatManager;
    ScopedPointer<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    
public:
    OMAudioFile( String path );
    ~OMAudioFile() = default;
    
    // JUCE METHODS
    //method to collect the next buffer to send to the sound card
    void getNextAudioBlock (const AudioSourceChannelInfo& info) override final;
    //method automatically called before starting playback (to prepare data if needed)
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override final;
    //method called when playback is stopped (to free data if needed)
    void releaseResources() override final;
    
    // NOT NECESSARY ?
    //get buffer size
    //long long getTotalLength() const override;
    //returns the repeat attribute value
    //bool isLooping() const override;
    
    void setPlayheadPos (int64 pos) override final;
    int64 getPlayheadPos () const override final;
    void setGain(float new_gain) override final;
    
    void playaudiofile();
    void pauseaudiofile();
    void stopaudiofile();
    
    void playOnPlayer (OMJucePlayer & player) override final;
    void pauseOnPlayer (OMJucePlayer & player) override final;
    void stopOnPlayer (OMJucePlayer & player) override final;
    
    // not used.. ?
    void getSamples (float** dest_buffer, int64 start_sample, int64 end_sample);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OMAudioFile)
};

#endif /* OMAudioFile_hpp */
