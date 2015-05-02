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
    int Init(int w, int h, int fullscreen = 0);
    SDL_Renderer* Renderer() { return _renderer; }
    SDL_DisplayMode& DisplayMode() { return _displayMode; }
    SDL_Point& ScreenDimensions()
    {
      // If SDL_WINDOW_FULLSCREEN_DESKTOP is enabled,
      // then window size in Init() is ignored (no video mode change is made)
      // and application mimics fullscreen by stretching to the current desktop resolution (from SDL wiki)
      //if (_fullscreen != 0)
      //{
        _screenDimensions.x = _displayMode.w;
        _screenDimensions.y = _displayMode.h;
      //}

      return _screenDimensions;
    }
  protected:
  private:
    SDL_Point _screenDimensions;
    bool _initialized;
    int _fullscreen;
    SDL_Renderer* _renderer;
    SDL_Window* _window;
    SDL_DisplayMode _displayMode;

    std::string _buildVersion;

    VideoSystem();
    ~VideoSystem();
    VideoSystem(const VideoSystem&);
    VideoSystem& operator= (const VideoSystem&);
};

#endif // VIDEOSYSTEM_H
