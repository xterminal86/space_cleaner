#include "introstate.h"
#include "mainstate.h"

IntroState::IntroState()
{
  _stateId = GameStatesIds::IntroState;

  _logger->Init(GlobalStrings::LogFilename);
  _config->Init();
  _videoSystem->Init(_config->GetValue(ConfigStrings::ScreenWidthString),
                     _config->GetValue(ConfigStrings::ScreenHeightString),
                     _config->GetValue(ConfigStrings::FullscreenString));
  _textureManager->Init(GlobalStrings::ImagesFilename, GlobalStrings::RelationFilename, false);
  _bitmapFont->Init();
  DrawLoadingText("Initializing animations...");
  _animationsManager->Init(false);
  DrawLoadingText("Initializing powerups...");
  _powerupsManager->Init();
  DrawLoadingText("Initializing sound system...");
  _soundSystem->Init(false);
  DrawLoadingText("Initializing forms manager...");
  _formsManager->Init();
  DrawLoadingText("Preparing main title screen...");

  _screenSizeX = VideoSystem::Get().ScreenDimensions().x;
  _screenSizeY = VideoSystem::Get().ScreenDimensions().y;

  _spawnPoints.push_back(Vector2(_spawnSpread, _spawnSpread));
  _spawnPoints.push_back(Vector2(_screenSizeX - _spawnSpread, _spawnSpread));
  _spawnPoints.push_back(Vector2(_screenSizeX - _spawnSpread, _screenSizeY - _spawnSpread));
  _spawnPoints.push_back(Vector2(_spawnSpread, _screenSizeY - _spawnSpread));

  _keyPressed = false;

  LoadBackground();

  _backgroundX = VideoSystem::Get().ScreenDimensions().x / 2;
  _backgroundY = VideoSystem::Get().ScreenDimensions().y / 2;

  for (int i = 0; i < _asteroidsCount; i++)
  {
    int index = Util::RandomNumber() % _spawnPoints.size();
    int breakdown = Util::RandomNumber() % (GameMechanic::AsteroidMaxBreakdownLevel + 1);

    _asteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(_spawnPoints[index], breakdown, &_asteroids)));
  }
}

IntroState::~IntroState()
{
  //dtor
}

void IntroState::Init(Application* game)
{
  _gameRef = game;
  _formsManager->SetApplicationPointer(game);
  SoundSystem::Get().PlayTitleMusic();
}

void IntroState::Cleanup()
{
  SoundSystem::Get().StopMusic();
  SoundSystem::Get().StopAllSounds();
}

void IntroState::Pause()
{
}

void IntroState::Resume()
{
  SoundSystem::Get().PlayTitleMusic();
}

void IntroState::HandleEvents(Application* game)
{
  SDL_PumpEvents();

  _keyboardState = (Uint8*)SDL_GetKeyboardState(nullptr);

  _formsManager->FormHandleInput(_keyboardState);

  if (_keyboardState[SDL_SCANCODE_ESCAPE] && !_keyPressed)
  {
    _keyPressed = true;
    FormsManager::Get().CloseCurrentForm();
  }

  if (!_keyboardState[SDL_SCANCODE_ESCAPE])
  {
    _keyPressed = false;
  }
}

void IntroState::Update(Application* game)
{
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

  _formsManager->FormDraw();

  _bitmapFont->SetTextColor(255, 255, 255, 255);
  _bitmapFont->SetScale(1.0);
  _bitmapFont->Printf(_screenSizeX / 2,
                      _screenSizeY - _bitmapFont->LetterWidth * _bitmapFont->ScaleFactor(),
                      BitmapFont::AlignCenter,
                      "(c) 2015 by xterminal86");

  _bitmapFont->SetTextColor(255, 255, 255, 255);
  _bitmapFont->SetScale(1.0);
  _bitmapFont->Printf(VideoSystem::Get().ScreenDimensions().x,
                      VideoSystem::Get().ScreenDimensions().y - _bitmapFont->LetterWidth * _bitmapFont->ScaleFactor(),
                      BitmapFont::AlignRight,
                      (char*)_gameRef->BuildVersionString().data());

  /*
  // Visual Debug: center of screen

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderDrawLine(renderer, _screenSizeX / 2, 0, _screenSizeX / 2, _screenSizeY);
  SDL_RenderDrawLine(renderer, 0, _screenSizeY / 2, _screenSizeX, _screenSizeY / 2);
  */

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

void IntroState::DrawLoadingText(std::string text)
{
  SDL_Renderer* renderer = VideoSystem::Get().Renderer();

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  _bitmapFont->SetScale(1.0);
  _bitmapFont->SetTextColor(255, 255, 255);
  _bitmapFont->Printf(_videoSystem->ScreenDimensions().x / 2, _videoSystem->ScreenDimensions().y / 2, _bitmapFont->AlignCenter, "%s", (char*)text.c_str());

  SDL_RenderPresent(renderer);
}
