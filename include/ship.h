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
  protected:
  private:
    Sprite _shipSprite;

    double _angle;
    double _speed;

    Vector2 _position;

    Vector2 _originalDirection;
    Vector2 _localDirection;

    SDL_Point _colliderCenter;
    std::vector<SDL_Point> _localCollider;

    std::vector<Bullet> _bullets;

    const int _directionResolution = 100;
    const int _maxBullets = 5;
    const double _bulletSpeed = 0.5;
    const double _shipMaxSpeed = 6.0;

    void Draw(int x, int y, bool drawCollider = false);
    void MoveCollider(int x, int y);
};

#endif // SHIP_H
