#include "form_options.h"

int FormOptions::_musicVolume = 0;
int FormOptions::_soundVolume = 0;
float FormOptions::_musicVolumePercent = 0.0f;
float FormOptions::_soundVolumePercent = 0.0f;
int FormOptions::_currentMusicJukebox = 0;
int FormOptions::_totalMusic = 0;
int FormOptions::_currentVideoDriverIndex = 0;

FormOptions::FormOptions()
{
  _musicVolume = Config::Get().GetValue(ConfigStrings::MusicVolumeString);
  _soundVolume = Config::Get().GetValue(ConfigStrings::SoundVolumeString);

  _musicVolumePercent = (float)_musicVolume * 100.0f / (float)Music::MaxMusicVolume;
  _soundVolumePercent = (float)_soundVolume * 100.0f / (float)Sounds::MaxSoundVolume;

  std::ostringstream str;
  str << "Music volume: " << _musicVolumePercent;

  MenuItem music;
  music.Id = 0;
  music.TextValue = str.str();
  music.LeftHandlerAuto = FormOptions::MusicLeftHandler;
  music.RightHandlerAuto = FormOptions::MusicRightHandler;

  _menuItems[music.Id] = music;

  str.str("");
  str << "Sound volume: " << (int)_soundVolumePercent;

  MenuItem sound;
  sound.Id = 1;
  sound.TextValue = str.str();
  sound.LeftHandlerAuto = FormOptions::SoundLeftHandler;
  sound.RightHandlerAuto = FormOptions::SoundRightHandler;

  _menuItems[sound.Id] = sound;

  _currentVideoDriverIndex = VideoSystem::Get().CurrentRenderDriverIndex();

  str.str("");
  str << "Video driver: " << VideoSystem::Get().GetCurrentRenderDriverInfo().name;

  MenuItem renderDriver;
  renderDriver.Id = 2;
  renderDriver.TextValue = str.str();
  renderDriver.LeftHandler = FormOptions::VideoDriverLeftHandler;
  renderDriver.RightHandler = FormOptions::VideoDriverRightHandler;

  _menuItems[renderDriver.Id] = renderDriver;

  _totalMusic = SoundSystem::Get().TotalMusic();

  str.str("");
  str << "Music test: " << _currentMusicJukebox;

  MenuItem jukebox;
  jukebox.Id = 3;
  jukebox.TextValue = str.str();
  jukebox.LeftHandler = FormOptions::JukeboxLeftHandler;
  jukebox.RightHandler = FormOptions::JukeboxRightHandler;
  jukebox.SelectHandler = FormOptions::JukeboxSelectHandler;

  _menuItems[jukebox.Id] = jukebox;

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

  if (keyboardState[SDL_SCANCODE_LEFT] && !_keyPressed)
  {
    _keyPressed = true;
    if (_menuItems[_currentMenuSelection].LeftHandler != nullptr)
    {
      _menuItems[_currentMenuSelection].LeftHandler();
    }
  }

  if (keyboardState[SDL_SCANCODE_RIGHT] && !_keyPressed)
  {
    _keyPressed = true;
    if (_menuItems[_currentMenuSelection].RightHandler != nullptr)
    {
      _menuItems[_currentMenuSelection].RightHandler();
    }
  }

  if (keyboardState[SDL_SCANCODE_LEFT] && _autoPress)
  {
    if (_menuItems[_currentMenuSelection].LeftHandlerAuto != nullptr)
    {
      _menuItems[_currentMenuSelection].LeftHandlerAuto();
    }
  }

  if (keyboardState[SDL_SCANCODE_RIGHT] && _autoPress)
  {
    if (_menuItems[_currentMenuSelection].RightHandlerAuto != nullptr)
    {
      _menuItems[_currentMenuSelection].RightHandlerAuto();
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

  if (keyboardState[SDL_SCANCODE_RETURN] && !_keyPressed)
  {
    _keyPressed = true;
    if (_menuItems[_currentMenuSelection].SelectHandler != nullptr)
    {
      _menuItems[_currentMenuSelection].SelectHandler();
    }
  }

  if (!keyboardState[SDL_SCANCODE_DOWN] && !keyboardState[SDL_SCANCODE_UP] && !keyboardState[SDL_SCANCODE_RETURN]
   && !keyboardState[SDL_SCANCODE_LEFT] && !keyboardState[SDL_SCANCODE_RIGHT])
  {
    _keyPressed = false;
  }
}

void FormOptions::Open()
{
  _currentMenuSelection = 0;
}

void FormOptions::Close()
{
  Config::Get().WriteConfig();
}

void FormOptions::UpdateTextValues()
{
  int musicVolume = Config::Get().GetValue(ConfigStrings::MusicVolumeString);
  int soundVolume = Config::Get().GetValue(ConfigStrings::SoundVolumeString);

  _musicVolumePercent = (float)_musicVolume * 100.0f / (float)Music::MaxMusicVolume;
  _soundVolumePercent = (float)_soundVolume * 100.0f / (float)Sounds::MaxSoundVolume;

  std::ostringstream str;
  str << "Music volume: " << _musicVolumePercent;

  _menuItems[0].TextValue = str.str();

  str.str("");
  str << "Sound volume: " << (int)_soundVolumePercent;

  _menuItems[1].TextValue = str.str();

  str.str("");
  str << "Video driver: " << VideoSystem::Get().GetRenderDriverInfo(_currentVideoDriverIndex).name;

  _menuItems[2].TextValue = str.str();

  str.str("");
  str << "Music test: " << _currentMusicJukebox;

  _menuItems[3].TextValue = str.str();
}
