#include "form_main_title.h"

FormMainTitle::FormMainTitle()
{
  _menuStrings.push_back("NEW GAME");
  _menuStrings.push_back("OPTIONS");
  _menuStrings.push_back("HOW TO PLAY");
  _menuStrings.push_back("HIGH SCORES");
  _menuStrings.push_back("EXIT");

  _screenSizeX = VideoSystem::Get().ScreenDimensions().x;
  _screenSizeY = VideoSystem::Get().ScreenDimensions().y;

  _menuItemScaleFactor = _menuItemDefaultScale;

  for (int i = 0; i < _menuStrings.size(); i++)
  {
    MenuItem item;
    item.Id = i;
    item.TextValue = _menuStrings[i];
    _menuItems[i] = item;
  }

  _menuItems[0].SelectHandler = FormMainTitle::NewGameHandler;
  _menuItems[1].SelectHandler = FormMainTitle::OptionsHandler;
  _menuItems[2].SelectHandler = FormMainTitle::HowToPlayHandler;
  _menuItems[3].SelectHandler = FormMainTitle::HighScoresHandler;
  _menuItems[4].SelectHandler = FormMainTitle::ExitGameHandler;
}

FormMainTitle::~FormMainTitle()
{
  //dtor
}

void FormMainTitle::Open()
{
}

void FormMainTitle::Close()
{
}

void FormMainTitle::HandleInput(Uint8* keyboardState)
{
  if (keyboardState[SDL_SCANCODE_DOWN] && !_keyPressed)
  {
    _keyPressed = true;

    if (_currentMenuSelection < _menuStrings.size() - 1)
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

  if (keyboardState[SDL_SCANCODE_RETURN] && !_keyPressed)
  {
    _keyPressed = true;
    SoundSystem::Get().PlaySound(Sounds::MENU_SELECT);

    if (_menuItems[_currentMenuSelection].SelectHandler != nullptr);
    {
      _menuItems[_currentMenuSelection].SelectHandler();
    }
  }

  if (!keyboardState[SDL_SCANCODE_DOWN] && !keyboardState[SDL_SCANCODE_UP])
  {
    _keyPressed = false;
  }
}

void FormMainTitle::Draw()
{
  Update();

  PrintMenuText();
}

void FormMainTitle::Update()
{
  _menuItemScaleFactor += (_menuItemScaleIncrement * GameTime::Get().DeltaTimeMs());

  if (_menuItemScaleFactor < _menuItemMinimalScale)
  {
    _menuItemScaleFactor = _menuItemMinimalScale;
    _menuItemScaleIncrement = -_menuItemScaleIncrement;
  }

  if (_menuItemScaleFactor > _menuItemMaximumScale)
  {
    _menuItemScaleFactor = _menuItemMaximumScale;
    _menuItemScaleIncrement = -_menuItemScaleIncrement;
  }
}

void FormMainTitle::PrintMenuText()
{
  for (int i = 0; i < _menuStrings.size(); i++)
  {
    if (_currentMenuSelection == i)
    {
      _bitmapFont->SetScale(_menuItemScaleFactor);
      _bitmapFont->SetTextColor(255, 0, 255, 255);
    }
    else
    {
      _bitmapFont->SetScale(_menuItemDefaultScale);
      _bitmapFont->SetTextColor(255, 255, 255, 255);
    }

    _bitmapFont->Printf(_screenSizeX / 2,
                        _screenSizeY / 2 + i * 30,
                        BitmapFont::AlignCenter,
                        (char*)_menuStrings[i].data());
  }
}
