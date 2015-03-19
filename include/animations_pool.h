#ifndef EXPLOSIONSPOOL_H
#define EXPLOSIONSPOOL_H

#include "animation_sequence.h"

class AnimationSequence;

class AnimationsPool
{
  public:
    AnimationsPool();
    virtual ~AnimationsPool();
    void Init(std::string spriteSheetName, int poolSize);
    void PlayExplosion(int x, int y);
    void Process();

    int FrameWidthX() { return _frameWidthX; }
    int FrameWidthY() { return _frameWidthY; }
    int FramesNumX() { return _framesNumX; }
    int FramesNumY() { return _framesNumY; }
    int TotalFrames() { return _totalFrames; }

    PNGLoader* SpriteSheet() { return _spriteSheet.get(); }

    const int SpeedMs = 25;

  protected:
  private:
    int _poolSize;
    int _frameWidthX;
    int _frameWidthY;
    int _framesNumX;
    int _framesNumY;
    int _totalFrames;

    std::unique_ptr<PNGLoader> _spriteSheet;

    std::vector<std::unique_ptr<AnimationSequence>> _pool;
};

#endif // EXPLOSIONSPOOL_H
