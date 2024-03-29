#include "ship.h"

Ship::Ship()
{
}

void Ship::Init(double posx, double posy)
{
  _active = true;
  _shieldHit = false;
  _autoFire = false;

  _shieldColor.r = 255;
  _shieldColor.g = 255;
  _shieldColor.b = 255;
  _shieldColor.a = 255;

  _angle = 0.0;
  _speed = 0.0;

  _hitPoints = ShipMaxHitPoints;
  _shieldPoints = ShieldMaxPoints;

  _position.Set(posx, posy);

  // We go up the screen when Y is decreasing.
  _originalDirection.Set(0.0, -1.0);
  _localDirection.Set(0.0, -1.0);

  int res = TextureManager::Get().FindTextureByRole(GlobalStrings::ShipRole);
  if (res != -1)
  {
    _shipSprite.Init(res);
  }

  res = TextureManager::Get().FindTextureByRole(GlobalStrings::ShipShieldRole);
  if (res != -1)
  {
    _shieldSprite.Init(res);
  }

  _kills = 0;
  _level = 0;

  for (int i = 0; i < Bullets::BULLET_TOTAL_TYPES; i++)
  {
    for (int j = 0; j < MaxBullets; j++)
    {
      _bullets[i].push_back(std::unique_ptr<Bullet>(new Bullet(i)));
    }
  }

  ChangeWeaponType(Bullets::BULLET_LAME);

  PNGLoader* image = nullptr;

  int index = TextureManager::Get().FindTextureByRole(GlobalStrings::DefaultParticleRole);
  if (index != -1)
  {
    image = TextureManager::Get().GetTextureWrapper(index);
  }

  index = TextureManager::Get().FindTextureByRole(GlobalStrings::BulletLameRole);
  if (index != -1)
  {
    _currentWeaponImage.Init(index, true);
  }

  int engineY = _shipSprite.ImageWrapper()->Height() / 2;

  ResetEnginePoint(Vector2(_position.X(), _position.Y() + engineY));

  _engineTrail.Init(EngineTrailParticles, 200, 250, DefaultParticleScaleIncrement, DefaultParticleScale, image);

  // Look for comments in ParticleEngine::SetLifeAndSpeed - it's important.
  _engineTrail.SetUp(Vector2(_position.X(), _position.Y() - engineY),
                     Vector2(_originalDirection.X(), -_originalDirection.Y()),
                     0.0, 0.0);

  _engineTrail.TurnOff();
}

Ship::~Ship()
{
  _bullets.clear();
}

void Ship::Scale(double scaleFactor)
{
  _scaleFactor = scaleFactor;

  _shipSprite.SetScaleFactor(scaleFactor);

  // We scale first, then use scaled collider in rotation calculations to determine rotated collider.
  // So, in case we scaled, but forgot to rotate, collider will not update.
  Rotate(_angle);

  double engineY = _shipSprite.ImageWrapper()->Height() / 2;
  engineY *= scaleFactor;

  ResetEnginePoint(Vector2(_position.X(), _position.Y() + engineY));
  _engineTrail.RescaleParticles(scaleFactor * DefaultParticleScale, DefaultParticleScaleIncrement);
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
  if (!_active) return;

  double newX = _position.X() + _localDirection.X() * _speed;
  double newY = _position.Y() + _localDirection.Y() * _speed;

  if (newX < 0) newX = 0;
  if (newY < GUI::GUITopBackgroundHeight + 20) newY = GUI::GUITopBackgroundHeight + 20;
  if (newX > VideoSystem::Get().ScreenDimensions().x) newX = VideoSystem::Get().ScreenDimensions().x;
  if (newY > VideoSystem::Get().ScreenDimensions().y) newY = VideoSystem::Get().ScreenDimensions().y;

  _position.Set(newX, newY);
  _shipSprite.MoveCollider(_position.X(), _position.Y());

  _enginePointTranslated.Set(Vector2(_enginePointRotated.X() + newX, _enginePointRotated.Y() + newY));
  _engineTrail.MoveOrigin(_enginePointTranslated);
}

void Ship::Draw(int x, int y, bool drawCollider)
{
  _engineTrail.Emit();

  if (_active)
  {
    if (drawCollider)
    {
      if (_shipSprite.Convex())
      {
        _shipSprite.Draw(x, y, &_shipSprite.TranslatedCollider());
      }
      else
      {
        _shipSprite.Draw(x, y, &_shipSprite.TriangulatedTranslatedCollider());
      }

      // Line showing ship's direction

      //SDL_RenderDrawLine(VideoSystem::Get().Renderer(), _position.X(), _position.Y(),
      //                                                  _position.X() + (int)(_localDirection.X() * GameMechanic::DirectionResolution),
      //                                                  _position.Y() + (int)(_localDirection.Y() * GameMechanic::DirectionResolution));
    }
    else
    {
      _shipSprite.Draw(x, y);
      DrawShield();
    }
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
    for (int j = 0; j < _bullets[i].size(); j++)
    {
      _bullets[i].at(j).get()->Compute();
    }
  }
}

