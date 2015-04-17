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

  LoadBackground();

  _backgroundX = _screenWidth / 2;
  _backgroundY = _screenHeight / 2;

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

void IntroState::Init()
{
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
    game->PushState(&MainState::Get());
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
  DrawMenu();

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
  _bitmapFont->SetTextColor(255, 255, 0, 255);
  _bitmapFont->SetScale(8.0);
  _bitmapFont->Printf(_screenSizeX / 2,
                      _screenSizeY / 2 - _bitmapFont->LetterWidth * _bitmapFont->ScaleFactor(),
                      BitmapFont::AlignCenter,
                      "SPACE CLEANER");

  _bitmapFont->SetTextColor(255, 255, 255, 255);
  _bitmapFont->SetScale(1.0);
  _bitmapFont->Printf(_screenSizeX / 2,
                      _screenSizeY - _bitmapFont->LetterWidth * _bitmapFont->ScaleFactor(),
                      BitmapFont::AlignCenter,
                      "(c) 2015 by xterminal86");

}
