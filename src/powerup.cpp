#include "powerup.h"

Powerup::Powerup()
{
  //ctor
}

Powerup::~Powerup()
{
  //dtor
}

void Powerup::Create(int type)
{
  _type = type;
  _active = false;
  _timePassedMs = 0;
  _scaleFactor = _defaultScaleFactor;
  _currentScaleIncrement = _scaleIncrement;
  _lifeTimeMs = GameMechanic::PowerupLifetimeMs;

  int spriteIndex = -1;
  switch (_type)
  {
    case Powerups::HEALTH_POWERUP:
      spriteIndex = TextureManager::Get().FindTextureByRole(GlobalStrings::GUIHeartRole);
      break;

    case Powerups::SHIELD_POWERUP:
      spriteIndex = TextureManager::Get().FindTextureByRole(GlobalStrings::GUIShieldRole);
      break;

    case Powerups::LIFE_POWERUP:
      spriteIndex = TextureManager::Get().FindTextureByRole(GlobalStrings::ShipBigRole);
      break;
    default:
      break;
  }

  if (spriteIndex != -1)
  {
    _sprite.Init(spriteIndex);
    _sprite.SetScaleFactor(_scaleFactor);
  }
}

void Powerup::CheckBounds()
{
  if (_position.X() <= _sprite.ImageWrapper()->Width())
  {
    _position.Set(_position.X() + _sprite.ImageWrapper()->Width(), _position.Y());
  }
  if (_position.X() >= VideoSystem::Get().ScreenDimensions().x - _sprite.ImageWrapper()->Width())
  {
    _position.Set(_position.X() - _sprite.ImageWrapper()->Width(), _position.Y());
  }
  if (_position.Y() <= _sprite.ImageWrapper()->Height())
  {
    _position.Set(_position.X(), _position.Y() + _sprite.ImageWrapper()->Height());
  }
  if (_position.Y() >= VideoSystem::Get().ScreenDimensions().y - _sprite.ImageWrapper()->Height())
  {
    _position.Set(_position.X(), _position.Y() - _sprite.ImageWrapper()->Height());
  }
}

void Powerup::Spawn(Vector2 pos)
{
  _position = pos;

  CheckBounds();

  _active = true;
  _timePassedMs = 0;
  _scaleFactor = _defaultScaleFactor;
}

void Powerup::Process()
{
  if (!_active) return;

  _scaleFactor -= (_currentScaleIncrement * GameTime::Get().DeltaTimeMs());

  if (_scaleFactor <= _minimalScaleFactor)
  {
    _scaleFactor = _minimalScaleFactor;
    _currentScaleIncrement *= -1;
  }

  if (_scaleFactor >= _maximumScaleFactor)
  {
    _scaleFactor = _maximumScaleFactor;
    _currentScaleIncrement *= -1;
  }

  _sprite.SetScaleFactor(_scaleFactor);
  _sprite.SetAngle(0.0);
  _sprite.MoveCollider(_position.X(), _position.Y());
  //_sprite.Draw(_position.X(), _position.Y(), _sprite.GetCollisionInfo().TranslatedColliderRef);
  _sprite.Draw(_position.X(), _position.Y());

  if (_timePassedMs >= _lifeTimeMs)
  {
    _active = false;
  }

  _timePassedMs += GameTime::Get().DeltaTimeMs();
}
