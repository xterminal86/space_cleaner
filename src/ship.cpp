#include "ship.h"

Ship::Ship(double posx, double posy)
{
  _angle = 0.0;
  _speed = 0.0;

  _position.Set(posx, posy);
  _direction.Set(0.0, 1.0);

  _shipSprite.Init(0);

  for (int i = 0; i < _shipSprite.OriginalCollider()->size(); i++)
  {
    _localCollider.push_back(_shipSprite.OriginalCollider()->at(i));
  }

  _colliderCenter.x = _shipSprite.ImageWrapper()->Width() / 2;
  _colliderCenter.y = _shipSprite.ImageWrapper()->Height() / 2;
}

Ship::~Ship()
{
  //dtor
}

void Ship::MoveCollider(int x, int y)
{
  for (int i = 0; i < _localCollider.size(); i++)
  {
    _localCollider[i].x += x;
    _localCollider[i].y += y;
  }
}

void Ship::Draw(int x, int y, bool drawCollider)
{
  _shipSprite.Draw(x, y, _angle);
  if (drawCollider)
  {
    MoveCollider(x, y);
    SDL_SetRenderDrawColor(VideoSystem::Get().Renderer(), 255, 255, 0, 255);
    SDL_RenderDrawLines(VideoSystem::Get().Renderer(), _localCollider.data(), _localCollider.size());
  }
}

void Ship::Draw(bool drawCollider)
{
  _shipSprite.Draw(_position.X(), _position.Y(), _angle);
  if (drawCollider)
  {
    MoveCollider(_position.X(), _position.Y());
    SDL_SetRenderDrawColor(VideoSystem::Get().Renderer(), 255, 255, 0, 255);
    SDL_RenderDrawLines(VideoSystem::Get().Renderer(), _localCollider.data(), _localCollider.size());
  }
}

void Ship::Rotate(double angle)
{
  _angle = angle;

  for (int i = 0; i < _localCollider.size(); i++)
  {
    // By default rotation is assumed around (0; 0). It is located in the top left corner of the image.
    // Our actual rotation center is image's (w / 2; h / 2). So, first, we move it to (0; 0) (which means, that collider's coordinates are "offseted"),
    // then rotate collider around it, and then move it back ("offset" collider's coordinates back).
    double nx = _colliderCenter.x + (_shipSprite.OriginalCollider()->at(i).x - _colliderCenter.x) * SDL_cos(angle * PIOVER180) - (_shipSprite.OriginalCollider()->at(i).y - _colliderCenter.y) * SDL_sin(angle * PIOVER180);
    double ny = _colliderCenter.y + (_shipSprite.OriginalCollider()->at(i).x - _colliderCenter.x) * SDL_sin(angle * PIOVER180) + (_shipSprite.OriginalCollider()->at(i).y - _colliderCenter.y) * SDL_cos(angle * PIOVER180);
    _localCollider[i].x = (int)nx;
    _localCollider[i].y = (int)ny;
  }
}

void Ship::Accelerate(double dspeed)
{
  _speed += dspeed;

  if (_speed < 0.0)
  {
    _speed = 0.0;
  }
}
