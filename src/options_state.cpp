#include "options_state.h"

OptionsState::OptionsState()
{
  //ctor
}

OptionsState::~OptionsState()
{
  //dtor
}

void OptionsState::Init(Application* game)
{
}

void OptionsState::Cleanup()
{
}

void OptionsState::Pause()
{
}

void OptionsState::Resume()
{
}

void OptionsState::HandleEvents(Application* game)
{
  SDL_PumpEvents();

  _keyboardState = (Uint8*)SDL_GetKeyboardState(nullptr);
}

void OptionsState::Update(Application* game)
{
}

void OptionsState::Draw(Application* game)
{
}
