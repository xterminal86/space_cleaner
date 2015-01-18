#ifndef GAMETIME_H
#define GAMETIME_H

#include "globals.h"

class GameTime
{
  public:
    static GameTime& Get()
    {
      static GameTime instance;
      return instance;
    }
    void MeasureBefore()
    {
      _lastTime = SDL_GetTicks();
    }
    void MeasureAfter()
    {
      _currentTime = SDL_GetTicks();
      _deltaTime = (double)(_currentTime - _lastTime);
      _lastTime = _currentTime;
    }
    double DeltaTime()
    {
      return _deltaTime;
    }
  protected:
  private:
    GameTime()
    {
      _currentTime = SDL_GetTicks();
      _lastTime = _currentTime;
    }
    ~GameTime()
    {
    }

    GameTime(const GameTime&);
    GameTime& operator=(const GameTime&);

    double _deltaTime;

    unsigned int _currentTime;
    unsigned int _lastTime;
};

#endif // GAMETIME_H
