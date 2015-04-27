#include "asteroid.h"
#include "mainstate.h"

Asteroid::Asteroid(double posx, double posy, int breakdownLevel, std::vector<std::unique_ptr<Asteroid>>* mainAsteroidsCollection)
{
  Init(Vector2(posx, posy), breakdownLevel, mainAsteroidsCollection);
}

Asteroid::Asteroid(Vector2 pos, int breakdownLevel, std::vector<std::unique_ptr<Asteroid>>* mainAsteroidsCollection)
{
  Init(pos, breakdownLevel, mainAsteroidsCollection);
}

void Asteroid::Init(Vector2 pos, int breakdownLevel, std::vector<std::unique_ptr<Asteroid>>* mainAsteroidsCollection)
{
  int res = TextureManager::Get().FindTextureByRole(GlobalStrings::AsteroidRole);
  if (res != -1)
  {
    _asteroidSprite.Init(res);
  }

  _position.Set(pos.X(), pos.Y());

  _asteroidSprite.SetScaleFactor(breakdownLevel == 0 ?
                                 GameMechanic::AsteroidInitialScaleFactor :
                                 GameMechanic::AsteroidInitialScaleFactor / (GameMechanic::AsteroidBreakdownScaleFactor * breakdownLevel));

  Util::CreateRandomDirection(_direction);

  _angle = 0.0;

  _angleIncrement = Util::CreateRandomRotation();

  _speed = GameMechanic::AsteroidMaxSpeed / (double)((Util::RandomNumber() % GameMechanic::RandomSpeedSpread) + 1);

  _asteroidSprite.MoveCollider(pos.X(), pos.Y());

  _active = true;

  _currentBreakdownLevel = breakdownLevel;

  if (_currentBreakdownLevel == 0)
  {
    _instances++;
  }

  _hitPoints = (GameMechanic::AsteroidMaxBreakdownLevel - _currentBreakdownLevel) * (GameMechanic::AsteroidMaxBreakdownLevel + 1);
  _hitPoints *= HitPointsScale;

  _maxHitPoints = _hitPoints;

  _mainAsteroidsCollectionReference = mainAsteroidsCollection;
}

Asteroid::~Asteroid()
{
}

void Asteroid::Move(double x, double y)
{
  _position.Set(x, y);
  _asteroidSprite.MoveCollider(_position.X(), _position.Y());
}

void Asteroid::Move(Vector2 newPos)
{
  Move(newPos.X(), newPos.Y());
}

void Asteroid::Move()
{
  double newX = _position.X() + _direction.X() * (_speed * GameTime::Get().DeltaTimeMs());
  double newY = _position.Y() + _direction.Y() * (_speed * GameTime::Get().DeltaTimeMs());

  if (newX < 0 || newX > VideoSystem::Get().ScreenDimensions().x)
  {
    _direction.Set(-_direction.X(), _direction.Y());
    newX = _position.X() + _direction.X() * (_speed * GameTime::Get().DeltaTimeMs());
  }

  if (newY < GUI::GUITopBackgroundHeight + 20 || newY > VideoSystem::Get().ScreenDimensions().y)
  {
    _direction.Set(_direction.X(), -_direction.Y());
    newY = _position.Y() + _direction.Y() * (_speed * GameTime::Get().DeltaTimeMs());
  }

  _position.Set(newX, newY);
  _asteroidSprite.MoveCollider(newX, newY);
}

void Asteroid::Draw(bool drawCollider, bool drawAxes)
{
  Draw((int)_position.X(), (int)_position.Y(), drawCollider);

  if (drawAxes)
  {
    DrawAxes();
  }
}

void Asteroid::Draw(int x, int y, bool drawCollider)
{
  if (drawCollider)
  {
    _asteroidSprite.Draw(x, y, &_asteroidSprite.TranslatedCollider());
  }
  else
  {
    _asteroidSprite.Draw(x, y);
  }
}

