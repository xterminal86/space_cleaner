#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "application.h"

class GameState
{
public:
  virtual void Init() = 0;
  virtual void Cleanup() = 0;

  virtual void Pause() = 0;
  virtual void Resume() = 0;

  virtual void HandleEvents(Application* game) = 0;
  virtual void Update(Application* game) = 0;
  virtual void Draw(Application* game) = 0;

  void ChangeState(Application* game, GameState* state);

  protected:
    GameState();
    virtual ~GameState();
};

#endif // GAMESTATE_H
