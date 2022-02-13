// omaudiolib test

#include "../Source/API.h"

#include <cassert>
#include <iostream>

// For sleep()
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

void u_sleep(int milliseconds)
{
#ifdef _WIN32
  Sleep(milliseconds);
#else
  usleep(milliseconds * 1000);
#endif
}

//===================================
// Utils
//===================================

void PrintState(void* audio_manager)
{
  std::cout << "> CURRENT STATE: " << audio_manager << std::endl;
  std::cout << "> Device Type: " << getCurrentDeviceType(audio_manager) << std::endl;
  std::cout << "> Device Name: " << getCurrentDeviceName(audio_manager) << std::endl;
  std::cout << "> Sample rate: " << getCurrentSampleRate(audio_manager) << std::endl;
  std::cout << "> Buffer size: " << getCurrentBufferSize(audio_manager) << std::endl;
  std::cout << "> Num Inputs: " << getInputChannelsCount(audio_manager) << std::endl;
  std::cout << "> Num Outputs: " << getOutputChannelsCount(audio_manager) << std::endl;
  std::cout << std::endl;
}


float** InitializeAudioBuffer(int n_channels, int n_samples)
{
  float** buffer = (float**)malloc(sizeof(float*) * n_channels);

  for (int c = 0; c < n_channels; ++c)
  {
    buffer[c] = (float*)malloc(sizeof(float) * n_samples);
  }

  return buffer;
}


void CleanupAudioBuffer(float** buffer, int n_channels)
{
  for (int c = 0; c < n_channels; ++c)
  {
    free(buffer[c]);
  }

  free(buffer);
}


void* DefaultAudioManager()
{
  const int DEVICE_TYPE = 0; // <= set your own choice here !
  void* audio_manager = openAudioManager();
  const char* device_type_name = getDeviceTypeName(audio_manager, DEVICE_TYPE);
  std::cout << "Setting device type to: " << device_type_name << std::endl;
  setDeviceType(audio_manager, device_type_name);
  std::cout << "Setting output to: " << getNthOutputDeviceName(audio_manager, 0, 0) << std::endl;
  setOutputDevice(audio_manager, 0);
  std::cout << "SR: " << getCurrentSampleRate(audio_manager) << std::endl;

  return audio_manager;
}


//===================================
// Devices & Settings
//===================================

void ScanDevices()
{
  std::cout << std::endl << "--- ScanDevices ---" << std::endl;

  void* audio_manager = openAudioManager();

  std::cout << "Current device type: " << getCurrentDeviceType(audio_manager) << std::endl;
  std::cout << "Current device name: " << getCurrentDeviceName(audio_manager) << std::endl;

  const int device_type_count = getDevicesTypeCount(audio_manager);
  std::cout << "Number of device types: " << device_type_count << std::endl;

  for (int device_type = 0; device_type < device_type_count; ++device_type)
  {
    std::cout << std::endl << device_type << ": " << getDeviceTypeName(audio_manager, device_type) << std::endl;

    const int input_device_count =  getInputDevicesCountForType(audio_manager, device_type);
    std::cout << " Number of input devices: " << input_device_count << std::endl;

    for (int device = 0; device < input_device_count; ++device)
    {
      std::cout << " " << device << ": " << getNthInputDeviceName(audio_manager, device_type, device) << std::endl;
    }

    const int output_device_count =  getOutputDevicesCountForType(audio_manager, device_type);
    std::cout << " Number of output devices: " << output_device_count << std::endl;

    for (int device = 0; device < output_device_count; ++device)
    {
      std::cout << " " << device << ": " << getNthOutputDeviceName(audio_manager, device_type, device) << std::endl;
    }
  }

  closeAudioManager(audio_manager);
}


