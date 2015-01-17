#ifndef BULLET_H
#define BULLET_H

#include "vector2.h"
#include "sprite.h"

class Bullet
{
  public:
    Bullet();
    virtual ~Bullet();
  protected:
  private:
    Sprite _bulletSprite;
    Vector2 _position;
};

#endif // BULLET_H
