#include "sound_system.h"

SoundSystem::SoundSystem()
{
  _soundSystem = nullptr;
}

SoundSystem::~SoundSystem()
{
  //dtor
}

void SoundSystem::Init()
{
  FMOD_RESULT result;

  result = FMOD_System_Create(&_soundSystem);      // Create the main system object.
  if (result != FMOD_OK)
  {
      Logger::Get().LogPrint("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
      return;
  }

  result = FMOD_System_Init(_soundSystem, 512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
  if (result != FMOD_OK)
  {
      Logger::Get().LogPrint("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
      return;
  }

  Logger::Get().LogPrint("FMOD sound system initialized\n");
}
