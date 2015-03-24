#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "png_loader.h"
#include "vector2.h"
#include "util.h"

struct Fragment
{
  double Angle;
  double RandomAngleIncrement;

  SDL_Rect SrcRect;
  SDL_Rect DstRect;
  Vector2 Position;
  Vector2 Direction;
};

class Explosion
{
  public:
    Explosion();
    virtual ~Explosion();
    void Init(PNGLoader* imageRef, int fragmentsNum);
  protected:
  private:
    PNGLoader* _imageRef;
    int _fragmentsNum;

    std::vector<Fragment> _fragments;
};

#endif // EXPLOSION_H
