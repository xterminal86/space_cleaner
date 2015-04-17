#include "introstate.h"
#include "mainstate.h"

IntroState::IntroState()
{
  _logger->Init(GlobalStrings::LogFilename);
  _videoSystem->Init(_screenWidth, _screenHeight);
  _textureManager->Init(GlobalStrings::ImagesFilename, GlobalStrings::RelationFilename);
  _bitmapFont->Init();
  _soundSystem->Init();

  _screenSizeX = VideoSystem::Get().ScreenDimensions().x;
  _screenSizeY = VideoSystem::Get().ScreenDimensions().y;

  _spawnPoints.push_back(Vector2(_spawnSpread, _spawnSpread));
  _spawnPoints.push_back(Vector2(_screenSizeX - _spawnSpread, _spawnSpread));
  _spawnPoints.push_back(Vector2(_screenSizeX - _spawnSpread, _screenSizeY - _spawnSpread));
  _spawnPoints.push_back(Vector2(_spawnSpread, _screenSizeY - _spawnSpread));

  _menuStrings.push_back("NEW GAME");
  _menuStrings.push_back("OPTIONS");
  _menuStrings.push_back("HOW TO PLAY");
  _menuStrings.push_back("INSTRUCTIONS");
  _menuStrings.push_back("EXIT");

  _menuIndex = 0;

  _menuMap[0] = nullptr;
  _menuMap[1] = nullptr;
  _menuMap[2] = nullptr;
  _menuMap[3] = nullptr;
  _menuMap[4] = &IntroState::ExitGame;

  _keyPressed = false;

  LoadBackground();

  _backgroundX = _screenWidth / 2;
  _backgroundY = _screenHeight / 2;

  for (int i = 0; i < _asteroidsCount; i++)
  {
    int index = Util::RandomNumber() % _spawnPoints.size();
    int breakdown = Util::RandomNumber() % (GameMechanic::AsteroidMaxBreakdownLevel + 1);

    _asteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(_spawnPoints[index], breakdown, &_asteroids)));
  }

  _menuItemScaleFactor = _menuItemDefaultScale;
}

IntroState::~IntroState()
{
  //dtor
}

void IntroState::Init(Application* game)
{
  _version = game->BuildVersionString();
}

void IntroState::Cleanup()
{
}

void IntroState::Pause()
{
}

void IntroState::Resume()
{
}

void IntroState::HandleEvents(Application* game)
{
  SDL_PumpEvents();

  _keyboardState = (Uint8*)SDL_GetKeyboardState(nullptr);

  if (_keyboardState[SDL_SCANCODE_RETURN])
  {
    if (_menuIndex == 4)
    {
      game->SetRunningFlag(false);
    }

    if (_menuIndex == 0)
    {
      game->PushState(&MainState::Get());
    }
  }

  if (_keyboardState[SDL_SCANCODE_DOWN])
  {
    if (!_keyPressed)
    {
      _keyPressed = true;
      _menuIndex++;
      if (_menuIndex > _menuStrings.size() - 1) _menuIndex = _menuStrings.size() - 1;
      _menuItemScaleFactor = _menuItemDefaultScale;
    }
  }

  if (_keyboardState[SDL_SCANCODE_UP])
  {
    if (!_keyPressed)
    {
      _keyPressed = true;
      _menuIndex--;
      if (_menuIndex < 0) _menuIndex = 0;
      _menuItemScaleFactor = _menuItemDefaultScale;
    }
  }

  if ( (!_keyboardState[SDL_SCANCODE_DOWN] && !_keyboardState[SDL_SCANCODE_UP]) )
  {
    _keyPressed = false;
  }
}

void IntroState::Update(Application* game)
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

void IntroState::Draw(Application* game)
{
  SDL_Renderer* renderer = VideoSystem::Get().Renderer();

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  DrawBackground();
  DrawAsteroids();

  _bitmapFont->SetTextColor(255, 255, 0, 255);
  _bitmapFont->SetScale(8.0);
  _bitmapFont->Printf(_screenSizeX / 2,
                      100,
                      BitmapFont::AlignCenter,
                      "SPACE CLEANER");

  DrawMenu();

  _bitmapFont->SetTextColor(255, 255, 255, 255);
  _bitmapFont->SetScale(1.0);
  _bitmapFont->Printf(_screenSizeX / 2,
                      _screenSizeY - _bitmapFont->LetterWidth * _bitmapFont->ScaleFactor(),
                      BitmapFont::AlignCenter,
                      "(c) 2015 by xterminal86");

  _bitmapFont->SetTextColor(255, 255, 255, 255);
  _bitmapFont->SetScale(1.0);
  _bitmapFont->Printf(_screenSizeX,
                      _screenSizeY - _bitmapFont->LetterWidth * _bitmapFont->ScaleFactor(),
                      BitmapFont::AlignRight,
                      (char*)_version.data());

  SDL_RenderPresent(renderer);
}

// ==================== Private Methods =================== //

void IntroState::LoadBackground()
{
  int textureId = TextureManager::Get().FindTextureByRole(GlobalStrings::BackgroundStarRole);
  if (textureId != -1)
  {
    _backgroundStar.Init(textureId);
  }

  for (int i = 0; i < _backgroundStars; i++)
  {
    Star s;

    s.Init(_backgroundStar.ImageWrapper());

    _stars.push_back(s);
  }

  textureId = TextureManager::Get().FindTextureByRole(GlobalStrings::BackgroundRole);
  if (textureId != -1)
  {
    _background.Init(textureId);
  }
}

void IntroState::DrawBackground()
{
  _background.Draw(_backgroundX, _backgroundY);

  for (auto& i : _stars)
  {
    i.Pulse();
  }
}

void IntroState::DrawAsteroids()
{
  for (auto& asteroid : _asteroids)
  {
    asteroid.get()->Compute();
    asteroid.get()->Draw(false, false);
  }
}

void IntroState::DrawMenu()
{
  for (int i = 0; i < _menuStrings.size(); i++)
  {
    if (_menuIndex == i)
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

void IntroState::ExitGame()
{
}
