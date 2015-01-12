#include "application.h"

Application::Application()
{
  _logger->Init(GlobalStrings::LogFilename);
  _videoSystem->Init(_screenWidth, _screenHeight);
  _textureManager->Init(GlobalStrings::ImagesFilename, GlobalStrings::RelationFilename);
}

Application::~Application()
{
  Logger::Get().LogPrint("Closing application...\n");
}

void Application::LoadBackground()
{
  std::map<int, std::string>& db = TextureManager::Get().Relation();

  for (int i = 0; i < db.size(); i++)
  {
    if (db[i] == GlobalStrings::BackgroundRole)
    {
      _background.Init(i);
      break;
    }
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

  SDL_Event event;
  while (_running)
  {
    // TODO:
    // Допилить InputManager, чтобы можно было юзать как зажатые кнопки, так и одиночные.
    InputManager::Get().PollEvents();

    if (InputManager::Get().GetKeyState(SDLK_ESCAPE) == SDL_KEYDOWN)
    {
      _running = false;
    }

    if (InputManager::Get().GetKeyState(SDLK_a) == SDL_KEYDOWN)
    {
      angle--;
    }

    if (InputManager::Get().GetKeyState(SDLK_d) == SDL_KEYDOWN)
    {
      angle++;
    }

    if (InputManager::Get().GetKeyState(SDLK_w) == SDL_KEYDOWN)
    {
      ship.Accelerate(0.1);
    }

    if (InputManager::Get().GetKeyState(SDLK_s) == SDL_KEYDOWN)
    {
      ship.Accelerate(-0.1);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    _background.Draw(bgx, bgy);

    ship.Rotate(angle);
    ship.Draw(true);

    SDL_RenderPresent(renderer);
  }
}
