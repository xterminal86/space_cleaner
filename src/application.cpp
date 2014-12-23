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

  SDL_Rect dst;

  dst.x = 0;
  dst.y = 0;
  dst.w = TextureManager::Get().GetTextureWrapper(0)->Width();
  dst.h = TextureManager::Get().GetTextureWrapper(0)->Height();

  std::vector<SDL_Point>* collider = TextureManager::Get().GetCollider(0);

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
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    //SDL_RenderCopy(renderer, TextureManager::Get().GetSDLTexture(0), nullptr, &dst);
    SDL_RenderCopyEx(renderer, TextureManager::Get().GetSDLTexture(0), nullptr, &dst, angle, nullptr, SDL_FLIP_NONE);
    //SDL_RenderDrawPoints(renderer, points, numPoints);
    SDL_RenderDrawLines(renderer, collider->data(), collider->size());
    SDL_RenderPresent(renderer);
  }
}
