#include "sound_system.h"

SoundSystem::SoundSystem()
{
  _musicChannel = nullptr;
  _lastPlayedMusicId = -1;
}

SoundSystem::~SoundSystem()
{
  Logger::Get().LogPrint("Closing audio system...\n");
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

  //LoadSounds();

  _maxMusicVolume = (float)Music::MaxMusicVolume / 100.0f;
  _maxSoundVolume = (float)Sounds::MaxSoundVolume / 100.0f;
}

void SoundSystem::LoadSounds()
{
  LoadSoundsFromMemory();

  Logger::Get().LogPrint("Loading music list...\n");

  FILE* f = fopen(GlobalStrings::MusicListFilename.data(), "r");
  if (f == nullptr)
  {
    Logger::Get().LogPrint("(warning) Could not open file %s!\n", GlobalStrings::MusicListFilename.data());
    exit(1);
  }

  int index = 0;
  while (!feof(f))
  {
    char buf[512];

    MusicData entry;
    fscanf(f, "%s %i %i", buf, &entry.LoopStart, &entry.LoopEnd);
    Logger::Get().LogPrint("----|Track %i - %s\n", index, buf);
    entry.Filename = buf;
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
    if (_channelsMap[soundType] == nullptr) return;

    FMOD_Channel_Stop(_channelsMap[soundType]);

    FMOD_System_PlaySound(_soundSystem, _soundsMap[soundType], nullptr, true, &_channelsMap[soundType]);
    float volume = (float)Config::Get().GetValue(ConfigStrings::SoundVolumeString) / 100.0f;
    if (volume > _maxSoundVolume) volume = _maxSoundVolume;
    FMOD_Channel_SetVolume(_channelsMap[soundType], volume);
    FMOD_Channel_SetPaused(_channelsMap[soundType], false);
  }
}

void SoundSystem::StopSound(int soundType)
{
  if (_soundsMap.count(soundType) == 1)
  {
    if (_channelsMap[soundType] != nullptr)
    {
      FMOD_Channel_Stop(_channelsMap[soundType]);
    }
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
  float volume = (float)Config::Get().GetValue(ConfigStrings::MusicVolumeString) / 100.0f;
  if (volume > _maxMusicVolume) volume = _maxMusicVolume;
  FMOD_Channel_SetVolume(_musicChannel, volume);
  FMOD_Channel_SetPaused(_musicChannel, false);
}

void SoundSystem::PlayGameMusic()
{
  if (!_musicList.empty())
  {
    if (_lastPlayedMusicId == -1)
    {
      int index = Util::RandomNumber() % _musicList.size();
      if (index == Music::TITLE_MUSIC_ID) index++;
      _lastPlayedMusicId = index;
    }
    else
    {
      _lastPlayedMusicId++;
      if (_lastPlayedMusicId > _musicList.size() - 1)
      {
        _lastPlayedMusicId = 1;
      }
    }

    PlayMusic(_lastPlayedMusicId);
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

MusicData* SoundSystem::GetMusicEntry(int index)
{
  for (int i = 0; i < _musicList.size(); i++)
  {
    if (i == index)
    {
      return &_musicList[i];
    }
  }

  return nullptr;
}

void SoundSystem::LoadSoundsFromMemory()
{
  std::string soundList;

  auto fileBytes = Config::Get().GetFileFromMemory(GlobalStrings::SoundsFilename);
  if (fileBytes != nullptr)
  {
    for (int i = 0; i < fileBytes->size(); i++)
    {
      soundList.push_back(fileBytes->at(i));
    }
  }

  char buf[512];
  int index = 0;

  std::istringstream iss(soundList);
  while (iss.getline(buf, 512))
  {
    char filename[256];
    sscanf(buf, "%i %s", &index, filename);

    auto res = Config::Get().GetFileFromMemory(filename);

    if (res != nullptr)
    {
      FMOD_CREATESOUNDEXINFO soundInfo;
      soundInfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
      int fileSizeBytes = res->size();
      //long fileSizeBytes = Config::Get().GetFileFromMemorySize(filename);
      if (fileSizeBytes == -1) continue;
      soundInfo.length = fileSizeBytes;
      soundInfo.format = FMOD_SOUND_FORMAT_PCM16;
      soundInfo.numchannels = 2;
      soundInfo.defaultfrequency = 44100;

      FMOD_SOUND* p;
      FMOD_RESULT r = FMOD_System_CreateSound(_soundSystem, res->data(), FMOD_OPENMEMORY | FMOD_OPENRAW, &soundInfo, &p);

      Logger::Get().LogPrint ("%i\n", r);

      if (r == FMOD_OK)
      {
        _sounds.push_back(p);

        FMOD_CHANNEL* channel = nullptr;
        _channels.push_back(channel);

        _soundsMap[index] = _sounds[index];
        _channelsMap[index] = _channels[index];

        Logger::Get().LogPrint("----|_sounds[%i] (0x%zX) = %s\n", index, _sounds[index], buf);
      }
    }
  }

  /*
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
  */
}
