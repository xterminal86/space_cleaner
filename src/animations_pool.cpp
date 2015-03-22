#include "animations_pool.h"

AnimationsPool::AnimationsPool()
{
  //ctor
}

AnimationsPool::~AnimationsPool()
{
  _pool.clear();
}

void AnimationsPool::Init(std::string spriteSheetName, int poolSize)
{
  _poolSize = poolSize;

  _spriteSheet = std::unique_ptr<PNGLoader>(new PNGLoader(GlobalStrings::ExplosionSpriteFilename));

  std::string fname = spriteSheetName;
  fname.replace(fname.end() - 3, fname.end(), "txt");
  FILE* f = fopen(fname.data(), "r");
  while (!feof(f))
  {
    fscanf(f, "%i %i", &_framesInRow, &_totalFrames);
  }

  _frameWidth = _spriteSheet.get()->Width() / _framesInRow;
  _frameHeight = _spriteSheet.get()->Height() / _framesInRow;

  for (int i = 0; i < _poolSize; i++)
  {
    _pool.push_back(std::unique_ptr<AnimationSequence>(new AnimationSequence(this)));
  }
}

void AnimationsPool::PlayExplosion(int x, int y, double scale)
{
  for (int i = 0; i < _pool.size(); i++)
  {
    if (!_pool[i].get()->Active())
    {
      _pool[i].get()->Play(x, y, scale);
      break;
    }
  }
}

void AnimationsPool::Process()
{
  for (int i = 0; i < _pool.size(); i++)
  {
    _pool[i].get()->Draw();
  }
}

