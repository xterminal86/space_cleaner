#include "input_manager.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

SDL_Event& InputManager::PollEvents()
{
  _eventsPending = SDL_PollEvent(&_event);

  return _event;
}
