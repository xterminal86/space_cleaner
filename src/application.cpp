#include "application.h"

Application::Application()
{
  _logger->Init(GlobalStrings::LogFilename);
  _videoSystem->Init(_screenWidth, _screenHeight);
  _textureManager->Init(GlobalStrings::ImagesFilename, GlobalStrings::RelationFilename);

  _deltaTime = 0.0;
}

Application::~Application()
{
  Logger::Get().LogPrint("Closing application...\n");
}

void Application::LoadBackground()
{
  int textureId = TextureManager::Get().FindTextureByRole(GlobalStrings::BackgroundRole);
  if (textureId != -1)
  {
    _background.Init(textureId);
  }
}

void Application::Start()
{
  double angle = 0.0;

  _running = true;

  SDL_Renderer* renderer = VideoSystem::Get().Renderer();

  LoadBackground();

  Ship ship(0.0, 0.0);

  int bgx = _screenWidth / 2 - _background.ImageWrapper()->Width() / 2;
  int bgy = _screenHeight / 2 - _background.ImageWrapper()->Height() / 2;

  ship.Move(300, 300);

  _lastTime = SDL_GetTicks();

  SDL_Event event;
  while (_running)
  {
    _currentTime = SDL_GetTicks();

    // TODO:
    // Допилить InputManager, чтобы можно было юзать как зажатые кнопки, так и одиночные.
    InputManager::Get().PollEvents();

    if (InputManager::Get().GetKeyState(SDLK_ESCAPE) == SDL_KEYDOWN)
    {
      _running = false;
    }

    if (InputManager::Get().GetKeyState(SDLK_a) == SDL_KEYDOWN)
    {
      angle -= _rotateSpeed * _deltaTime;
    }

    if (InputManager::Get().GetKeyState(SDLK_d) == SDL_KEYDOWN)
    {
      angle += _rotateSpeed * _deltaTime;
    }

    if (InputManager::Get().GetKeyState(SDLK_w) == SDL_KEYDOWN)
    {
      ship.Accelerate(_accelerationSpeed * _deltaTime);
    }

//    if (InputManager::Get().GetKeyState(SDLK_s) == SDL_KEYDOWN)
//    {
//      ship.Accelerate(-_accelerationSpeed);
//    }

    if (InputManager::Get().GetKeyState(SDLK_w) == SDL_KEYUP && ship.Speed() > 0.0)
    {
      ship.Accelerate(-_accelerationSpeed * _deltaTime);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    _background.Draw(bgx, bgy);

    ship.Rotate(angle);
    ship.Move();

    ship.Draw(true);

    SDL_RenderPresent(renderer);

    _deltaTime = (double)(_currentTime - _lastTime) / 1000;

    _lastTime = _currentTime;
  }
}
