#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "gametime.h"
#include "png_loader.h"
#include "vector2.h"
#include "util.h"

struct Fragment
{
  bool Active;

  double Angle;
  double RandomAngleIncrement;
  double Speed;

  SDL_Color Color;
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
    void Init(PNGLoader* imageRef, int subdivisionsPower, int lifeTimeMs);
    void Play(Vector2 pos);
    void Process();
    void SetScale(double scaleFactor)
    {
      _scaleFactor = scaleFactor;
      for (auto& i : _fragments)
      {
        i.DstRect.w = _divX * _scaleFactor;
        i.DstRect.h = _divY * _scaleFactor;
      }
    }
  protected:
  private:
    bool _active;

    PNGLoader* _imageRef;
    int _subdivisionsPower;
    int _lifeTimeMs;
    int _msPassed;
    int _divX;
    int _divY;

    double _alphaValue;
    double _scaleFactor;

    std::vector<Fragment> _fragments;
};

#endif // EXPLOSION_H
