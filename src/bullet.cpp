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
  //printf ("bullet dtor ");
}

void Bullet::Fire(Vector2 shotPoint, Vector2 dir, double angle, double speed)
{
  _position = shotPoint;
  _direction = dir;
  _bulletSpeed = speed;
  _angle = angle;
  _active = true;

  Vector2 res;
  int cs = _bulletSprite.RotatedCollider().size();
  for (int i = 0; i < cs; i++)
  {
    Vector2 origCollider(_bulletSprite.OriginalCollider()->at(i).x, _bulletSprite.OriginalCollider()->at(i).y);
    Vector2::RotateVector(res, Vector2(0.0, 0.0), origCollider, angle);

    _bulletSprite.RotatedCollider()[i].x = res.X();
    _bulletSprite.RotatedCollider()[i].y = res.Y();
  }

  _bulletSprite.MoveCollider(shotPoint.X(), shotPoint.Y());
}

void Bullet::Compute()
{
  if (!_active) return;

  int sx = VideoSystem::Get().ScreenDimensions().x;
  int sy = VideoSystem::Get().ScreenDimensions().y;

  double dx = _direction.X() * _bulletSpeed * GameTime::Get().DeltaTime();
  double dy = _direction.Y() * _bulletSpeed * GameTime::Get().DeltaTime();

  _position.Set(_position.X() + dx, _position.Y() + dy);

  _bulletSprite.MoveCollider(_position.X(), _position.Y());

  Draw();

  if (_position.X() < 0 || _position.X() > sx || _position.Y() < 0 || _position.Y() > sy)
  {
    _active = false;
  }
}

void Bullet::Draw()
{
  _bulletSprite.Draw(_position.X(), _position.Y(), _angle, &_bulletSprite.TranslatedCollider());
}
