#include "application.h"

Application::Application()
{
  _logger->Init(_logFilename);
  _videoSystem->Init(_screenWidth, _screenHeight);
  _textureManager->Init(_imagesFilename);
}

Application::~Application()
{
  Logger::Get().LogPrint("Closing application...\n");
}

void Application::Start()
{
  double angle = 0.0;

  _running = true;

  SDL_Renderer* renderer = VideoSystem::Get().Renderer();

  Ship ship;

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

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    ship.Rotate(angle);
    ship.Draw(300, 300, true);

    SDL_RenderPresent(renderer);
  }
}