void Asteroid::DrawAxes()
{
  SDL_SetRenderDrawColor(VideoSystem::Get().Renderer(), 0, 255, 255, 255);

  _asteroidSprite.CalculateSATAxes();
  auto axes = _asteroidSprite.GetAxes();

  for (auto &i : axes)
  {
    SDL_Point pos;

    pos.x = (int)_position.X() + i.x;
    pos.y = (int)_position.Y() + i.y;

    SDL_RenderDrawLine(VideoSystem::Get().Renderer(), (int)_position.X(), (int)_position.Y(), pos.x, pos.y);
  }
}

void Asteroid::Rotate(double angle)
{
  _angle = angle;

  _asteroidSprite.SetAngle(angle);
  _asteroidSprite.MoveCollider(_position.X(), _position.Y());
}

void Asteroid::Compute()
{
  _angle += _angleIncrement;

  Rotate(_angle);
  Move();
}

void Asteroid::ProcessCollision(Bullet* bulletRef)
{
  // Smallest asteroids are disabled regardless of their hitpoints
  if (_currentBreakdownLevel == GameMechanic::AsteroidMaxBreakdownLevel)
  {
    _currentBreakdownLevel++;
    _active = false;

    if (bulletRef != nullptr)
    {
      SoundSystem::Get().PlaySound(Sounds::ASTEROID_HIT_SMALL);

      AnimationsManager::Get().Play(AnimationsIds::EXPLOSION_ASTEROID,
                                    _position.X(),
                                    _position.Y(),
                                    GameMechanic::BigAsteroidExplosionScale / (_currentBreakdownLevel + 1));

      MainState::Get().TryToSpawnPowerup(_position.X(), _position.Y());
      MainState::Get().AddScore(_currentBreakdownLevel);
      MainState::Get().AddKills(1);
    }

    return;
  }

  // Collision with ship
  if (bulletRef == nullptr)
  {
    _currentBreakdownLevel++;
    _active = false;
    Breakdown();
  }
  // Collision with bullet
  else
  {
    AddDamage(bulletRef->Damage());

    if (_hitPoints <= 0)
    {
      SoundSystem::Get().PlaySound(Sounds::ASTEROID_HIT_BIG);

      AnimationsManager::Get().Play(AnimationsIds::EXPLOSION_ASTEROID,
                                    _position.X(),
                                    _position.Y(),
                                    GameMechanic::BigAsteroidExplosionScale / (_currentBreakdownLevel + 1));

      MainState::Get().TryToSpawnPowerup(_position.X(), _position.Y());

      _currentBreakdownLevel++;

      MainState::Get().AddScore(_currentBreakdownLevel);
      MainState::Get().AddKills(1);

      _active = false;
      Breakdown();
    }
    else
    {
      AnimationsManager::Get().Play(bulletRef->BulletType() == Bullets::BULLET_LAME ? AnimationsIds::BULLET_HIT : AnimationsIds::BULLET_HIT2, bulletRef->Position().X(), bulletRef->Position().Y());
      SoundSystem::Get().PlaySound(bulletRef->BulletType() == Bullets::BULLET_LAME ? Sounds::ASTEROID_HIT : Sounds::ASTEROID_HIT2);
    }
  }
}

void Asteroid::Breakdown()
{
  if (_currentBreakdownLevel == 1) _instances--;

  for (int i = 0; i < GameMechanic::AsteroidBreakdownChildren; i++)
  {
    _mainAsteroidsCollectionReference->push_back(std::unique_ptr<Asteroid>(new Asteroid(_position, _currentBreakdownLevel, _mainAsteroidsCollectionReference)));
  }
}

void Asteroid::AddDamage(int value)
{
  _hitPoints += value;

  if (_hitPoints > _maxHitPoints) _hitPoints = _maxHitPoints;

  if (_hitPoints <= 0)
  {
    _hitPoints = 0;
  }
}
