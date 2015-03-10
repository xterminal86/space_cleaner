#ifndef ASTEROID_H
#define ASTEROID_H

#include "gametime.h"
#include "sprite.h"
#include "vector2.h"
#include "util.h"

class Asteroid
{
  public:
    Asteroid(double posx, double posy);
    Asteroid(Vector2 pos);
    virtual ~Asteroid();
    void Draw(bool drawCollider = false, bool drawAxes = false);
    void Move(Vector2 newPos);
    void Move(double x, double y);
    void Move();
    void Rotate(double angle);
    void Compute();
    void Breakdown();

    Vector2& Position() { return _position; }

    Sprite& GetSprite() { return _asteroidSprite; }

    const int Children = 4;
    const double BreakdownScaleFactor = 0.5;

  protected:
  private:
    Sprite _asteroidSprite;
    std::vector<std::unique_ptr<Asteroid>> _children;

    double _speed;
    double _angle;
    double _angleIncrement;

    Vector2 _position;
    Vector2 _direction;

    void Draw(int x, int y, bool drawCollider = false);
    void DrawAxes();
    void Init(Vector2 pos);
};

#endif // ASTEROID_H
