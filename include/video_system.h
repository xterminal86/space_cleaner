#ifndef VIDEOSYSTEM_H
#define VIDEOSYSTEM_H

#include "globals.h"

class VideoSystem
{
  public:
    static VideoSystem& Get()
    {
      static VideoSystem instance;
      return instance;
    }
    int Init(int w, int h);
    SDL_Renderer* Renderer() { return _renderer; }
    SDL_DisplayMode& DisplayMode() { return _displayMode; }
    SDL_Point& ScreenDimensions() { return _screenDimensions; }
  protected:
  private:
    SDL_Point _screenDimensions;
    bool _initialized;
    SDL_Renderer* _renderer;
    SDL_Window* _window;
    SDL_DisplayMode _displayMode;

    VideoSystem();
    ~VideoSystem();
    VideoSystem(const VideoSystem&);
    VideoSystem& operator= (const VideoSystem&);
};

#endif // VIDEOSYSTEM_H
