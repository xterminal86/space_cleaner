#ifndef ASTEROID_H
#define ASTEROID_H

#include "gametime.h"
#include "sprite.h"
#include "vector2.h"

class Asteroid
{
  public:
    Asteroid(double posx, double posy);
    virtual ~Asteroid();
    void Draw(bool drawCollider = false);
    void Move(Vector2 newPos);
    void Move(double x, double y);
    void Move();
    void Rotate(double angle);
  protected:
  private:
    Sprite _asteroidSprite;

    double _speed;
    double _angle;

    Vector2 _position;
    Vector2 _direction;

    void Draw(int x, int y, bool drawCollider = false);
};

#endif // ASTEROID_H