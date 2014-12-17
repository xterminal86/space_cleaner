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
    virtual void Init(MapCoordinate coordinate) = 0;
  protected:
    MapCoordinate _mapCoordinate;
    Sprite _imageData;
  private:
};

#endif // MAPOBJECT_H
