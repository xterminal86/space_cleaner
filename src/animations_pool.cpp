#include "animations_pool.h"
#include "config.h"

AnimationsPool::AnimationsPool()
{
  //ctor
}

AnimationsPool::~AnimationsPool()
{
  _pool.clear();
}

void AnimationsPool::Init(std::string spriteSheetName, int poolSize, int speedMs, bool fromDisk)
{
  _poolSize = poolSize;
  _speedMs = speedMs;

  _spriteSheet = std::unique_ptr<PNGLoader>(new PNGLoader(spriteSheetName, fromDisk));

  std::string fname = spriteSheetName;
  fname.replace(fname.end() - 3, fname.end(), "txt");

  if (fromDisk)
  {
    FILE* f = fopen(fname.data(), "r");
    while (!feof(f))
    {
      fscanf(f, "%i %i %i", &_framesInRow, &_framesInCol, &_totalFrames);
    }
  }
  else
  {
    char buf[512];
    std::string asciiFile = Config::Get().ConvertFileToAscii(fname);
    std::istringstream iss(asciiFile);
    while (iss.getline(buf, 512))
    {
      sscanf(buf, "%i %i %i", &_framesInRow, &_framesInCol, &_totalFrames);
    }
  }

  _frameWidth = _spriteSheet.get()->Width() / _framesInRow;
  _frameHeight = _spriteSheet.get()->Height() / _framesInCol;

  for (int i = 0; i < _poolSize; i++)
  {
    _pool.push_back(std::unique_ptr<AnimationSequence>(new AnimationSequence(this)));
  }
}

// If we have free animation object available, we play it
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

