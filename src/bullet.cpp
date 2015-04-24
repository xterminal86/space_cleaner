#include "bullet.h"

Bullet::Bullet(int type)
{
  double scaleFactor = 1.0;
  int res = -1;
  switch(type)
  {
    case Bullets::BULLET_ONE_SHOT:
      res = TextureManager::Get().FindTextureByRole(GlobalStrings::BulletShotSingleRole);
      _damage = -3;
      scaleFactor = 1.0;
    break;

    case Bullets::BULLET_FIRE:
    case Bullets::BULLET_FIRE_AUTO:
      res = TextureManager::Get().FindTextureByRole(GlobalStrings::BulletShotFireRole);
      _damage = -100;
      scaleFactor = 1.0;
    break;

    case Bullets::BULLET_LAME:
    default:
      res = TextureManager::Get().FindTextureByRole(GlobalStrings::BulletLameRole);
      _damage = 0;
      scaleFactor = 1.25;
    break;
  }

  if (res != -1)
  {
    _bulletSprite.Init(res);
    _bulletSprite.SetScaleFactor(scaleFactor);
  }

  _trail.Init(TrailLenght, 100, 110, 0.2, _bulletSprite.ScaleFactor(), _bulletSprite.ImageWrapper());
  _trail.TurnOff();

  _position.ToZero();
  _direction.ToZero();

  _bulletSpeed = 0;
  _angle = 0.0;

  _bulletType = type;

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

  Vector2 tmp = dir;
  tmp.Negate();

  // Look for comments in ParticleEngine::SetLifeAndSpeed for the reason why speed is 0.0
  _trail.SetUp(shotPoint, tmp, 0.0, angle);
}

void Bullet::Compute()
{
  if (_bulletType != Bullets::BULLET_LAME)
  {
    _trail.Emit();
  }

  if (!_active) return;

  int sx = VideoSystem::Get().ScreenDimensions().x;
  int sy = VideoSystem::Get().ScreenDimensions().y;

  double dx = _direction.X() * _bulletSpeed * GameTime::Get().DeltaTimeMs();
  double dy = _direction.Y() * _bulletSpeed * GameTime::Get().DeltaTimeMs();

  _position.Set(_position.X() + dx, _position.Y() + dy);

  _trail.MoveOrigin(_position);

  _bulletSprite.MoveCollider(_position.X(), _position.Y());

  if (_bulletType == Bullets::BULLET_LAME)
  {
    //Draw(true, true);
    Draw(false, false);
  }

  if (_position.X() < 0 || _position.X() > sx || _position.Y() < 0 || _position.Y() > sy)
  {
    _active = false;

    if (_bulletType != Bullets::BULLET_LAME)
    {
      _trail.TurnOff();
    }
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
