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
  protected:
  private:
    bool _initialized;
    SDL_Renderer* _renderer;
    SDL_Window* _window;

    VideoSystem();
    ~VideoSystem();
    VideoSystem(const VideoSystem&);
    VideoSystem& operator= (const VideoSystem&);
};

#endif // VIDEOSYSTEM_H
