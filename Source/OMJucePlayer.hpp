/************************************************************************************/
/*!
 *  @file       OMJucePlayer.hpp
 *  @brief      OMJucePlayer header
 *  @author     Dimitri Bouche
 *  @date       04/11/2016
 *
 */
/************************************************************************************/
#ifndef OMJucePlayer_hpp
#define OMJucePlayer_hpp

#define N_MAX_BUFFERS 512 // not used

#include "../JuceLibraryCode/JuceHeader.h"
#include <string>
#include <vector>

using std::vector;


/************************************************************************************/
//@brief           OMJucePlayer class definition
//@comment         A structure to send OMJuceBuffers data to the sound card
/************************************************************************************/
class OMJucePlayer : public AudioDeviceManager {
    
private:
    
    //OMJuceBuffer* bufferRegister[N_MAX_BUFFERS];
    int bufferRegisterCount = 0;
    
public:

    OMJucePlayer();
    ~OMJucePlayer();
    
    std::vector<int> * outputChannelsRouting;
    
    int getDevicesTypeCount();
    const char *getDeviceTypeName(int i);
    void setDeviceType(String type);
    const char *getCurrentDeviceType();
    
    int getInputDevicesCountForType(int num_device_type);
    int getOutputDevicesCountForType(int num_device_type);
    int getInputDevicesCount();
    int getOutputDevicesCount();
    const char* getNthInputDeviceName(int device_type_num, int device_num);
    const char* getNthOutputDeviceName(int device_type_num, int device_num);
	const char* getCurrentDeviceName();
	int setInputDevice(int deviceNum);
	int setOutputDevice(int deviceNum);

    int getOutputChannelsCount();
    int getInputChannelsCount();
	void initializeAudioChannels(int inChannels, int outChannels);
	int setOutputChannelsMapping(int n, int *map);

    int getAvailableSampleRatesCount();
    int getNthAvailableSampleRate(int n);
    int getCurrentSampleRate();
    int setSampleRate (int SR);
    
    int getAvailableBufferSizesCount();
    int getNthAvailableBufferSize(int n);
    int getDefaultBufferSize();
    int getCurrentBufferSize();
    int setBufferSize (int size);
    

	void audioSetup(int inputDevice, int outputDevice, int inChannels, int outChannels, double sr, int buffer_size);

    int registerBuffer(AudioSourcePlayer *sp);
    int unregisterBuffer(AudioSourcePlayer *sp);
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OMJucePlayer)
};


#endif /* OMJucePlayer_hpp */

