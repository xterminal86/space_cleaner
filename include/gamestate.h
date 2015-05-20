#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "application.h"

// Base class for managing game states (title screen and game itself)
class GameState
{
public:
  virtual void Init(Application* game) = 0;
  virtual void Cleanup() = 0;

  virtual void Pause() = 0;
  virtual void Resume() = 0;

  virtual void HandleEvents(Application* game) = 0;
  virtual void Update(Application* game) = 0;
  virtual void Draw(Application* game) = 0;

  virtual int StateId() { return _stateId; };

  protected:
    GameState();
    virtual ~GameState();

    int _stateId;
};

#endif // GAMESTATE_H
