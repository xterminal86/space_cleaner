#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "globals.h"
#include "component.h"
#include "logger.h"

class GameObject
{
  public:
    GameObject();
    virtual ~GameObject()
    {
      Logger::Get().LogPrint("GameObject 0x%zX is departing:\n", this);
      _components.clear();
    }

    virtual void Update() = 0;
  protected:
    std::vector<std::unique_ptr<Component>> _components;
  private:
};

#endif // GAMEOBJECT_H
