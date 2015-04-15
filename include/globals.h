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
  static std::string DefaultParticleRole = "default_particle";
  static std::string FontRole = "font";
  static std::string ShipShieldRole = "ship_shield";
  static std::string GUIShieldRole = "gui_shield";
  static std::string GUIHeartRole = "gui_heart";

  static std::string ImagesFilename = "images.txt";
  static std::string LogFilename = "output_log.txt";
  static std::string RelationFilename = "relation.txt";

  static std::string ExplosionSpriteFilename = "assets/animations/explosion.png";
  static std::string ExplosionSpriteShipFilename = "assets/animations/explosion2.png";
  static std::string SpawnAnimationFilename = "assets/animations/spawn.png";
}

namespace Math
{
  static const double PIOVER180 = 0.01745329251;
}

namespace GameMechanic
{
  static const int MaxAsteroids = 8;
  static const int AsteroidMaxBreakdownLevel = 2;
  static const int AsteroidBreakdownChildren = 4;
  static const double AsteroidBreakdownScaleFactor = 1.5;
  static const double AsteroidInitialScaleFactor = 0.5;
  static const double AsteroidMaxSpeed = 0.125;
  static const int StartingSpawnRateMs = 10000;
  static const int MaxSpawnRateMs = 1000;
  static const int SpawnRateDeltaMs = 500;

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

  static const int PowerupLifetimeMs = 5000;
}

namespace Powerups
{
  static const int SHIELD_POWERUP = 0;
  static const int HEALTH_POWERUP = 1;
}

namespace Colors
{
  static SDL_Color AsIs = { 255, 255, 255, 255 };
  static SDL_Color Red = { 255, 0, 0, 255 };
}

#endif // GLOBALS_H_INCLUDED