void ChangeDevice()
{
  std::cout << std::endl << "--- ChangeDevices ---" << std::endl;

  void* audio_manager = openAudioManager();

  PrintState(audio_manager);

  const int device_type_count = getDevicesTypeCount(audio_manager);

  for (int device_type = 0; device_type < device_type_count; ++device_type)
  {
    const char* device_type_name = getDeviceTypeName(audio_manager, device_type);

    std::cout << "=========== SETTING DEVICE TYPE: " << device_type_name << std::endl;
    setDeviceType(audio_manager, device_type_name);

    PrintState(audio_manager);

    for (int device = 0; device < getInputDevicesCountForType(audio_manager, device_type); ++device)
    {
      std::cout << "Setting INTPUT to: " << getNthInputDeviceName(audio_manager, device_type, device) << std::endl;

      setInputDevice(audio_manager, device);

      PrintState(audio_manager);
    }

  for (int device = 0; device < getOutputDevicesCountForType(audio_manager, device_type); ++device)
  {
    std::cout << "Setting OUTPUT to: " << getNthOutputDeviceName(audio_manager, device_type, device) << std::endl;

    setOutputDevice(audio_manager, device);

    PrintState(audio_manager);

    assert(getAvailableSampleRatesCount(audio_manager) > 0);
    assert(getAvailableBufferSizesCount(audio_manager) > 0);
  }
  }

  closeAudioManager(audio_manager);
}


void Settings()
{
  std::cout << std::endl << "--- Settings ---" << std::endl;

  void* audio_manager = openAudioManager();

  const int device_type_count = getDevicesTypeCount(audio_manager);

  for (int device_type = 0; device_type < device_type_count; ++device_type)
  {
    const char* device_type_name = getDeviceTypeName(audio_manager, device_type);

  std::cout << std::endl  << "=========== SETTING DEVICE TYPE: " << device_type_name << std::endl;
    setDeviceType(audio_manager, device_type_name);

    for (int out_device = 0; out_device < getOutputDevicesCountForType(audio_manager, device_type); ++out_device)
    {
      std::cout << std::endl << "Setting OUTPUT to: " << getNthOutputDeviceName(audio_manager, device_type, out_device) << std::endl;

      setOutputDevice(audio_manager, out_device);

    for (int in_device = 0; in_device < getInputDevicesCountForType(audio_manager, device_type); ++in_device)
    {
      std::cout << std::endl << "Setting INPUT to: " << getNthInputDeviceName(audio_manager, device_type, in_device) << std::endl;

      setInputDevice(audio_manager, in_device);

      const int num_sr = getAvailableSampleRatesCount(audio_manager);

      for (int i = 0; i < num_sr; ++i)
      {
        int sample_rate = getNthAvailableSampleRate(audio_manager, i);

        std::cout << "Setting sample rate to: " << sample_rate << std::endl;

        setSampleRate(audio_manager, sample_rate);

        assert(sample_rate == getCurrentSampleRate(audio_manager));
      }

      const int num_buffersize = getAvailableBufferSizesCount(audio_manager);

      if (num_buffersize > 0)
      {
        int size = getNthAvailableBufferSize(audio_manager, 0);
        std::cout << "Setting buffer size to: " << size << std::endl;
        setBufferSize(audio_manager, size);
        assert(size == getCurrentBufferSize(audio_manager));

        size = getNthAvailableBufferSize(audio_manager, num_buffersize - 1);
        std::cout << "Setting buffer size to: " << size << std::endl;
        setBufferSize(audio_manager, size);
        assert(size == getCurrentBufferSize(audio_manager));
      }
    }
    }
  }

  closeAudioManager(audio_manager);
}


