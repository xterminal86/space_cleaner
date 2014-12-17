#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include <string>
#include <map>
#include <memory>
#include <list>
#include <typeinfo>
#include <typeindex>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

#include "logger.h"

class MapCoordinate
{
public:
  MapCoordinate() { X = -1; Y = -1; Z = -1; }
  MapCoordinate(int x, int y, int z)
  {
    X = x;
    Y = y;
    Z = z;
  }
  unsigned int X;
  unsigned int Y;
  unsigned int Z;
};

enum ObjectOrientation { NW = 0, NE };

#endif // GLOBALS_H_INCLUDED
