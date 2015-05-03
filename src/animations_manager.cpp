#include "animations_manager.h"

AnimationsManager::AnimationsManager()
{
}

AnimationsManager::~AnimationsManager()
{
}

// Loads all animations from file and constructs a vector of respective pool objects
// based on the information if AnimationsFilename
void AnimationsManager::Init()
{
  FILE* f = fopen(GlobalStrings::AnimationsFilename.data(), "r");
  if (f != nullptr);
  {
    char buf[512];
    std::string tmp;
    int id = 0, poolSize = 0, ms = 0;
    while (!feof(f))
    {
      tmp.clear();
      fscanf(f, "%i %s %i %i", &id, buf, &poolSize, &ms);
      tmp = buf;
      _animations[id] = std::unique_ptr<AnimationsPool>(new AnimationsPool());
      _animations[id].get()->Init(tmp, poolSize, ms);
    }
  }
  fclose(f);
}

void AnimationsManager::Play(int id, int x, int y, double scale)
{
  if (_animations.count(id) == 1)
  {
    _animations[id].get()->Play(x, y, scale);
  }
}

void AnimationsManager::Process()
{
  for (int i = 0; i < _animations.size(); i++)
  {
    _animations[i].get()->Process();
  }
}

void AnimationsManager::StopAnimations()
{
  for (int i = 0; i < _animations.size(); i++)
  {
    _animations[i].get()->StopAll();
  }
}
