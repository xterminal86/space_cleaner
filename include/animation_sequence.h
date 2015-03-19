#ifndef SPRITEANIMATED_H
#define SPRITEANIMATED_H

#include "gametime.h"
#include "png_loader.h"
#include "animations_pool.h"

class AnimationsPool;

class AnimationSequence
{
  public:
    AnimationSequence(AnimationsPool* owner);
    virtual ~AnimationSequence();

    void Play(int x, int y);
    void Draw();

    bool Active() { return _active; }

  protected:
  private:

  AnimationsPool* _owner;
  PNGLoader* _spriteSheetRef;

  bool _active;

  SDL_Rect _src;
  SDL_Rect _dst;

  int _framesPlayed;

  double _currentMsPassed;
};

#endif // SPRITEANIMATED_H