void InitializeChannels()
{
  std::cout << std::endl << "--- Channels ---" << std::endl;

  void* audio_manager = openAudioManager();

  const int device_type_count = getDevicesTypeCount(audio_manager);

  for (int device_type = 0; device_type < device_type_count; ++device_type)
  {
    const char* device_type_name = getDeviceTypeName(audio_manager, device_type);

  std::cout << std::endl << "=========== SETTING DEVICE TYPE: " << device_type_name << std::endl;
  setDeviceType(audio_manager, device_type_name);

    for (int out_device = 0; out_device < getOutputDevicesCountForType(audio_manager, device_type); ++out_device)
    {
      std::cout << std::endl << "Setting OUTPUT to: " << getNthOutputDeviceName(audio_manager, device_type, out_device) << std::endl;

      setOutputDevice(audio_manager, out_device);

      for (int in_device = 0; in_device < getOutputDevicesCountForType(audio_manager, device_type); ++in_device)
      {
        std::cout << std::endl << "Setting INPUT to: " << getNthOutputDeviceName(audio_manager, device_type, in_device) << std::endl;

        setInputDevice(audio_manager, in_device);

        const int num_in = getInputChannelsCount(audio_manager);
        const int num_out = getOutputChannelsCount(audio_manager);

        // Test a few channel configurations
        initializeAudioChannels(audio_manager, 0, 0);
        initializeAudioChannels(audio_manager, 0, 1);
        initializeAudioChannels(audio_manager, 0, num_out);
        initializeAudioChannels(audio_manager, 1, 0);
        initializeAudioChannels(audio_manager, 1, 1);
        initializeAudioChannels(audio_manager, 1, num_out);
        initializeAudioChannels(audio_manager, num_in, 0);
        initializeAudioChannels(audio_manager, num_in, 1);
        initializeAudioChannels(audio_manager, num_in, num_out);
      }
    }
  }

  closeAudioManager(audio_manager);
}


//===================================
// Read
//===================================

void ReadFile(const char* path)
{
  std::cout << std::endl << "--- ReadFile ---" << std::endl;
  std::cout << "Reading: " << path << std::endl;
  void* file_reader = makeAudioFileReader(path);

  const char* format = getAudioFileFormat(file_reader);
  const int sample_size = getAudioFileSampleSize(file_reader);
  const bool float_format = getAudioFileFloatFormat(file_reader);

  std::cout << "Format: " << format
            << " (" << sample_size << "bit "
            << (float_format ? "float" : "int") << ")" << std::endl;

  const int num_channels = getAudioFileNumChannels(file_reader);
  std::cout << "Number of channels: " << num_channels << std::endl;

  const double sample_rate = getAudioFileSampleRate(file_reader);
  std::cout << "Sample rate: " << sample_rate << std::endl;

  const long long num_samples = getAudioFileNumSamples(file_reader);
  const int num_samples_truncated = (int)num_samples;
  std::cout << "Size: " << num_samples << "(" << num_samples_truncated << ")" << std::endl;

  std::cout << "Reading samples... " << std::endl;

  float** audio_buffer = InitializeAudioBuffer(num_channels, num_samples_truncated);

  bool res = getAudioFileSamples(file_reader, audio_buffer, 0, num_samples_truncated);
  assert(res == 1);
  std::cout << "Ok!" << std::endl;

  CleanupAudioBuffer(audio_buffer, num_channels);

  freeAudioFileReader(file_reader);
}


//===================================
// Play
//===================================

