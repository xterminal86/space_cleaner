#include "asteroid.h"

Asteroid::Asteroid(double posx, double posy)
{
  _position.Set(posx, posy);

  int res = TextureManager::Get().FindTextureByRole(GlobalStrings::AsteroidRole);
  if (res != -1)
  {
    _asteroidSprite.Init(res);
  }

  _angle = 0.0;

  double dx = 1.0 / (double)(rand() % 10 + 1);
  double dy = 1.0 / (double)(rand() % 10 + 1);

  _direction.Set(dx, dy);

  _speed = MaxSpeed / (double)(rand() % 10 + (MaxSpeed + 1));
  _angleIncrement = MaxRotationSpeed / (double)(rand() % 10 + 1);

  int sign = rand() % 4 + 1;

  if (sign % 2 == 0) _angleIncrement *= -1;

  _asteroidSprite.MoveCollider(posx, posy);
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

void Asteroid::Draw(bool drawCollider)
{
  Draw((int)_position.X(), (int)_position.Y(), drawCollider);
}

void Asteroid::Draw(int x, int y, bool drawCollider)
{
  if (drawCollider)
  {
    _asteroidSprite.Draw(x, y, _angle, &_asteroidSprite.TranslatedCollider());
  }
  else
  {
    _asteroidSprite.Draw(x, y);
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
