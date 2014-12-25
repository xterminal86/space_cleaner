#ifndef SHIP_H
#define SHIP_H

#include "globals.h"
#include "sprite.h"

// TODO: think about extending the usage of this class: include support of many ships,
// i.e. make constructor with argument that is index on ship's sprite, or something like that.
class Ship
{
  public:
    Ship();
    virtual ~Ship();
    Sprite& GetSprite()
    {
      return _shipSprite;
    }
    void Draw(int x, int y, bool drawCollider = false);
    void Rotate(double angle);
  protected:
  private:
    Sprite _shipSprite;
    double _angle;

    SDL_Point _colliderCenter;
    std::vector<SDL_Point> _localCollider;

    void MoveCollider(int x, int y);
};

#endif // SHIP_H