void PlaySequence(void* audio_manager, void* source)
{
  long long source_pos;

  source_pos = getAudioSourcePos(source);
  // We start at 0
  assert(source_pos == 0);
  std::cout << source_pos << " START" << std::endl;
  startAudioSource(audio_manager, source);

  u_sleep(1000);

  source_pos = getAudioSourcePos(source);
  // Source pos has moved forward
  assert(source_pos > 0);
  std::cout << source_pos << " PAUSE" << std::endl;
  pauseAudioSource(audio_manager, source);

  source_pos = getAudioSourcePos(source);
  std::cout << "Source position: " << source_pos << std::endl;
  // Source pos was not reset to 0
  assert(source_pos > 0);

  u_sleep(1000);
  // Source pos has not moved during the pause
  assert(getAudioSourcePos(source) == source_pos);

  std::cout << source_pos << " START" << std::endl;
  startAudioSource(audio_manager, source);
  u_sleep(1000);
  // Source pos has moved forward
  assert(getAudioSourcePos(source) > source_pos);

  std::cout << getAudioSourcePos(source) << " STOP" << std::endl;
  stopAudioSource(audio_manager, source);
  source_pos = getAudioSourcePos(source);
  std::cout << "Source position: " << source_pos << std::endl;
  // Source position is reset to 0
  assert(source_pos == 0);
  u_sleep(1000);

  // Source pos has not moved during the stop
  source_pos = getAudioSourcePos(source);
  assert(source_pos == 0);

  std::cout << source_pos << " START" << std::endl;
  startAudioSource(audio_manager, source);
  u_sleep(1000);

  source_pos = getAudioSourcePos(source);
  assert(source_pos > 0);
  std::cout << source_pos << " STOP" << std::endl;
  stopAudioSource(audio_manager, source);
  u_sleep(1000);

  const int NewPos = 200000;
  std::cout << "SET POSITION: " << NewPos << std::endl;
  setAudioSourcePos(source, NewPos);
  source_pos = getAudioSourcePos(source);
  std::cout << "Source position: " << source_pos << std::endl;
  assert(source_pos == NewPos);

  startAudioSource(audio_manager, source);
  u_sleep(2000);
  source_pos = getAudioSourcePos(source);
  std::cout << "Source position: " << source_pos << std::endl;
  assert(source_pos > NewPos);

  stopAudioSource(audio_manager, source);

  u_sleep(2000);
}


void PlayFromBuffer(const char* path)
{
  std::cout << std::endl << "--- PlayFromBuffer ---" << std::endl;

  void* audio_manager = DefaultAudioManager();

  std::cout << "Reading: " << path << std::endl;
  void* file_reader = makeAudioFileReader(path);
  const int num_channels = getAudioFileNumChannels(file_reader);
  const int num_samples = (int)getAudioFileNumSamples(file_reader);
  const int sample_rate = (int)getAudioFileSampleRate(file_reader);

  float** audio_buffer = InitializeAudioBuffer(num_channels, num_samples);

  getAudioFileSamples(file_reader, audio_buffer, 0, num_samples);
  freeAudioFileReader(file_reader);

  setSampleRate(audio_manager, sample_rate);

  void* source = makeAudioSourceFromBuffer(audio_buffer, num_channels, num_samples, sample_rate);

  std::cout << "Playing from buffer... (SR " << sample_rate << ")" << std::endl;
  PlaySequence(audio_manager, source);

  freeAudioSource(source);

  CleanupAudioBuffer(audio_buffer, num_channels);

  closeAudioManager(audio_manager);
}


void PlayFromFile(const char* path)
{
  std::cout << std::endl << "--- PlayFromFile ---" << std::endl;
  void* audio_manager = DefaultAudioManager();

  void* source = makeAudioSourceFromFile(path);

  std::cout << "Playing: " << path << std::endl;

  PlaySequence(audio_manager, source);

  freeAudioSource(source);
  closeAudioManager(audio_manager);
}


//===================================
// Gain
//===================================

void GainSequence(void* audio_manager, void* source)
{
  assert(getAudioSourceGain(source) == 1.0);

  std::cout << "Gain: 1.0" << std::endl;
  startAudioSource(audio_manager, source);
    u_sleep(1000);
  stopAudioSource(audio_manager, source);

  u_sleep(1000);

  float gain;

  gain = 0.02f;
  setAudioSourceGain(source, gain);
  assert(getAudioSourceGain(source) == gain);
  std::cout << "Gain: " << gain << std::endl;
  startAudioSource(audio_manager, source);
  u_sleep(1000);
  stopAudioSource(audio_manager, source);

  u_sleep(1000);

  gain = 0.0;
  setAudioSourceGain(source, gain);
  assert(getAudioSourceGain(source) == gain);
  std::cout << "Gain: " << gain << std::endl;
  startAudioSource(audio_manager, source);
  u_sleep(1000);
  stopAudioSource(audio_manager, source);

  u_sleep(1000);

  std::cout << "1.0-->0.0" << std::endl;
  startAudioSource(audio_manager, source);
  for (float g = 1.0f; g >= 0.0; g -= 0.1f)
  {
    std::cout << "Gain: " << g << std::endl;
    setAudioSourceGain(source, g);
    assert(getAudioSourceGain(source) == g);
    u_sleep(100);
  }

  stopAudioSource(audio_manager, source);
}


