#ifndef STAR_H
#define STAR_H

#include "vector2.h"
#include "png_loader.h"
#include "gametime.h"
#include "util.h"

class Star
{
private:
  bool _active;

  SDL_Rect _srcRect;
  SDL_Rect _dstRect;

  PNGLoader* _imageRef;
  double _scaleFactor;
  double _alpha;
  double _alphaIncrement;
  Vector2 _position;
  SDL_Color _color;
  int _pauseMs;
  int _msPassed;

public:
  Star();
  virtual ~Star();

  void Init(PNGLoader* imageRef);
  void Pulse();

  void SetRandomPosition();
};

#endif // STAR_H
