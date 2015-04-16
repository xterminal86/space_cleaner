#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include <fmod.hpp>
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
  protected:
  private:
    SoundSystem();
    virtual ~SoundSystem();
    SoundSystem& operator=(const SoundSystem&);
    SoundSystem(const SoundSystem&);

    FMOD_SYSTEM* _soundSystem;
};

#endif // SOUNDSYSTEM_H
