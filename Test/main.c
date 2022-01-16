/* omaudiolib test */

#include "../Source/API.h"

#include <iostream>


void TestAudio()
{
  std::cout << "TEST AUDIO" << std::endl;

  auto Player = OpenAudioManager();

  std::cout << "DeviceTypeCount: " << GetDevicesTypeCount(Player) << std::endl;
  std::cout << "DeviceType: " << GetCurrentDeviceType(Player) << std::endl;

  SetOutputDevice(Player, 0);
  std::cout << "DeviceName: " << GetCurrentDeviceName(Player) << std::endl;

  std::cout << "Sample rate count: " << GetAvailableSampleRatesCount(Player) << std::endl;
  std::cout << "Current sample rate: " << GetCurrentSampleRate(Player) << std::endl;

  CloseAudioManager(Player);
}


int main()
{
  std::cout << "Testing omaudiolib:" << std::endl;

  TestAudio();

  return 0;
}
