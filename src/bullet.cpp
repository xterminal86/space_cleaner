#include "bullet.h"

Bullet::Bullet(int type)
{
  int res = -1;
  switch(type)
  {
    case Bullets::BULLET_ONE_SHOT:
      res = TextureManager::Get().FindTextureByRole(GlobalStrings::BulletOneShotRole);
      _damage = -100;
    break;

    case Bullets::BULLET_ONE_SHOT_AUTO:
      res = TextureManager::Get().FindTextureByRole(GlobalStrings::BulletOneShotAutoRole);
      _damage = -100;
    break;

    case Bullets::BULLET_ROCKET:
      res = TextureManager::Get().FindTextureByRole(GlobalStrings::BulletShotRocketRole);
      _damage = -1;
    break;

    case Bullets::BULLET_LAME:
    default:
      res = TextureManager::Get().FindTextureByRole(GlobalStrings::BulletLameRole);
      _damage = -1;
    break;
  }

  if (res != -1)
  {
    _bulletSprite.Init(res);
    _bulletSprite.SetScaleFactor(1.25);
  }

  _trail.Init(TrailLenght, 100, 110, 0.1, 0.5, _bulletSprite.ImageWrapper());
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

void Bullet::Compute(bool disableTrail)
{
  if (!disableTrail)
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

  if (disableTrail)
  {
    //Draw(true, true);
    Draw(false, false);
  }

  if (_position.X() < 0 || _position.X() > sx || _position.Y() < 0 || _position.Y() > sy)
  {
    _active = false;

    if (!disableTrail)
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
