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
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace GlobalStrings
{
  static std::string BackgroundRole = "background";
  static std::string ShipRole = "ship";
  static std::string AsteroidRole = "asteroid";
  static std::string BulletLaserRole = "laser_shot";

  static std::string ImagesFilename = "images.txt";
  static std::string LogFilename = "output_log.txt";
  static std::string RelationFilename = "relation.txt";
}

namespace Math
{
  static const double PIOVER180 = 0.01745329251;
}

#endif // GLOBALS_H_INCLUDED
