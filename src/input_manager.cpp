#include "input_manager.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

// FIXME: грязный хак игнорирует эксепшн, т.к. на первом входе, когда ещё ничего не нажато, в словаре
// ничего нет. Каноничный вызов _keys.at(key) покрешит приложение.
unsigned int InputManager::GetKeyState(SDL_Keycode key)
{
  return _keys[key];
}

void InputManager::PollEvents()
{
  _eventsPending = SDL_PollEvent(&_event);

  if (_event.type == SDL_KEYDOWN || _event.type == SDL_KEYUP)
  {
    _keys[_event.key.keysym.sym] = _event.type;
  }
}
