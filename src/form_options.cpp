#include "form_options.h"

int FormOptions::_musicVolume = 0;
int FormOptions::_soundVolume = 0;
float FormOptions::_musicVolumePercent = 0.0f;
float FormOptions::_soundVolumePercent = 0.0f;

FormOptions::FormOptions()
{
  _musicVolume = Config::Get().GetValue("music_volume");
  _soundVolume = Config::Get().GetValue("sound_volume");

  _musicVolumePercent = (float)_musicVolume * 100.0f / (float)Music::MaxMusicVolume;
  _soundVolumePercent = (float)_soundVolume * 100.0f / (float)Sounds::MaxSoundVolume;

  std::ostringstream str;
  str << "Music volume: " << _musicVolumePercent;

  MenuItem music;
  music.Id = 0;
  music.TextValue = str.str();
  music.LeftHandler = FormOptions::MusicLeftHandler;
  music.RightHandler = FormOptions::MusicRightHandler;

  _menuItems[0] = music;

  str.str("");
  str << "Sound volume: " << (int)_soundVolumePercent;

  MenuItem sound;
  sound.Id = 1;
  sound.TextValue = str.str();
  sound.LeftHandler = FormOptions::SoundLeftHandler;
  sound.RightHandler = FormOptions::SoundRightHandler;

  _menuItems[1] = sound;

  _autoPress = false;
}

FormOptions::~FormOptions()
{
}

void FormOptions::Draw()
{
  UpdateTextValues();

  for (int i = 0; i < _menuItems.size(); i++)
  {
    _bitmapFont->SetScale(_menuItemDefaultScale);

    if (_currentMenuSelection == i)
    {
      _bitmapFont->SetTextColor(255, 0, 255, 255);
    }
    else
    {
      _bitmapFont->SetTextColor(255, 255, 255, 255);
    }

    _bitmapFont->Printf(VideoSystem::Get().ScreenDimensions().x / 2,
                        VideoSystem::Get().ScreenDimensions().y / 2 + i * 30,
                        BitmapFont::AlignCenter,
                        (char*)_menuItems[i].TextValue.data());
  }
}

void FormOptions::HandleInput(Uint8* keyboardState)
{
  if (keyboardState[SDL_SCANCODE_DOWN] && !_keyPressed)
  {
    _keyPressed = true;

    if (_currentMenuSelection < _menuItems.size() - 1)
    {
      SoundSystem::Get().PlaySound(Sounds::MENU_MOVE);
      _currentMenuSelection++;
    }
  }

  if (keyboardState[SDL_SCANCODE_UP] && !_keyPressed)
  {
    _keyPressed = true;

    if (_currentMenuSelection > 0)
    {
      SoundSystem::Get().PlaySound(Sounds::MENU_MOVE);
      _currentMenuSelection--;
    }
  }

  if (keyboardState[SDL_SCANCODE_LEFT] && _autoPress)
  {
    if (_menuItems[_currentMenuSelection].LeftHandler != nullptr)
    {
      _menuItems[_currentMenuSelection].LeftHandler();
    }
  }

  if (keyboardState[SDL_SCANCODE_RIGHT] && _autoPress)
  {
    if (_menuItems[_currentMenuSelection].RightHandler != nullptr)
    {
      _menuItems[_currentMenuSelection].RightHandler();
    }
  }

  if (keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_RIGHT])
  {
    _autoPressTimePassed += GameTime::Get().DeltaTimeMs();

    _autoPress = (_autoPressTimePassed > AutoChangeRateMs);
    if (_autoPress)
    {
      _autoPressTimePassed = 0;
    }
  }

  if (!keyboardState[SDL_SCANCODE_DOWN] && !keyboardState[SDL_SCANCODE_UP])
  {
    _keyPressed = false;
  }
}

void FormOptions::Open()
{
}

void FormOptions::Close()
{
  Config::Get().WriteConfig();
}

void FormOptions::UpdateTextValues()
{
  int musicVolume = Config::Get().GetValue("music_volume");
  int soundVolume = Config::Get().GetValue("sound_volume");

  _musicVolumePercent = (float)_musicVolume * 100.0f / (float)Music::MaxMusicVolume;
  _soundVolumePercent = (float)_soundVolume * 100.0f / (float)Sounds::MaxSoundVolume;

  std::ostringstream str;
  str << "Music volume: " << _musicVolumePercent;

  _menuItems[0].TextValue = str.str();

  str.str("");
  str << "Sound volume: " << (int)_soundVolumePercent;

  _menuItems[1].TextValue = str.str();
}
