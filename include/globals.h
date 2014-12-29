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
  // Explanation from previous case:
  //
  // By default rotation is assumed around (0; 0). It is located in the top left corner of the image.
  // Our actual rotation center is image's (w / 2; h / 2). So, first, we move it to (0; 0) (which means, that collider's coordinates are "offseted"),
  // then rotate collider around it, and then move it back ("offset" collider's coordinates back).

  template <typename T>
  static void RotateVector(T around, T what, double angle, T& result)
  {
    double nx = around.X() + (what.X() - around.X()) * SDL_cos(angle * PIOVER180) - (what.Y() - around.Y()) * SDL_sin(angle * PIOVER180);
    double ny = around.Y() + (what.X() - around.X()) * SDL_sin(angle * PIOVER180) + (what.Y() - around.Y()) * SDL_cos(angle * PIOVER180);

    result.Set(nx, ny);
  }
}

#endif // GLOBALS_H_INCLUDED
