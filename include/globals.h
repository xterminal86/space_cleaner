#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <typeinfo>
#include <typeindex>

#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>

#include "vector2.h"

#define PIOVER180     0.01745329251

namespace GlobalStrings
{
  static std::string BackgroundRole = "background";
  static std::string ShipRole = "ship";
  static std::string AsteroidRole = "asteroid";

  static std::string ImagesFilename = "images.txt";
  static std::string LogFilename = "output_log.txt";
  static std::string RelationFilename = "relation.txt";
}

namespace Utility
{
  static SDL_Point RotateVector(SDL_Point around, SDL_Point what, double angle)
  {
    double nx = around.x + (what.x - around.x) * SDL_cos(angle * PIOVER180) - (what.y - around.y) * SDL_sin(angle * PIOVER180);
    double ny = around.y + (what.x - around.x) * SDL_sin(angle * PIOVER180) + (what.y - around.y) * SDL_cos(angle * PIOVER180);

    return SDL_Point((int)nx, (int)ny);
  }
}

#endif // GLOBALS_H_INCLUDED
