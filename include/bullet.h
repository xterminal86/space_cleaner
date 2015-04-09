#ifndef BULLET_H
#define BULLET_H

#include "globals.h"

#include "gametime.h"
#include "vector2.h"
#include "sprite.h"
#include "particle_engine.h"

class Bullet
{
  public:
    Bullet();
    virtual ~Bullet();

    void Compute();
    void Fire(Vector2 shotPoint, Vector2 dir, double angle, double speed);

    double Angle() { return _angle; }

    Vector2 Position() { return _position; }

    bool Active() { return _active; }
    void SetActive(bool status)
    {
      _active = status;
      _trail.TurnOff();
    }

    Sprite& GetSprite() { return _bulletSprite; }

    const int TrailLenght = 8;

  protected:
  private:
    Sprite _bulletSprite;
    ParticleEngine _trail;

    Vector2 _position;
    Vector2 _direction;

    double _bulletSpeed;
    double _angle;

    bool _active;

    void Draw(bool drawCollider, bool drawSxes);
    void DrawAxes();
};

#endif // BULLET_H
