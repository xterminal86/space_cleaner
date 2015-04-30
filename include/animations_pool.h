#ifndef EXPLOSIONSPOOL_H
#define EXPLOSIONSPOOL_H

#include "animation_sequence.h"

class AnimationSequence;

class AnimationsPool
{
  public:
    AnimationsPool();
    virtual ~AnimationsPool();
    void Init(std::string spriteSheetName, int poolSize, int speedMs);
    void Play(int x, int y, double scale = 1.0);
    void Process();
    void StopAll();

    int FrameWidth() { return _frameWidth; }
    int FrameHeight() { return _frameHeight; }
    int FramesInRow() { return _framesInRow; }
    int TotalFrames() { return _totalFrames; }
    int SpeedMs() { return _speedMs; }

    PNGLoader* SpriteSheet() { return _spriteSheet.get(); }

  protected:
  private:
    int _poolSize;
    int _frameWidth;
    int _frameHeight;
    int _framesInRow;
    int _framesInCol;
    int _totalFrames;
    int _speedMs;

    std::unique_ptr<PNGLoader> _spriteSheet;

    std::vector<std::unique_ptr<AnimationSequence>> _pool;
};

#endif // EXPLOSIONSPOOL_H