void GainWithBuffer(const char* path)
{
  std::cout << std::endl << "--- GainWithBuffer ---" << std::endl;

  void* audio_manager = DefaultAudioManager();

  std::cout << "Reading: " << path << std::endl;
  void* file_reader = makeAudioFileReader(path);
  const int num_channels = getAudioFileNumChannels(file_reader);
  const int num_samples = (int)getAudioFileNumSamples(file_reader);
  const int sample_rate = (int)getAudioFileSampleRate(file_reader);

  float** audio_buffer = InitializeAudioBuffer(num_channels, num_samples);

  getAudioFileSamples(file_reader, audio_buffer, 0, num_samples);
  freeAudioFileReader(file_reader);

  void* source = makeAudioSourceFromBuffer(audio_buffer, num_channels, num_samples, sample_rate);

  GainSequence(audio_manager, source);

  CleanupAudioBuffer(audio_buffer, num_channels);
  freeAudioSource(source);
  closeAudioManager(audio_manager);
}


void GainWithFile(const char* path)
{
  std::cout << std::endl << "--- GainWithFile ---" << std::endl;

  void* audio_manager = DefaultAudioManager();
  void* source = makeAudioSourceFromFile(path);

  GainSequence(audio_manager, source);

  freeAudioSource(source);
  closeAudioManager(audio_manager);
}


//===================================
// Sample rate
//===================================

void SampleRateSequence(void* source, int src_sample_rate)
{
  void* audio_manager = DefaultAudioManager();
  const int init_sr = getCurrentSampleRate(audio_manager);
  std::cout << "Current sample rate: " << init_sr << std::endl;

  const int num_sr = getAvailableSampleRatesCount(audio_manager);
  std::cout << "Available sample rates: " << std::endl;
  for (int i = 0; i < num_sr; ++i)
  {
	  int sr = getNthAvailableSampleRate(audio_manager, i);

	  std::cout << " " << sr;
  }
  std::cout << std::endl;

  std::cout << "Matching sample rate: setting to " << src_sample_rate << std::endl;
  setSampleRate(audio_manager, src_sample_rate);
  std::cout << "Current sample rate: " << getCurrentSampleRate(audio_manager) << std::endl;

  startAudioSource(audio_manager, source);
  u_sleep(1000);
  stopAudioSource(audio_manager, source);

  u_sleep(2000);

  const int other_sr = 2 * src_sample_rate;
  std::cout << "Different sample rate: setting to " << other_sr << std::endl;
  setSampleRate(audio_manager, other_sr);
  std::cout << "Current sample rate: " << getCurrentSampleRate(audio_manager) << std::endl;

  startAudioSource(audio_manager, source);
  u_sleep(4000);
  stopAudioSource(audio_manager, source);

  setSampleRate(audio_manager, init_sr);
  closeAudioManager(audio_manager);
}


