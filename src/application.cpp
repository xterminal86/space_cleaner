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

    _background.Draw(0, 0);

    ship.Rotate(angle);
    ship.Draw(300, 300, true);

    SDL_RenderPresent(renderer);
  }
}
