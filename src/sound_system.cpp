#include "sound_system.h"

SoundSystem::SoundSystem()
{
}

SoundSystem::~SoundSystem()
{
  //dtor
}

void SoundSystem::Init()
{
  FMOD_RESULT result;

  result = FMOD_System_Create(&_soundSystem);
  if (result != FMOD_OK)
  {
      Logger::Get().LogPrint("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
      return;
  }

  result = FMOD_System_Init(_soundSystem, 512, FMOD_INIT_NORMAL, nullptr);
  if (result != FMOD_OK)
  {
      Logger::Get().LogPrint("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
      return;
  }

  Logger::Get().LogPrint("FMOD sound system initialized\n");

  LoadSounds();
}

void SoundSystem::LoadSounds()
{
  FILE* f = fopen(GlobalStrings::SoundsFilename.data(), "r");
  if (f == nullptr)
  {
    Logger::Get().LogPrint("!!! ERROR !!! Could not open file %s!\n", GlobalStrings::SoundsFilename.data());
    exit(1);
  }
  Logger::Get().LogPrint("Loading sounds...\n");
  char buf[512];
  int index = 0;
  while (!feof(f))
  {
    std::string str;
    fscanf(f, "%i %s", &index, buf);

    FMOD_SOUND* p;
    FMOD_System_CreateSound(_soundSystem, buf, FMOD_DEFAULT, nullptr, &p);
    _sounds.push_back(p);

    FMOD_CHANNEL* channel = nullptr;
    _channels.push_back(channel);

    _soundsMap[index] = _sounds[index];
    _channelsMap[index] = _channels[index];

    Logger::Get().LogPrint("----|_sounds[%i] (0x%zX) = %s\n", index, _sounds[index], buf);
  }
  fclose(f);
}

void SoundSystem::PlaySound(int soundType)
{
  if (_soundsMap.count(soundType) == 1)
  {
    if (_channelsMap[soundType] != nullptr)
    {
      FMOD_Channel_Stop(_channelsMap[soundType]);
    }

    FMOD_System_PlaySound(_soundSystem, _soundsMap[soundType], nullptr, false, &_channelsMap[soundType]);
  }
}
