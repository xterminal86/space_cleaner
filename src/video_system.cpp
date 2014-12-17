#include "video_system.h"

VideoSystem::VideoSystem()
{
  _initialized = false;
  _renderer = nullptr;
  _window = nullptr;
}

VideoSystem::~VideoSystem()
{
  Logger::Get().LogPrint("Closing video system...\n");

  SDL_DestroyRenderer(_renderer);
  SDL_DestroyWindow(_window);
  IMG_Quit();
  SDL_Quit();
}

int VideoSystem::Init(int w, int h)
{
  if (!_initialized)
  {
    Logger::Get().LogPrint("Initializing video system...\n");

	  if(SDL_Init(SDL_INIT_VIDEO) < 0)
	  {
		  Logger::Get().LogPrint("!!!ERROR!!! SDL could not initialize!\nReason: %s\n", SDL_GetError());
		  exit(1);
	  }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			Logger::Get().LogPrint( "(warning) Linear texture filtering could not be enabled!");
		}

    _window = SDL_CreateWindow("Isometric Engine",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              w, h,
                              SDL_WINDOW_SHOWN);

	  if(_window == nullptr)
	  {
		  Logger::Get().LogPrint("Window could not be created!\nReason: %s\n", SDL_GetError());
		  exit(1);
	  }

    int numberOfRenderDrivers = SDL_GetNumRenderDrivers();
    bool success = false;
    for (int i = 0; i < numberOfRenderDrivers; i++)
    {
      SDL_RendererInfo info;
      SDL_GetRenderDriverInfo(i, &info);
      Logger::Get().LogPrint("%i. driver: %s\n", i + 1, info.name);
      Logger::Get().LogPrint("----| flags: %zu\n", info.flags);
      Logger::Get().LogPrint("Trying to create renderer...\n");
      _renderer = SDL_CreateRenderer(_window, i, info.flags);

      if (_renderer != nullptr)
      {
        Logger::Get().LogPrint("*** SUCCESS *** Created renderer: \"%s\"\n", info.name);
        success = true;
        break;
      }
    }

    if (!success)
    {
      Logger::Get().LogPrint("!!! ERROR !!! Failed to create renderer!\nReason: %s\n", SDL_GetError());
      exit(1);
    }

    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);

    int flags = IMG_INIT_PNG;
    int res = IMG_Init(flags);

    if ( (res & flags) != flags)
    {
      Logger::Get().LogPrint("!!! ERROR !!! IMG_Init: Failed to init required PNG support!\n");
      Logger::Get().LogPrint("Reason: %s\n", IMG_GetError());
      exit(1);
    }
    Logger::Get().LogPrint("Video System is ready...\n");
    _initialized = true;
  }
  else Logger::Get().LogPrint("(warning) Video system already initialized!\n");

  return 0;
}
