#include "ship.h"

Ship::Ship()
{
}

void Ship::Init(double posx, double posy)
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
    _bullets.push_back(std::unique_ptr<Bullet>(new Bullet()));
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
  _shipSprite.MoveCollider(_position.X(), _position.Y());
}

void Ship::Draw(int x, int y, bool drawCollider)
{
  if (drawCollider)
  {
    _shipSprite.Draw(x, y, &_shipSprite.TranslatedCollider());

    SDL_RenderDrawLine(VideoSystem::Get().Renderer(), _position.X(), _position.Y(),
                                                      _position.X() + (int)(_localDirection.X() * DirectionResolution),
                                                      _position.Y() + (int)(_localDirection.Y() * DirectionResolution));
  }
  else
  {
    _shipSprite.Draw(x, y);
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
    _bullets[i].get()->Compute();
  }
}

void Ship::Rotate(double angle)
{
  _angle = angle;

  _shipSprite.SetAngle(angle);

  Vector2 res;
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
    if (_bullets[i].get()->Active() == false)
    {
      Vector2 shotPoint(_position.X(), _position.Y());
      _bullets[i].get()->Fire(shotPoint, _localDirection, _angle, BulletSpeed);
      break;
    }
  }
}

bool Ship::HasBulletsActive()
{
  for (auto &i : _bullets)
  {
    if (i.get()->Active())
    {
      return true;
    }
  }

  return false;
}
