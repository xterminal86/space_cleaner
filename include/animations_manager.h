#ifndef ANIMATIONSMANAGER_H
#define ANIMATIONSMANAGER_H

#include "globals.h"
#include "animations_pool.h"

class AnimationsManager
{
  public:
    static AnimationsManager& Get()
    {
      static AnimationsManager instance;
      return instance;
    }
    void Init();
    void Play(int id, int x, int y, double scale = 1.0);
    void Process();
    void StopAnimations();
  protected:
  private:
    AnimationsManager();
    virtual ~AnimationsManager();
    AnimationsManager& operator=(const AnimationsManager&);
    AnimationsManager(const AnimationsManager&);

    std::map<int, std::unique_ptr<AnimationsPool>> _animations;
};

#endif // ANIMATIONSMANAGER_H
