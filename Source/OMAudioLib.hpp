/************************************************************************************/
/*!
 *  @file       OMJuceAudioLib.hpp
 *  @brief      OMJuceAudioLib C-interface header
 *  @author     Dimitri Bouche
 *  @date       04/11/2016
 *
 */
/************************************************************************************/
#ifndef OMJuceAudioLib_hpp
#define OMJuceAudioLib_hpp

#ifdef _MSC_VER
	#define OM_JUCE_VISIBILITY_DEFAULT	__declspec( dllexport )
#else
	#define OM_JUCE_VISIBILITY_DEFAULT	__attribute__ ((visibility ("default")))
#endif

#ifdef __cplusplus
#define OM_JUCE_C_EXPORTS   extern "C"
#else
#define OM_JUCE_C_EXPORTS
#endif

#define OM_JUCE_API OM_JUCE_C_EXPORTS OM_JUCE_VISIBILITY_DEFAULT


//================//
//  PLAYER UTILS  //
//================//

/************************************************************************************/
//@brief    Returns a pointer to an empty player (starting point for everything)
/************************************************************************************/
OM_JUCE_API
void* openAudioManager ();

/************************************************************************************/
//@brief          Deletes a player (AudioDeviceManager)
/************************************************************************************/
OM_JUCE_API
void closeAudioManager (void* player);

/************************************************************************************/
//@brief          Returns the number of types of audio devices available
/************************************************************************************/
OM_JUCE_API
int getDevicesTypeCount (void* player);

/************************************************************************************/
//@brief          Returns the name of nth type of audio device
/************************************************************************************/
OM_JUCE_API
const char* getDeviceTypeName (void* player, int i);

/************************************************************************************/
//@brief          (re)sets a type of device for the player
/************************************************************************************/
OM_JUCE_API
void setDeviceType (void* player, const char* type);

/************************************************************************************/
//@brief          returns the name of the current selected type of audio device
/************************************************************************************/
OM_JUCE_API
const char* getCurrentDeviceType(void* player);


/************************************************************************************/
//@brief          returns the name of the current selected type of audio device
/************************************************************************************/
OM_JUCE_API
const char* getCurrentDeviceName(void* player);

/************************************************************************************/
//@brief           Gets the number of available devices
/************************************************************************************/
OM_JUCE_API
int getInputDevicesCount (void* player);

/************************************************************************************/
//@brief           Gets the number of available devices
/************************************************************************************/
OM_JUCE_API
int getOutputDevicesCount (void* player);

/************************************************************************************/
//@brief          Returns the number of input devices for device type n
/************************************************************************************/
OM_JUCE_API
int getInputDevicesCountForType (void* player, int device_type_num);

/************************************************************************************/
//@brief          Returns the number of output devices for device type n
/************************************************************************************/
OM_JUCE_API
int getOutputDevicesCountForType (void* player, int device_type_num);

/************************************************************************************/
//@brief          Returns the input name of nth device of a given type
/************************************************************************************/
OM_JUCE_API
const char* getNthInputDeviceName(void* player, int device_type_num, int device_num);

/************************************************************************************/
//@brief          Returns the output name of nth device of a given type
/************************************************************************************/
OM_JUCE_API
const char* getNthOutputDeviceName(void* player, int device_type_num, int device_num);


/************************************************************************************/
//@brief           Gets the number of available input channels for current device
/************************************************************************************/
OM_JUCE_API
int getInputChannelsCount (void* player);

/************************************************************************************/
//@brief           Gets the number of available output channels for current device
/************************************************************************************/
OM_JUCE_API
int getOutputChannelsCount (void* player);


/************************************************************************************/
//@brief          Gets the number of available SR
/************************************************************************************/
OM_JUCE_API
int getAvailableSampleRatesCount (void* player);

/************************************************************************************/
//@brief          Gets the value of nth available SR
/************************************************************************************/
OM_JUCE_API
int getNthAvailableSampleRate (void* player, int n);

/************************************************************************************/
//@brief          Gets current SR
/************************************************************************************/
OM_JUCE_API
int getCurrentSampleRate (void* player);

/************************************************************************************/
//@brief          Sets the sample rate
/************************************************************************************/
OM_JUCE_API
int setSampleRate (void* player, int sr);


/************************************************************************************/
//@brief          Gets count of available buffer sizes
/************************************************************************************/
OM_JUCE_API
int getAvailableBufferSizesCount (void* player);

/************************************************************************************/
//@brief          Gets the value of nth available buffer size
/************************************************************************************/
OM_JUCE_API
int getNthAvailableBufferSize (void* player, int n);

/************************************************************************************/
//@brief          Gets default buff size
/************************************************************************************/
OM_JUCE_API
int getDefaultBufferSize (void* player);

/************************************************************************************/
//@brief          Gets current buff size
/************************************************************************************/
OM_JUCE_API
int getCurrentBufferSize (void* player);

/************************************************************************************/
//@brief          Sets the buffer size
/************************************************************************************/
OM_JUCE_API
int setBufferSize (void* player, int size);



/************************************************************************************/
//@brief           Returns a pointer to a player (AudioDeviceManager)
/************************************************************************************/
OM_JUCE_API
void setAudioDevice (void* player, int inputDevice, int outputDevice, int ninputs, int noutputs, int samplerate, int buffer_size);

/************************************************************************************/
//@brief          Sets a list of n mapping integers redirecting channels to outputs
/************************************************************************************/
OM_JUCE_API
int setOutputChannelsMapping(void* player, int n, int *map);



//================//
//  BUFFER UTILS  //
//================//

/************************************************************************************/
//@brief          Allocates an OMJuceBuffer
/************************************************************************************/
OM_JUCE_API
void* MakeDataReader (float** audio_buffer, int numChannels, int numSamples, int sampleRate);

/************************************************************************************/
//@brief          Allocates an OMJuceBuffer in read-from-disk mode
/************************************************************************************/
OM_JUCE_API
void* MakeFileReader (const char* path);

/************************************************************************************/
//@brief          Deletes an OMJuceBuffer
/************************************************************************************/
OM_JUCE_API
void FreeReader (void* reader);

/************************************************************************************/
//@brief          Starts or continues a buffer playback through a player
/************************************************************************************/
OM_JUCE_API
void StartReader (void* player, void* reader);

/************************************************************************************/
//@brief          Pauses a buffer playback through a player
/************************************************************************************/
OM_JUCE_API
void PauseReader (void* player, void* reader);

/************************************************************************************/
//@brief          Stops a buffer playback through a player
/************************************************************************************/
OM_JUCE_API
void StopReader (void* player, void* reader);

/************************************************************************************/
//@brief          Loop buffer playback or not
/************************************************************************************/
//void SetLoopReader (void* reader, bool looper);

/************************************************************************************/
//@brief          Sets the playback position (in samples)
/************************************************************************************/
OM_JUCE_API
void SetPosReader (void* reader, long long pos);

/************************************************************************************/
//@brief          Gets the playback position (in samples)
/************************************************************************************/
OM_JUCE_API
long long GetPosReader (void* reader);

/************************************************************************************/
//@brief          Allocates an OMJuceBuffer in read-from-disk mode
/************************************************************************************/
OM_JUCE_API
int GetChannelsReader (void* structure);

OM_JUCE_API
int GetNumSamplesReader (void* structure);

OM_JUCE_API
int GetSampleRateReader (void* structure);

OM_JUCE_API
float GetGainReader(void* reader);

OM_JUCE_API
void SetGainReader (void* reader, float gain);

#endif /* OMJuceAudioLib_hpp */
