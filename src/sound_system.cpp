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
  index = 0;
  while (!feof(f))
  {
    MusicData entry;
    fscanf(f, "%s %i %i", buf, &entry.LoopStart, &entry.LoopEnd);
    Logger::Get().LogPrint("----|Track %i - %s\n", index, buf);
    FMOD_System_CreateSound(_soundSystem, buf, FMOD_LOOP_NORMAL, nullptr, &entry.Music);
    FMOD_Sound_SetLoopCount(entry.Music, -1);
    FMOD_Sound_SetLoopPoints(entry.Music, entry.LoopStart, FMOD_TIMEUNIT_PCM, entry.LoopEnd, FMOD_TIMEUNIT_PCM);
    _musicList.push_back(entry);
    index++;
  }
  fclose(f);

  Logger::Get().LogPrint("Music list loaded!\n");
}

void SoundSystem::PlaySound(int soundType)
{
  if (_soundsMap.count(soundType) == 1)
  {
    if (_channelsMap[soundType] != nullptr)
    {
      FMOD_Channel_Stop(_channelsMap[soundType]);
    }

    FMOD_System_PlaySound(_soundSystem, _soundsMap[soundType], nullptr, true, &_channelsMap[soundType]);
    float volume = (float)Config::Get().GetValue("sound_volume") / 100.0f;
    FMOD_Channel_SetVolume(_channelsMap[soundType], volume);
    FMOD_Channel_SetPaused(_channelsMap[soundType], false);
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

  FMOD_System_PlaySound(_soundSystem, _musicList[musicIndex].Music, nullptr, true, &_musicChannel);
  float volume = (float)Config::Get().GetValue("music_volume") / 100.0f;
  FMOD_Channel_SetVolume(_musicChannel, volume);
  FMOD_Channel_SetPaused(_musicChannel, false);
}

void SoundSystem::PlayGameMusic()
{
  if (!_musicList.empty())
  {
    int index = Util::RandomNumber() % _musicList.size();
    if (index == 0) index++;
    PlayMusic(index);
  }
}
void SoundSystem::PlayTitleMusic()
{
  if (!_musicList.empty())
  {
    PlayMusic(Music::TITLE_MUSIC_ID);
  }
}

void SoundSystem::StopMusic()
{
  /*
  double dt = 0.0;
  float volume = 0.0f;
  FMOD_Channel_GetVolume(_musicChannel, &volume);
  while (dt < 1000.0)
  {
    dt += GameTime::Get().DeltaTimeMs();
    FMOD_Channel_SetVolume(_musicChannel, volume);

    volume -= 0.1f;

    FMOD_System_Update(_soundSystem);

    if (volume < 0.0f) break;
  }
  */

  FMOD_Channel_Stop(_musicChannel);
}

void SoundSystem::StopAllSounds()
{
  for (int i = 0; i < _channelsMap.size(); i++)
  {
    FMOD_Channel_Stop(_channelsMap[i]);
  }
}