void SampleRateWithBuffer(const char* path)
{
  std::cout << std::endl << "--- SampleRateWithBuffer ---" << std::endl;

  std::cout << "Reading: " << path << std::endl;
  void* file_reader = makeAudioFileReader(path);
  const int num_channels = getAudioFileNumChannels(file_reader);
  const int num_samples = (int)getAudioFileNumSamples(file_reader);
  const int sample_rate = (int)getAudioFileSampleRate(file_reader);
  std::cout << "Source sample rate: " << sample_rate << std::endl;

  float** audio_buffer = InitializeAudioBuffer(num_channels, num_samples);
  getAudioFileSamples(file_reader, audio_buffer, 0, num_samples);
  freeAudioFileReader(file_reader);

  void* source = makeAudioSourceFromBuffer(audio_buffer, num_channels, num_samples, sample_rate);

  SampleRateSequence(source, sample_rate);

  CleanupAudioBuffer(audio_buffer, num_channels);
  freeAudioSource(source);
}


void SampleRateWithFile(const char* path)
{
  std::cout << std::endl << "--- SampleRateWithFile ---" << std::endl;

  void* file_reader = makeAudioFileReader(path);
  const int sample_rate = (int)getAudioFileSampleRate(file_reader);
  freeAudioFileReader(file_reader);
  std::cout << "Source sample rate: " << sample_rate << std::endl;

  void* source = makeAudioSourceFromFile(path);

  SampleRateSequence(source, sample_rate);

  freeAudioSource(source);
}


//===================================
// Channels
//===================================

void ChannelSequence(void* audio_manager, void* source)
{
  initializeAudioChannels(audio_manager, 0, 0);
  std::cout << "0 Channel: You should hear nothing" << std::endl;
  startAudioSource(audio_manager, source);
  u_sleep(1000);
  stopAudioSource(audio_manager, source);

  u_sleep(2000);

  initializeAudioChannels(audio_manager, 0, 1);
  std::cout << "1 Channel: You should hear on channel 1 only" << std::endl;
  startAudioSource(audio_manager, source);
  u_sleep(1000);
  stopAudioSource(audio_manager, source);

  u_sleep(2000);

  initializeAudioChannels(audio_manager, 0, 2);
  std::cout << "2 Channels: You should hear in stereo" << std::endl;
  startAudioSource(audio_manager, source);
  u_sleep(1000);
  stopAudioSource(audio_manager, source);

  u_sleep(2000);
}


void ChannelsWithBuffer(const char* path)
{
  std::cout << std::endl << "--- ChannelsWithBuffer ---" << std::endl;

  void* audio_manager = DefaultAudioManager();

  std::cout << "Reading: " << path << std::endl;
  void* file_reader = makeAudioFileReader(path);
  const int num_channels = getAudioFileNumChannels(file_reader);
  const int num_samples = (int)getAudioFileNumSamples(file_reader);
  const int sample_rate = (int)getAudioFileSampleRate(file_reader);

  float** audio_buffer = InitializeAudioBuffer(num_channels, num_samples);

  getAudioFileSamples(file_reader, audio_buffer, 0, num_samples);
  freeAudioFileReader(file_reader);

  void* source = makeAudioSourceFromBuffer(audio_buffer, num_channels, num_samples, sample_rate);

  ChannelSequence(audio_manager, source);

  freeAudioSource(source);
  CleanupAudioBuffer(audio_buffer, num_channels);
  closeAudioManager(audio_manager);
}


void ChannelsWithFile(const char* path)
{
  std::cout << std::endl << "--- ChannelsWithFile ---" << std::endl;

  void* audio_manager = DefaultAudioManager();

  void* source = makeAudioSourceFromFile(path);

  ChannelSequence(audio_manager, source);

  freeAudioSource(source);
  closeAudioManager(audio_manager);
}


