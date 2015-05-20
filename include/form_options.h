#ifndef FORMOPTIONS_H
#define FORMOPTIONS_H

#include "form.h"
#include "sound_system.h"

class FormOptions : public Form
{
  public:
    FormOptions();
    virtual ~FormOptions();
  protected:
    void Draw() override;
    void HandleInput(Uint8* keyboardState) override;
    void Open() override;
    void Close() override;
  private:
    const double _menuItemDefaultScale = 2.0;
    const int AutoChangeRateMs = 50;

    static float _musicVolumePercent;
    static float _soundVolumePercent;

    static int _musicVolume;
    static int _soundVolume;
    static int _currentMusicJukebox;
    static int _totalMusic;
    static int _currentVideoDriverIndex;

    bool _autoPress;

    unsigned int _autoPressTimePassed;

    std::map<int, MenuItem> _menuItems;

    void UpdateTextValues();

    static void MusicLeftHandler()
    {
      _musicVolume--;
      if (_musicVolume < 0) _musicVolume = 0;

      Config::Get().SetValue(ConfigStrings::MusicVolumeString, _musicVolume);

      SoundSystem::Get().SetMusicVolume((float)_musicVolume / 100.0f);
    }

    static void MusicRightHandler()
    {
      _musicVolume++;
      if (_musicVolume > Music::MaxMusicVolume) _musicVolume = Music::MaxMusicVolume;

      Config::Get().SetValue(ConfigStrings::MusicVolumeString, _musicVolume);

      SoundSystem::Get().SetMusicVolume((float)_musicVolume / 100.0f);
    }

    static void SoundLeftHandler()
    {
      _soundVolume--;
      if (_soundVolume < 0) _soundVolume = 0;

      Config::Get().SetValue(ConfigStrings::SoundVolumeString, _soundVolume);
    }

    static void SoundRightHandler()
    {
      _soundVolume++;
      if (_soundVolume > Sounds::MaxSoundVolume) _soundVolume = Sounds::MaxSoundVolume;

      Config::Get().SetValue(ConfigStrings::SoundVolumeString, _soundVolume);
    }

    static void VideoDriverLeftHandler()
    {
      _currentVideoDriverIndex--;
      if (_currentVideoDriverIndex < 0) _currentVideoDriverIndex = 0;
      Config::Get().SetValue(ConfigStrings::VideoDriverString, _currentVideoDriverIndex);
    }

    static void VideoDriverRightHandler()
    {
      _currentVideoDriverIndex++;
      if (_currentVideoDriverIndex >= VideoSystem::Get().TotalVideoDrivers() - 1)
      {
        _currentVideoDriverIndex = VideoSystem::Get().TotalVideoDrivers() - 1;
      }
      Config::Get().SetValue(ConfigStrings::VideoDriverString, _currentVideoDriverIndex);
    }

    static void JukeboxRightHandler()
    {
      _currentMusicJukebox++;
      if (_currentMusicJukebox == _totalMusic) _currentMusicJukebox = 0;
    }

    static void JukeboxLeftHandler()
    {
      _currentMusicJukebox--;
      if (_currentMusicJukebox < 0) _currentMusicJukebox = _totalMusic - 1;
    }

    static void JukeboxSelectHandler()
    {
      SoundSystem::Get().PlayMusic(_currentMusicJukebox);
    }
};

#endif // FORMOPTIONS_H
