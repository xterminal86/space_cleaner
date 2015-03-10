#include "asteroid.h"

Asteroid::Asteroid(double posx, double posy)
{
  Init(Vector2(posx, posy));
}

Asteroid::Asteroid(Vector2 pos)
{
  Init(pos);
}

void Asteroid::Init(Vector2 pos)
{
  int res = TextureManager::Get().FindTextureByRole(GlobalStrings::AsteroidRole);
  if (res != -1)
  {
    _asteroidSprite.Init(res);
  }

  _position.Set(pos.X(), pos.Y());

  Util::CreateRandomDirection(_direction);

  _angle = 0.0;

  _angleIncrement = Util::CreateRandomRotation();

  _speed = 0.25 / (double)(rand() % Util::SpeedSpread + 1);

  _asteroidSprite.MoveCollider(pos.X(), pos.Y());
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
  double newX = _position.X() + _direction.X() * (_speed * GameTime::Get().DeltaTime());
  double newY = _position.Y() + _direction.Y() * (_speed * GameTime::Get().DeltaTime());

  if (newX < 0 || newX > VideoSystem::Get().ScreenDimensions().x) _direction.Set(-_direction.X(), _direction.Y());
  if (newY < 0 || newY > VideoSystem::Get().ScreenDimensions().y) _direction.Set(_direction.X(), -_direction.Y());

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
    _asteroidSprite.Draw(x, y, _angle, &_asteroidSprite.TranslatedCollider());
  }
  else
  {
    _asteroidSprite.Draw(x, y, _angle);
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

  int cs = _asteroidSprite.RotatedCollider().size();
  Vector2 res;
  for (int i = 0; i < cs; i++)
  {
    Vector2 origCollider(_asteroidSprite.OriginalCollider()->at(i).x, _asteroidSprite.OriginalCollider()->at(i).y);
    Vector2::RotateVector(res, Vector2(0.0, 0.0), origCollider, angle);

    _asteroidSprite.RotatedCollider()[i].x = res.X();
    _asteroidSprite.RotatedCollider()[i].y = res.Y();
  }

  _asteroidSprite.MoveCollider(_position.X(), _position.Y());
}

void Asteroid::Compute()
{
  _angle += _angleIncrement;

  Rotate(_angle);
  Move();
}

void Asteroid::Breakdown()
{
}
