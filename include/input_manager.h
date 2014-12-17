#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "globals.h"

class InputManager
{
  public:
    static InputManager& Get()
    {
      static InputManager instance;
      return instance;
    }
    unsigned int GetKeyState(SDL_Keycode key);
    SDL_Event* GetEvent() { return &_event; }
    void PollEvents();
  protected:
  private:
    InputManager();
    InputManager(const InputManager&);
    ~InputManager();
    InputManager& operator= (const InputManager&);

    SDL_Event _event;

    std::map<SDL_Keycode, unsigned int> _keys;
};

#endif // INPUTMANAGER_H
