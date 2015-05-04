#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include <fmod.h>
#include <fmod_errors.h>

#include "config.h"
#include "util.h"
#include "logger.h"
#include "gametime.h"

struct MusicData
{
  unsigned int LoopStart;
  unsigned int LoopEnd;
  FMOD_SOUND* Music;
};

// Wrapper class for FMOD with ability to play given sound and music
class SoundSystem
{
  public:
    static SoundSystem& Get()
    {
      static SoundSystem instance;
      return instance;
    }
    void Init();
    void PlaySound(int soundType);
    void PlayMusic(int musicIndex);
    void SetMusicVolume(float volume) { FMOD_Channel_SetVolume(_musicChannel, volume); }
    void PlayTitleMusic();
    void PlayGameMusic();
    void StopMusic();
    void StopAllSounds();
    void Update() { FMOD_System_Update(_soundSystem); }
  protected:
  private:
    SoundSystem();
    virtual ~SoundSystem();
    SoundSystem& operator=(const SoundSystem&);
    SoundSystem(const SoundSystem&);

    FMOD_SYSTEM* _soundSystem;

    void LoadSounds();

    std::map<int, FMOD_SOUND*> _soundsMap;
    std::map<int, FMOD_CHANNEL*> _channelsMap;

    std::vector<MusicData> _musicList;
    FMOD_CHANNEL* _musicChannel;

    std::vector<FMOD_SOUND*> _sounds;
    std::vector<FMOD_CHANNEL*> _channels;

    float _maxSoundVolume;
    float _maxMusicVolume;

    int _lastPlayedMusicId;
};

#endif // SOUNDSYSTEM_H