void Ship::Rotate(double angle)
{
  _angle = angle;

  if (_angle > 360) _angle -= 360;
  if (_angle < 0) _angle += 360;

  _shipSprite.SetAngle(angle);

  Vector2 res;
  Vector2::RotateVector(res, Vector2::Zero(), _originalDirection, angle);

  _localDirection = res;
  _localDirection.Normalize();

  Vector2::RotateVector(res, Vector2::Zero(), _originalEnginePoint, angle);
  _enginePointRotated = res;
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

  if (dspeed > 0.0 && !_engineTrail.Active())
  {
    _engineTrail.TurnOn();
  }

  if (dspeed <= 0.0 && _engineTrail.Active())
  {
    _engineTrail.TurnOff();
  }

  // Look for comments in ParticleEngine::SetLifeAndSpeed - it's important.
  _engineTrail.SetLifeAndSpeed((int)_speed * 100, (int)_speed * 100, _speed);
}

void Ship::Fire()
{
  for (int i = 0; i < _bullets[_weaponType].size(); i++)
  {
    if (_bullets[_weaponType].at(i).get()->Active() == false)
    {
      Vector2 shotPoint(_position.X(), _position.Y());
      _bullets[_weaponType].at(i).get()->Fire(shotPoint, _localDirection, _angle, BulletSpeed);
      SoundSystem::Get().PlaySound(Sounds::ShotSoundsMap[_weaponType]);
      break;
    }
  }
}

bool Ship::HasBulletsActive()
{
  for (auto &i : _bullets[_weaponType])
  {
    if (i.get()->Active())
    {
      return true;
    }
  }

  return false;
}

void Ship::ResetEnginePoint(Vector2 newEnginePoint)
{
  _originalEnginePoint.Set(newEnginePoint);
  _enginePointRotated.Set(newEnginePoint);
  _enginePointTranslated.Set(newEnginePoint);
}

void Ship::ProcessCollision(Asteroid* collidedAsteroid)
{
  _hitPoints -= ((GameMechanic::AsteroidMaxBreakdownLevel + 1) - collidedAsteroid->CurrentBreakdownLevel());
}

void Ship::ProcessShieldCollision(Asteroid* collidedAsteroid)
{
  _shieldHit = true;
  _shieldColor.a = 255;

  // Damage done is equal to inverse asteroid breakdown level: big asteroid will have 0 breakdown level,
  // so it does (GameMechanic::AsteroidMaxBreakdownLevel + 1) damage. +1 is to avoid zero damage from small asteroids.
  // If we apply the same rules of damage to shield, we can get a situation
  // where player has only 1 shield point remaining and hits big asteroid.
  // In such case asteroid will break and shield will be destroyed, however if player has more than
  // (GameMechanic::AsteroidMaxBreakdownLevel + 1) shield points, they will be reduced by that number, which is not
  // very logical.

  //_shieldPoints -= ((GameMechanic::AsteroidMaxBreakdownLevel + 1) - collidedAsteroid->CurrentBreakdownLevel());

  _shieldPoints -= 1;

  if (_shieldPoints <= 0)
  {
    _shieldPoints = 0;
  }
}

void Ship::DrawShield()
{
  if (_shieldHit)
  {
    _shieldColor.a -= ShieldFadeSpeed;

    if (_shieldColor.a <= 0)
    {
      _shieldColor.a = 0;
      _shieldHit = false;
    }

    _shieldSprite.SetColor(_shieldColor);
    _shieldSprite.Draw(_position.X(), _position.Y());
  }
}

void Ship::Respawn()
{
  SetActive(true);
  _hitPoints = ShipMaxHitPoints;
  _shieldPoints = ShieldMaxPoints;
  _kills = 0;
  _level = 0;
  ChangeWeaponType(Bullets::BULLET_LAME);
}

void Ship::AddKills(int value)
{
  if (_level == (GameMechanic::ExperienceMap.size() - 1)) return;

  _kills += value;

  if (_kills <= 0) _kills = 0;

  if ((_level + 1) >= GameMechanic::ExperienceMap.size())
  {
    return;
  }

  if (_kills >= GameMechanic::ExperienceMap[_level])
  {
    _kills = 0;
    _level++;

    if (Bullets::LevelToWeaponsMap.count(_level) == 1)
    {
      ChangeWeaponType(Bullets::LevelToWeaponsMap[_level]);
    }
  }
}

void Ship::ChangeWeaponType(int type)
{
  _weaponType = type;
  _autoFire = type == Bullets::BULLET_FIRE_AUTO;

  if (type != Bullets::BULLET_LAME)
  {
    SoundSystem::Get().PlaySound(Sounds::WEAPON_UPGRADE);
  }

  int index = TextureManager::Get().FindTextureByRole(Bullets::BulletsToRolesMap[type]);
  if (index != -1)
  {
    _currentWeaponImage.Init(index, true);
  }

}

void Ship::StopBullets()
{
  for (auto &i : _bullets[_weaponType])
  {
    if (i.get()->Active())
    {
      i.get()->SetActive(false);
    }
  }
}

void Ship::ResetShieldVisual()
{
  _shieldColor.a = 0;
  _shieldHit = false;
}
