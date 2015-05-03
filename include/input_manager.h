#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "globals.h"

// Reads input
class InputManager
{
  public:
    static InputManager& Get()
    {
      static InputManager instance;
      return instance;
    }
    bool EventsPending() { return _eventsPending == 1 ? true : false; }
    SDL_Event& PollEvents();
  protected:
  private:
    InputManager();
    InputManager(const InputManager&);
    ~InputManager();
    InputManager& operator= (const InputManager&);

    SDL_Event _event;
    int _eventsPending;
};

#endif // INPUTMANAGER_H
