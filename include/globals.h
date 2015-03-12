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

namespace GlobalStrings
{
  static std::string BackgroundRole = "background";
  static std::string ShipRole = "ship";
  static std::string AsteroidRole = "asteroid";
  static std::string BulletLaserRole = "laser_shot";

  static std::string ImagesFilename = "images.txt";
  static std::string LogFilename = "output_log.txt";
  static std::string RelationFilename = "relation.txt";

  static std::string BitmapFontFilename = "assets/fonts/retro.png";
}

namespace Math
{
  static const double PIOVER180 = 0.01745329251;
}

namespace GameMechanic
{
  static const int MaxAsteroids = 3;
  static const int AsteroidMaxBreakdownLevel = 1;
  static const int AsteroidBreakdownChildren = 4;
  static const double AsteroidBreakdownScaleFactor = 0.5;
}

namespace Colors
{
  static SDL_Color AsIs = { 255, 255, 255, 255 };
  static SDL_Color Red = { 255, 0, 0, 255 };
}

#endif // GLOBALS_H_INCLUDED
