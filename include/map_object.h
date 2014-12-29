#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include "globals.h"
#include "sprite.h"

class MapObject
{
  public:
    MapObject();
    virtual ~MapObject();
    void Draw();
    virtual void Init() = 0;
  protected:
    Sprite _imageData;
  private:
};

#endif // MAPOBJECT_H
