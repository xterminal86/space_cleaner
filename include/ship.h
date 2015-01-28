#ifndef SHIP_H
#define SHIP_H

#include "vector2.h"
#include "sprite.h"
#include "bullet.h"

// TODO: think about extending the usage of this class: include support of many ships,
// i.e. make constructor with argument that is index on ship's sprite, or something like that.
class Ship
{
  public:
    Ship(double posx, double posy);
    virtual ~Ship();
    Sprite& GetSprite()
    {
      return _shipSprite;
    }
    void Accelerate(double dspeed);
    void Move(int x, int y);
    void Move(Vector2 newPos);
    void Move();
    void Draw(bool drawCollider = false);
    void ComputeBullets();
    void Rotate(double angle);
    void Fire();

    double Speed() { return _speed; }
    Vector2& Direction() { return _localDirection; }

    const int DirectionResolution = 100;
    const int MaxBullets = 50;
    const double BulletSpeed = 0.07;
    const double ShipMaxSpeed = 4.0;
    const double RotationSpeed = 0.1;
    const double AccelerationSpeed = 0.005;

  protected:
  private:
    Sprite _shipSprite;

    double _angle;
    double _speed;

    Vector2 _position;

    Vector2 _originalDirection;
    Vector2 _localDirection;

    std::vector<Bullet> _bullets;

    void Draw(int x, int y, bool drawCollider = false);
};

#endif // SHIP_H
