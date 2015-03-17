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
  //printf ("Bullet dtor\t");
}

void Bullet::Fire(Vector2 shotPoint, Vector2 dir, double angle, double speed)
{
  _position = shotPoint;
  _direction = dir;
  _bulletSpeed = speed;
  _angle = angle;
  _active = true;

  _bulletSprite.SetAngle(angle);
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

  Draw(true, true);

  if (_position.X() < 0 || _position.X() > sx || _position.Y() < 0 || _position.Y() > sy)
  {
    _active = false;
  }
}

void Bullet::Draw(bool drawCollider, bool drawAxes)
{
  if (drawCollider)
  {
    _bulletSprite.Draw(_position.X(), _position.Y(), &_bulletSprite.TranslatedCollider());
  }
  else
  {
    _bulletSprite.Draw(_position.X(), _position.Y());
  }

  if (drawAxes)
  {
    DrawAxes();
  }
}

void Bullet::DrawAxes()
{
  SDL_SetRenderDrawColor(VideoSystem::Get().Renderer(), 0, 255, 255, 255);

  _bulletSprite.CalculateSATAxes();
  auto axes = _bulletSprite.GetAxes();

  for (auto &i : axes)
  {
    SDL_Point pos;

    pos.x = (int)_position.X() + i.x;
    pos.y = (int)_position.Y() + i.y;

    SDL_RenderDrawLine(VideoSystem::Get().Renderer(), (int)_position.X(), (int)_position.Y(), pos.x, pos.y);
  }
}
