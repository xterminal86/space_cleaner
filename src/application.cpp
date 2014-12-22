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

  SDL_Rect colliders[5];

  colliders[0].x = 83;
  colliders[0].y = 12;
  colliders[0].w = 26;
  colliders[0].h = 38;

  colliders[1].x = 59;
  colliders[1].y = 50;
  colliders[1].w = 74;
  colliders[1].h = 24;

  colliders[2].x = 64;
  colliders[2].y = 74;
  colliders[2].w = 64;
  colliders[2].h = 8;

  colliders[3].x = 58;
  colliders[3].y = 82;
  colliders[3].w = 76;
  colliders[3].h = 80;

  colliders[4].x = 19;
  colliders[4].y = 102;
  colliders[4].w = 154;
  colliders[4].h = 40;


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
    SDL_RenderDrawRects(renderer, colliders, 5);
    SDL_RenderPresent(renderer);
  }
}
