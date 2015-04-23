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
  static std::string BulletLameRole = "shot_lame";
  static std::string DefaultParticleRole = "default_particle";
  static std::string FontRole = "font";
  static std::string ShipShieldRole = "ship_shield";
  static std::string GUIShieldRole = "gui_shield";
  static std::string GUIHeartRole = "gui_heart";
  static std::string GUIWeaponFrame = "gui_weapon_frame";
  static std::string GUIBlackBack = "gui_black_back";
  static std::string GUIDivider = "gui_divider";
  static std::string GUIShipOutline = "gui_ship_outline";

  static std::string ImagesFilename = "images.txt";
  static std::string LogFilename = "output_log.txt";
  static std::string RelationFilename = "relation.txt";

  static std::string HighScoresFilename = "highscores.dat";

  static std::string ExplosionSpriteFilename = "assets/animations/explosion.png";
  static std::string ExplosionSpriteShipFilename = "assets/animations/explosion2.png";
  static std::string SpawnAnimationFilename = "assets/animations/spawn.png";

  static std::string SoundsFilename = "sounds.txt";

  static std::string HowToPlayString = \
  "Clean the space by shooting down flying asteroids!\n\n\
- Move and turn using WASD, shoot by tapping SPACE ;-)\n\
- Your shield will absorb any amount of damage.\n\
- Damage to ship's hull depends on the size of the incoming asteroid.";
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
  static const double AsteroidMaxSpeed = 0.25;
  static const int StartingSpawnRateMs = 10000;
  static const int MaxSpawnRateMs = 1000;
  static const int SpawnRateDeltaMs = 100;

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
  static const int LIFE_POWERUP = 2;
}

namespace Sounds
{
  static int POWERUP_HEALTH = 0;
  static int SHIP_FIRE = 1;
  static int SHIP_EXPLODE = 2;
  static int SHIP_HIT = 3;
  static int POWERUP_SHIELD = 4;
  static int GAME_OVER = 5;
  static int SHIELD_HIT_ENERGY = 6;
  static int ASTEROID_HIT_BIG = 7;
  static int ASTEROID_SPAWN = 8;
  static int ASTEROID_HIT_SMALL = 9;
  static int MENU_MOVE = 10;
  static int MENU_SELECT = 11;
  static int MENU_BACK = 12;
  static int POWERUP_LIFE = 13;
  static int SHIP_FIRE_LAME = 14;
}

namespace GUI
{
  const static int GUITopBackgroundHeight = 32;
}

#ifndef HIGH_SCORE
#define HIGH_SCORE

struct HighScore
{
  unsigned int Score;
  unsigned int Wave;
};

#endif // HIGH_SCORE

#endif // GLOBALS_H_INCLUDED
