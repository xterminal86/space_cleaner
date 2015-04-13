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
      _lastTimeMs = SDL_GetTicks();
    }
    void MeasureAfter()
    {
      _currentTimeMs = SDL_GetTicks();
      _deltaTimeMs = (double)(_currentTimeMs - _lastTimeMs);
      _lastTimeMs = _currentTimeMs;
    }
    double DeltaTimeMs()
    {
      return _deltaTimeMs;
    }
  protected:
  private:
    GameTime()
    {
      _currentTimeMs = SDL_GetTicks();
      _lastTimeMs = _currentTimeMs;
    }
    ~GameTime()
    {
    }

    GameTime(const GameTime&);
    GameTime& operator=(const GameTime&);

    double _deltaTimeMs;

    unsigned int _currentTimeMs;
    unsigned int _lastTimeMs;
};

#endif // GAMETIME_H
