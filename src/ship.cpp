#include "ship.h"

Ship::Ship(double posx, double posy)
{
  _angle = 0.0;
  _speed = 0.0;

  _position.Set(posx, posy);

  // We go up the screen when Y is decreasing.
  _originalDirection.Set(0.0, -1.0 * DirectionResolution);
  _localDirection.Set(0.0, -1.0 * DirectionResolution);

  int res = TextureManager::Get().FindTextureByRole(GlobalStrings::ShipRole);
  if (res != -1)
  {
    _shipSprite.Init(res);
  }

  for (int i = 0; i < MaxBullets; i++)
  {
    Bullet b;
    _bullets.push_back(b);
  }
}

Ship::~Ship()
{
  //dtor
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
  double newX = _position.X() + _localDirection.X() * _speed;
  double newY = _position.Y() + _localDirection.Y() * _speed;

  if (newX < 0) newX = 0;
  if (newY < 0) newY = 0;
  if (newX > VideoSystem::Get().ScreenDimensions().x) newX = VideoSystem::Get().ScreenDimensions().x;
  if (newY > VideoSystem::Get().ScreenDimensions().y) newY = VideoSystem::Get().ScreenDimensions().y;

  _position.Set(newX, newY);
}

void Ship::Draw(int x, int y, bool drawCollider)
{
  if (drawCollider)
  {
    _shipSprite.Draw(x, y, _angle, &_shipSprite.LocalCollider());

    SDL_RenderDrawLine(VideoSystem::Get().Renderer(), _position.X(), _position.Y(),
                                                      _position.X() + (int)(_localDirection.X() * DirectionResolution),
                                                      _position.Y() + (int)(_localDirection.Y() * DirectionResolution));
  }
  else
  {
    _shipSprite.Draw(x, y, _angle);
  }
}

void Ship::Draw(bool drawCollider)
{
  Draw((int)_position.X(), (int)_position.Y(), drawCollider);
}

void Ship::ComputeBullets()
{
  for (int i = 0; i < _bullets.size(); i++)
  {
    _bullets[i].Compute();
  }
}

void Ship::Rotate(double angle)
{
  _angle = angle;

  int cs = _shipSprite.LocalCollider().size();
  Vector2 res;
  for (int i = 0; i < cs; i++)
  {
    Vector2 origCollider(_shipSprite.OriginalCollider()->at(i).x, _shipSprite.OriginalCollider()->at(i).y);
    Vector2::RotateVector(res, Vector2(0.0, 0.0), origCollider, angle);

    _shipSprite.LocalCollider()[i].x = res.X();
    _shipSprite.LocalCollider()[i].y = res.Y();
  }

  _shipSprite.MoveCollider(_position.X(), _position.Y());

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
  else if (_speed > ShipMaxSpeed)
  {
    _speed = ShipMaxSpeed;
  }
}

void Ship::Fire()
{
  for (int i = 0; i < _bullets.size(); i++)
  {
    if (_bullets[i].Active() == false)
    {
      Vector2 shotPoint(_position.X(), _position.Y());
      _bullets[i].Fire(shotPoint, _localDirection, _angle, BulletSpeed);
      break;
    }
  }
}
