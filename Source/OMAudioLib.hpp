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

//========//
// PLAYER //
//========//

OM_JUCE_API void* openAudioManager ();
OM_JUCE_API void closeAudioManager (void* player);

OM_JUCE_API void initializeAudioChannels(void* player, int ninputs, int noutputs);
OM_JUCE_API int getInputChannelsCount(void* player);
OM_JUCE_API int getOutputChannelsCount(void* player);
OM_JUCE_API int setOutputChannelsMapping(void* player, int n, int *map);

OM_JUCE_API int getDevicesTypeCount (void* player);
OM_JUCE_API const char* getDeviceTypeName (void* player, int i);
OM_JUCE_API void setDeviceType (void* player, const char* type);
OM_JUCE_API const char* getCurrentDeviceType(void* player);

OM_JUCE_API int getInputDevicesCount(void* player);
OM_JUCE_API int getOutputDevicesCount(void* player);
OM_JUCE_API int getInputDevicesCountForType(void* player, int device_type_num);
OM_JUCE_API int getOutputDevicesCountForType(void* player, int device_type_num);
OM_JUCE_API const char* getNthInputDeviceName(void* player, int device_type_num, int device_num);
OM_JUCE_API const char* getNthOutputDeviceName(void* player, int device_type_num, int device_num);
OM_JUCE_API int setInputDevice(void* player, int deviceNum);
OM_JUCE_API int setOutputDevice(void* player, int deviceNum);

OM_JUCE_API const char* getCurrentDeviceName(void* player);

OM_JUCE_API int getAvailableSampleRatesCount (void* player);
OM_JUCE_API int getNthAvailableSampleRate (void* player, int n);
OM_JUCE_API int getCurrentSampleRate (void* player);
OM_JUCE_API int setSampleRate (void* player, int sr);

OM_JUCE_API int getAvailableBufferSizesCount (void* player);
OM_JUCE_API int getNthAvailableBufferSize (void* player, int n);
OM_JUCE_API int getDefaultBufferSize (void* player);
OM_JUCE_API int getCurrentBufferSize (void* player);
OM_JUCE_API int setBufferSize (void* player, int size);

OM_JUCE_API void setAudioDevice (void* player, int inputDevice, int outputDevice, int ninputs, int noutputs, int samplerate, int buffer_size);

//===========//
//  BUFFERS  //
//===========//

OM_JUCE_API void* MakeDataReader (float** audio_buffer, int numChannels, int numSamples, int sampleRate);
OM_JUCE_API void* MakeFileReader (const char* path);
OM_JUCE_API void FreeReader (void* reader);

OM_JUCE_API void StartReader (void* player, void* reader);
OM_JUCE_API void PauseReader (void* player, void* reader);
OM_JUCE_API void StopReader (void* player, void* reader);

OM_JUCE_API long long GetPosReader(void* reader);
OM_JUCE_API void SetPosReader (void* reader, long long pos);

OM_JUCE_API float GetGainReader(void* reader);
OM_JUCE_API void SetGainReader (void* reader, float gain);

#endif /* OMJuceAudioLib_hpp */
