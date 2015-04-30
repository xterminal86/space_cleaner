#include "animations_pool.h"

AnimationsPool::AnimationsPool()
{
  //ctor
}

AnimationsPool::~AnimationsPool()
{
  _pool.clear();
}

void AnimationsPool::Init(std::string spriteSheetName, int poolSize, int speedMs)
{
  _poolSize = poolSize;
  _speedMs = speedMs;

  _spriteSheet = std::unique_ptr<PNGLoader>(new PNGLoader(spriteSheetName));

  std::string fname = spriteSheetName;
  fname.replace(fname.end() - 3, fname.end(), "txt");
  FILE* f = fopen(fname.data(), "r");
  while (!feof(f))
  {
    fscanf(f, "%i %i %i", &_framesInRow, &_framesInCol, &_totalFrames);
  }

  _frameWidth = _spriteSheet.get()->Width() / _framesInRow;
  _frameHeight = _spriteSheet.get()->Height() / _framesInCol;

  for (int i = 0; i < _poolSize; i++)
  {
    _pool.push_back(std::unique_ptr<AnimationSequence>(new AnimationSequence(this)));
  }
}

void AnimationsPool::Play(int x, int y, double scale)
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

void AnimationsPool::StopAll()
{
  for (int i = 0; i < _pool.size(); i++)
  {
    if (_pool[i].get()->Active())
    {
      _pool[i].get()->SetActive(false);
    }
  }
}

