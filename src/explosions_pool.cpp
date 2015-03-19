#include "explosions_pool.h"

ExplosionsPool::ExplosionsPool()
{
  //ctor
}

ExplosionsPool::~ExplosionsPool()
{
  _pool.clear();
}

void ExplosionsPool::InitPool(int size)
{
  _poolSize = size;

  for (int i = 0; i < size; i++)
  {
    _pool.push_back(std::unique_ptr<SpriteAnimated>(new SpriteAnimated()));
  }
}

void ExplosionsPool::PlayExplosion(int x, int y)
{
  for (int i = 0; i < _pool.size(); i++)
  {
    if (!_pool[i].get()->Active())
    {
      _pool[i].get()->Play(x, y);
      break;
    }
  }
}

void ExplosionsPool::Process()
{
  for (int i = 0; i < _pool.size(); i++)
  {
    _pool[i].get()->Draw();
  }
}

