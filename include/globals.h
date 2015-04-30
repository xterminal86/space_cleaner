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
  static std::string BulletLameRole = "shot_lame";
  static std::string BulletShotSingleRole = "shot_single";
  static std::string BulletShotFireRole = "shot_fire";
  static std::string DefaultParticleRole = "default_particle";
  static std::string FontRole = "font";
  static std::string ShipShieldRole = "ship_shield";
  static std::string GUIShieldRole = "gui_shield";
  static std::string GUIHeartRole = "gui_heart";
  static std::string GUIWeaponFrame = "gui_weapon_frame";
  static std::string GUIBlackBack = "gui_black_back";
  static std::string GUIDivider = "gui_divider";
  static std::string GUIShipOutline = "gui_ship_outline";

  static std::string ConfigFilename = "config.txt";

  static std::string ImagesFilename = "images.txt";
  static std::string AnimationsFilename = "animations.txt";
  static std::string LogFilename = "output_log.txt";
  static std::string RelationFilename = "relation.txt";

  static std::string HighScoresFilename = "highscores.dat";

  static std::string SoundsFilename = "sounds.txt";
  static std::string MusicListFilename = "music.txt";

  static std::string HowToPlayString = \
  "CLEAN THE SPACE BY SHOOTING DOWN FLYING ASTEROIDS!\n\n\
Your starting weapon is capable of destroying only small asteroids,\n\
so you have to carefully use your shield to break down big ones.\n\n\
- Move and turn using WASD, shoot by tapping SPACE ;-)\n\
- Your shield will absorb any amount of damage\n\
- Damage to ship's hull depends on the size of the incoming asteroid\n\
- Shield recharges with time";
}

namespace Math
{
  static const double PIOVER180 = 0.01745329251;
}

namespace GameMechanic
{
  static const int AsteroidMaxBreakdownLevel = 2;
  static const int AsteroidBreakdownChildren = 4;
  static const double AsteroidBreakdownScaleFactor = 1.5;
  static const double AsteroidInitialScaleFactor = 0.5;
  static const double AsteroidMaxSpeed = 0.25;
  static const int StartingSpawnRateMs = 10000;
  static const int MaxSpawnRateMs = 1000;
  static const int SpawnRateDeltaMs = 100;
  static const int ShieldRechargeMs = 10000;

  // For visual debug: determines length of direction vectors
  // Because they are normalized, we have to extend them in order to see.
  static const int DirectionResolution = 100;

  // In particular, determines the values below AsteroidMaxSpeed
  // The more - the less speed values can be.
  //
  // TLDR: goes in denominator of asteroid random speed generation.
  static const int RandomSpeedSpread = 5;

  // Determines spread of rotation increment fraction. The more - the less increment value can possibly be.
  // See Util::CreateRandomRotation.
  //
  // TLDR: this value goes in denominator.
  static const int RandomRotationSpeedSpread = 10;

  static const int PowerupLifetimeMs = 5000;

  static const int AutoFireRateMs = 100;

  static const double BigAsteroidExplosionScale = 1.5;

  static std::map<int, int> ExperienceMap =
  {
    {0, 10}, {1, 20}, {2, 40}, {3, 80}, {4, 120},
    {5, 150}, {6, 200}
  };
}

namespace DefaultConfigValues
{
  static std::map<std::string, int> DefaultConfigPairs =
  {
    {"sound_volume", 100}, {"music_volume", 50}, {"screen_width", 1280},
    {"screen_height", 1024}, {"fullscreen", 0}
  };
}

namespace Powerups
{
  static const int SHIELD_POWERUP = 0;
  static const int HEALTH_POWERUP = 1;
  static const int LIFE_POWERUP = 2;
}

namespace Bullets
{
  const static int BULLET_LAME = 0;
  const static int BULLET_ONE_SHOT = 1;
  const static int BULLET_FIRE = 2;
  const static int BULLET_FIRE_AUTO = 3;
  const static int BULLET_TOTAL_TYPES = 4;

  static std::map<int, int> LevelToWeaponsMap =
  {
    {0, BULLET_LAME},
    {2, BULLET_ONE_SHOT},
    {4, BULLET_FIRE},
    {6, BULLET_FIRE_AUTO}
  };

  static std::map<int, std::string> BulletsToRolesMap =
  {
    {BULLET_LAME, GlobalStrings::BulletLameRole},
    {BULLET_ONE_SHOT, GlobalStrings::BulletShotSingleRole},
    {BULLET_FIRE, GlobalStrings::BulletShotFireRole},
    {BULLET_FIRE_AUTO, GlobalStrings::BulletShotFireRole}
  };
}

namespace Sounds
{
  static int POWERUP_HEALTH = 0;
  static int SHIP_FIRE_AUTO = 1;
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
  static int ASTEROID_HIT = 15;
  static int POWERUP_SPAWN = 16;
  static int SHIP_FIRE_SINGLE = 17;
  static int ASTEROID_HIT2 = 18;
  static int WEAPON_UPGRADE = 19;

  static std::map<int, int> ShotSoundsMap =
  {
    {Bullets::BULLET_LAME,          SHIP_FIRE_LAME},
    {Bullets::BULLET_ONE_SHOT,      SHIP_FIRE_SINGLE},
    {Bullets::BULLET_FIRE,          SHIP_FIRE_AUTO},
    {Bullets::BULLET_FIRE_AUTO,     SHIP_FIRE_AUTO}
  };
}

namespace GUI
{
  const static int GUITopBackgroundHeight = 32;
  static const double SpawnTimeMeterLength = 20.0;
  static const double KillsMeterLength = 10.0;
}

namespace AnimationsIds
{
  const static int EXPLOSION_ASTEROID = 0;
  const static int EXPLOSION_SHIP = 1;
  const static int SPAWN_SMALL = 2;
  const static int SPAWN_BIG = 3;
  const static int BULLET_HIT = 4;
  const static int BULLET_HIT2 = 5;
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
