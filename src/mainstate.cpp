#include "mainstate.h"

MainState::MainState()
{
  //ctor
}

MainState::~MainState()
{
  //dtor
}

void MainState::Init()
{
}

void MainState::Cleanup()
{
}

void MainState::Pause()
{
}

void MainState::Resume()
{
}

void MainState::HandleEvents(Application* game)
{
  SDL_PumpEvents();

  _keyboardState = (Uint8*)SDL_GetKeyboardState(nullptr);

  if (_keyboardState[SDL_SCANCODE_ESCAPE])
  {
    game->SetRunningFlag(false);
  }
}

void MainState::Update(Application* game)
{
}

void MainState::Draw(Application* game)
{
}
