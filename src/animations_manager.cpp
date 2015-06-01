#include "animations_manager.h"
#include "config.h"

AnimationsManager::AnimationsManager()
{
}

AnimationsManager::~AnimationsManager()
{
}

// Loads all animations from file and constructs a vector of respective pool objects
// based on the information if AnimationsFilename
void AnimationsManager::Init(bool fromDisk)
{
  if (fromDisk)
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
  else
  {
    char buf[512];
    std::string tmp;

    std::string asciiFile = Config::Get().ConvertFileToAscii(GlobalStrings::AnimationsFilename);

    std::istringstream iss(asciiFile);
    char spriteSheet[256];
    int id = 0, poolSize = 0, ms = 0;
    while (iss.getline(buf, 512))
    {
      tmp.clear();
      sscanf(buf, "%i %s %i %i", &id, spriteSheet, &poolSize, &ms);
      tmp = spriteSheet;
      _animations[id] = std::unique_ptr<AnimationsPool>(new AnimationsPool());
      _animations[id].get()->Init(tmp, poolSize, ms);
    }
  }
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
