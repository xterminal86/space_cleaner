#ifndef COMPONENT_H
#define COMPONENT_H

#include "globals.h"

class GameObject;

class Component
{
  public:
    Component();
    virtual ~Component();

    virtual void Update() = 0;
  protected:
    GameObject* _ownerGameObjectPointer;
    size_t _componentId;
  private:
};

#endif // COMPONENT_H
