#ifndef EXPLOSIONSPOOL_H
#define EXPLOSIONSPOOL_H

#include "sprite_animated.h"

class ExplosionsPool
{
  public:
    ExplosionsPool();
    virtual ~ExplosionsPool();
    void InitPool(int size);
    void PlayExplosion(int x, int y);
    void Process();
  protected:
  private:
    int _poolSize;

    std::vector<std::unique_ptr<SpriteAnimated>> _pool;
};

#endif // EXPLOSIONSPOOL_H
