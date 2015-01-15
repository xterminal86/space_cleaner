#include "ship.h"

Ship::Ship(double posx, double posy)
{
  _angle = 0.0;
  _speed = 0.0;

  _position.Set(posx, posy);

  // We go up the screen when Y is decreasing.
  _originalDirection.Set(0.0, -1.0 * _directionResolution);
  _localDirection.Set(0.0, -1.0 * _directionResolution);

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

void Ship::Move(int x, int y)
{
  _position.Set(x, y);
}

void Ship::Move(Vector2 newPos)
{
  Move(newPos.X(), newPos.Y());
}

void Ship::Move()
{
  _position.Set(_position.X() + _localDirection.X() * _speed, _position.Y() + _localDirection.Y() * _speed);
}

void Ship::Draw(int x, int y, bool drawCollider)
{
  _shipSprite.Draw(x - _colliderCenter.x, y - _colliderCenter.y, _angle);

  if (drawCollider)
  {
    SDL_SetRenderDrawColor(VideoSystem::Get().Renderer(), 255, 255, 0, 255);
    SDL_RenderDrawLines(VideoSystem::Get().Renderer(), _localCollider.data(), _localCollider.size());
  }

  SDL_RenderDrawLine(VideoSystem::Get().Renderer(), _position.X(), _position.Y(),
                                                    _position.X() + (int)(_localDirection.X() * _directionResolution),
                                                    _position.Y() + (int)(_localDirection.Y() * _directionResolution));
}

void Ship::Draw(bool drawCollider)
{
  Draw((int)_position.X(), (int)_position.Y(), drawCollider);
}

void Ship::Rotate(double angle)
{
  _angle = angle;

  Vector2 center(_colliderCenter.x, _colliderCenter.y);

  Vector2 res;
  for (int i = 0; i < _localCollider.size(); i++)
  {
    Vector2 origCollider(_shipSprite.OriginalCollider()->at(i).x, _shipSprite.OriginalCollider()->at(i).y);
    Vector2::RotateVector(res, center, origCollider, angle);

    _localCollider[i].x = res.X();
    _localCollider[i].y = res.Y();
  }

  MoveCollider(_position.X() - _colliderCenter.x, _position.Y() - _colliderCenter.y);

  Vector2::RotateVector(res, Vector2::Zero(), _originalDirection, angle);
  _localDirection = res;
  _localDirection.Normalize();
}

void Ship::Accelerate(double dspeed)
{
  _speed += dspeed;

  if (_speed < 0.0)
  {
    _speed = 0.0;
  }
  else if (_speed > _shipMaxSpeed)
  {
    _speed = _shipMaxSpeed;
  }
}
