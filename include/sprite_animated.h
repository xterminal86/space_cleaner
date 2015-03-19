#ifndef SPRITEANIMATED_H
#define SPRITEANIMATED_H

#include "gametime.h"
#include "png_loader.h"

class SpriteAnimated
{
  public:
    SpriteAnimated();
    virtual ~SpriteAnimated();

    void Play(int x, int y);
    void Draw();

    bool Active() { return _active; }

    const int SpeedMs = 25;

  protected:
  private:

  std::unique_ptr<PNGLoader> _spriteSheet;

  bool _active;

  int _wx;
  int _wy;

  int _lengthX;
  int _lengthY;

  SDL_Rect _src;
  SDL_Rect _dst;

  int _frames;
  int _framesPlayed;

  double _currentMsPassed;
};

#endif // SPRITEANIMATED_H
