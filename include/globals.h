#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <chrono>
#include <random>

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

namespace GlobalStrings
{
  static std::string BackgroundRole = "background";
  static std::string BackgroundStarRole = "background_star";
  static std::string ShipRole = "ship";
  static std::string AsteroidRole = "asteroid";
  static std::string BulletLaserRole = "fire_shot";
  static std::string ShipBigRole = "ship_big";

  static std::string ImagesFilename = "images.txt";
  static std::string LogFilename = "output_log.txt";
  static std::string RelationFilename = "relation.txt";

  static std::string BitmapFontFilename = "assets/fonts/retro.png";

  static std::string ExplosionSpriteFilename = "assets/animations/explosion.png";
  static std::string ExplosionSpriteShipFilename = "assets/animations/explosion2.png";
}

namespace Math
{
  static const double PIOVER180 = 0.01745329251;
}

namespace GameMechanic
{
  static const int MaxAsteroids = 5;
  static const int AsteroidMaxBreakdownLevel = 1;
  static const int AsteroidBreakdownChildren = 4;
  static const double AsteroidBreakdownScaleFactor = 1.5;
  static const double AsteroidInitialScaleFactor = 1.0;
  static const double AsteroidMaxSpeed = 0.125;

  // For visual debug: determines length of direction vectors
  // Because they are normalized, we have to extend them in order to see.
  static const int DirectionResolution = 100;

  // In particular, determines the values below AsteroidMaxSpeed
  // The more - the less speed values can be.
  // TLDR: goes in denominator of asteroid random speed generation.
  static const int RandomSpeedSpread = 5;

  // Determines spread of rotation increment fraction. The more - the less increment value can possibly be.
  // See Util::CreateRandomRotation.
  // TLDR: this value goes in denominator.
  static const int RandomRotationSpeedSpread = 10;
}

namespace Colors
{
  static SDL_Color AsIs = { 255, 255, 255, 255 };
  static SDL_Color Red = { 255, 0, 0, 255 };
}

#endif // GLOBALS_H_INCLUDED
