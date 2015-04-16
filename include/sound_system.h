#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include <fmod.h>
#include <fmod_errors.h>

#include "logger.h"

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

    std::vector<FMOD_SOUND*> _sounds;
    std::vector<FMOD_CHANNEL*> _channels;
};

#endif // SOUNDSYSTEM_H
