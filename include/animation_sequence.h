#ifndef SPRITEANIMATED_H
#define SPRITEANIMATED_H

#include "gametime.h"
#include "png_loader.h"
#include "animations_pool.h"

// Holds AnimationSequence list
class AnimationsPool;

// Draws sequence of sprites from _spriteSheetRef with speed specified by AnimationsPool
class AnimationSequence
{
  public:
    AnimationSequence(AnimationsPool* owner);
    virtual ~AnimationSequence();

    void Play(int x, int y, double scale = 1.0);
    void Draw();

    bool Active() { return _active; }
    void SetActive(bool status) { _active = status; }

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
