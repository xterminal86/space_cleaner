#include "gamestate.h"

GameState::GameState()
{
  //ctor
}

GameState::~GameState()
{
  //dtor
}

void GameState::ChangeState(Application* game, GameState* newState)
{
  game->ChangeState(newState);
}
