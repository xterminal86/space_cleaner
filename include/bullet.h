#ifndef BULLET_H
#define BULLET_H

#include "globals.h"

#include "gametime.h"
#include "vector2.h"
#include "sprite.h"

class Bullet
{
  public:
    Bullet();
    virtual ~Bullet();

    void Compute();
    void Fire(Vector2 shotPoint, Vector2 dir, double angle, double speed);

    Vector2 Position() { return _position; }

    bool Active() { return _active; }

    Sprite& BulletSprite() { return _bulletSprite; }
  protected:
  private:
    Sprite _bulletSprite;

    Vector2 _position;
    Vector2 _direction;

    double _bulletSpeed;
    double _angle;

    bool _active;

    void Draw();
};

#endif // BULLET_H
