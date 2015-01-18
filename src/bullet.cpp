#include "bullet.h"

Bullet::Bullet()
{
  int res = TextureManager::Get().FindTextureByRole(GlobalStrings::BulletLaserRole);
  if (res != -1)
  {
    _bulletSprite.Init(res);
  }

  _position.ToZero();
  _direction.ToZero();

  _bulletSpeed = 0;
  _angle = 0.0;

  _active = false;
}

Bullet::~Bullet()
{
  //dtor
}

void Bullet::Fire(Vector2 shotPoint, Vector2 dir, double angle, double speed)
{
  _position = shotPoint;
  _direction = dir;
  _bulletSpeed = speed;
  _angle = angle;
  _active = true;
}

void Bullet::Compute(bool drawCollider)
{
  if (!_active) return;

  int sx = VideoSystem::Get().ScreenDimensions().x;
  int sy = VideoSystem::Get().ScreenDimensions().y;

  _position.Set(_position.X() + _direction.X() * _bulletSpeed * GameTime::Get().DeltaTime(),
                _position.Y() + _direction.Y() * _bulletSpeed * GameTime::Get().DeltaTime());

  Draw(drawCollider);

  if (_position.X() < 0 || _position.X() > sx || _position.Y() < 0 || _position.Y() > sy)
  {
    _active = false;
  }
}

void Bullet::Draw(bool drawCollider)
{
  _bulletSprite.Draw(_position.X(), _position.Y(), _angle, drawCollider);
}