void ChannelMapping(const char* path)
{
  std::cout << std::endl << "--- ChannelMapping ---" << std::endl;

  void* audio_manager = DefaultAudioManager();

  std::cout << "Reading: " << path << std::endl;
  void* file_reader = makeAudioFileReader(path);
  const int num_channels = getAudioFileNumChannels(file_reader);
  const int num_samples = (int)getAudioFileNumSamples(file_reader);
  const int sample_rate = (int)getAudioFileSampleRate(file_reader);

  float** audio_buffer = InitializeAudioBuffer(num_channels, num_samples);

  getAudioFileSamples(file_reader, audio_buffer, 0, num_samples);
  freeAudioFileReader(file_reader);

  void* source = makeAudioSourceFromBuffer(audio_buffer, num_channels, num_samples, sample_rate);

  std::cout << "Initialize with 2 channels" << std::endl;
  initializeAudioChannels(audio_manager, 0, 2);

  int map1[4] = {0, 0, 0, 0};
  setOutputChannelsMapping(audio_manager, 4, map1);
  std::cout << "All to channel 1" << std::endl;
  startAudioSource(audio_manager, source);
  u_sleep(1000);
  stopAudioSource(audio_manager, source);

  u_sleep(1000);

  int map2[4] = {1, 1, 1, 1};
  setOutputChannelsMapping(audio_manager, 4, map2);
  std::cout << "All to channel 2" << std::endl;
  startAudioSource(audio_manager, source);
  u_sleep(1000);
  stopAudioSource(audio_manager, source);

  u_sleep(1000);

  int map3[4] = {3, 3, 3, 3};
  setOutputChannelsMapping(audio_manager, 4, map3);
  std::cout << "Wrong channel" << std::endl;
  startAudioSource(audio_manager, source);
  u_sleep(1000);
  stopAudioSource(audio_manager, source);

  u_sleep(1000);

  int map4[1] = {1};
  setOutputChannelsMapping(audio_manager, 1, map4);
  std::cout << "Channel 1 goes to 2" << std::endl;
  startAudioSource(audio_manager, source);
    u_sleep(1000);
  stopAudioSource(audio_manager, source);

  u_sleep(1000);

  std::cout << "Initialize with 1 channel" << std::endl;
  initializeAudioChannels(audio_manager, 0, 1);

  int map5[2] = {0, 0};
  setOutputChannelsMapping(audio_manager, 2, map5);
  std::cout << "Mono / All to channel 1" << std::endl;
  startAudioSource(audio_manager, source);
  u_sleep(1000);
  stopAudioSource(audio_manager, source);

  u_sleep(1000);

  int map6[2] = {1, 1};
  setOutputChannelsMapping(audio_manager, 2, map6);
  std::cout << "Mono / All to channel 2 (wrong)" << std::endl;
  startAudioSource(audio_manager, source);
  u_sleep(1000);
  stopAudioSource(audio_manager, source);

  u_sleep(1000);
  
  CleanupAudioBuffer(audio_buffer, num_channels);

  freeAudioSource(source);
  closeAudioManager(audio_manager);
}


//===================================
//===================================

int main()
{
  std::cout << "--- Testing omaudiolib ---" << std::endl;

  InitTestEnv();

  ScanDevices();
  ChangeDevice();
  Settings();
  InitializeChannels();

#ifdef _WIN32
  const char* Path1 = "C:\\Users\\Bresson\\SRC\\omaudiolib\\Test\\chords-4ch.aiff";
  const char* Path2 = "C:\\Users\\Bresson\\SRC\\omaudiolib\\Test\\sinewave.aif";
#else
  const char* Path1 = "/Users/bresson/SRC/OM/omaudiolib/Test/chords-4ch.aiff";
  const char* Path2 = "/Users/bresson/SRC/OM/omaudiolib/Test/sinewave.aif";
#endif

  ReadFile(Path1);
  PlayFromBuffer(Path1);
  GainWithBuffer(Path2);
  SampleRateWithBuffer(Path2);
  ChannelsWithBuffer(Path1);
  ChannelMapping(Path1);

  PlayFromFile(Path1);
  GainWithFile(Path2);
  SampleRateWithFile(Path2);
  ChannelsWithFile(Path1);

  KillTestEnv();

  std::cout << "DONE! (type something to exit)" << std::endl;
  getchar();
  return 0;
}
