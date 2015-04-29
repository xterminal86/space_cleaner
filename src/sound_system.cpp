#include "sound_system.h"

SoundSystem::SoundSystem()
{
  _musicChannel = nullptr;
}

SoundSystem::~SoundSystem()
{
  FMOD_System_Release(_soundSystem);
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

  f = fopen(GlobalStrings::MusicListFilename.data(), "r");
  if (f == nullptr)
  {
    Logger::Get().LogPrint("(warning) Could not open file %s!\n", GlobalStrings::MusicListFilename.data());
    exit(1);
  }

  Logger::Get().LogPrint("Loading music list...\n");
  while (!feof(f))
  {
    MusicData entry;
    fscanf(f, "%s %i %i", buf, &entry.LoopStartMs, &entry.LoopEndMs);

    FMOD_System_CreateSound(_soundSystem, buf, FMOD_LOOP_NORMAL, nullptr, &entry.Music);
    FMOD_Sound_SetLoopCount(entry.Music, -1);
    FMOD_Sound_SetLoopPoints(entry.Music, entry.LoopStartMs, FMOD_TIMEUNIT_PCM, entry.LoopEndMs, FMOD_TIMEUNIT_PCM);
    _musicList.push_back(entry);
  }

  /*
  FMOD_System_CreateSound(_soundSystem, "assets/music/power_blade-3.mp3", FMOD_LOOP_NORMAL, nullptr, &_music);
  FMOD_Sound_SetLoopPoints(_music, 10000, FMOD_TIMEUNIT_MS, 50000, FMOD_TIMEUNIT_MS);
  FMOD_Sound_SetLoopCount(_music, -1);
  FMOD_System_PlaySound(_soundSystem, _music, nullptr, false, nullptr);
  */
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

void SoundSystem::PlayMusic(int musicIndex)
{
  FMOD_BOOL playing = false;
  FMOD_Channel_IsPlaying(_musicChannel, &playing);
  if (playing)
  {
    FMOD_Channel_Stop(_musicChannel);
  }

  FMOD_System_PlaySound(_soundSystem, _musicList[musicIndex].Music, nullptr, false, &_musicChannel);
  FMOD_Channel_SetVolume(_musicChannel, 0.5);
}

void SoundSystem::PlayMusic()
{
  if (!_musicList.empty())
  {
    int index = Util::RandomNumber() % _musicList.size();
    PlayMusic(index);
  }
}
