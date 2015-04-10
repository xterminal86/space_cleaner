#include "ship.h"

Ship::Ship()
{
}

void Ship::Init(double posx, double posy)
{
  _active = true;

  _angle = 0.0;
  _speed = 0.0;

  _position.Set(posx, posy);

  // We go up the screen when Y is decreasing.
  _originalDirection.Set(0.0, -1.0);
  _localDirection.Set(0.0, -1.0);

  int res = TextureManager::Get().FindTextureByRole(GlobalStrings::ShipBigRole);
  //int res = TextureManager::Get().FindTextureByRole(GlobalStrings::ShipRole);
  if (res != -1)
  {
    _shipSprite.Init(res);
  }

  for (int i = 0; i < MaxBullets; i++)
  {
    _bullets.push_back(std::unique_ptr<Bullet>(new Bullet()));
  }

  PNGLoader* image = nullptr;

  int index = TextureManager::Get().FindTextureByRole(GlobalStrings::DefaultParticleRole);
  if (index != -1)
  {
    image = TextureManager::Get().GetTextureWrapper(index);
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
  if (newY < 0) newY = 0;
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
    _bullets[i].get()->Compute();
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
  _engineTrail.SetLifeAndSpeed(_speed * 100, _speed * 100, _speed);
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

void Ship::ResetEnginePoint(Vector2 newEnginePoint)
{
  _originalEnginePoint.Set(newEnginePoint);
  _enginePointRotated.Set(newEnginePoint);
  _enginePointTranslated.Set(newEnginePoint);
}
