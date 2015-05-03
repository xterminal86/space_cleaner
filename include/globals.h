#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include <fstream>
#include <sstream>
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
  // Text keys for searching image in the image dictionary

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

  // Name of the config file where settings will be stored
  static std::string ConfigFilename = "config.txt";

  // Name of the file with paths to all images
  static std::string ImagesFilename = "images.txt";
  // Name of the file with paths to all sprite animations and their parameters: total frames, frames in row and col
  static std::string AnimationsFilename = "animations.txt";
  // Name of the file with console output
  static std::string LogFilename = "output_log.txt";
  // Name of the file with corresponcdence of sprite id and its "role" - text key from above here
  static std::string RelationFilename = "relation.txt";

  static std::string HighScoresFilename = "highscores.dat";

  // File with all sounds
  static std::string SoundsFilename = "sounds.txt";
  // File with all music
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
  // Number of asteroid breakdowns after it has been hit
  static const int AsteroidMaxBreakdownLevel = 2;
  // Number of asteroids created after breakdown
  static const int AsteroidBreakdownChildren = 4;
  // Asteroid scale increment after breakdown
  static const double AsteroidBreakdownScaleFactor = 1.5;
  // Initial asteroid scale
  static const double AsteroidInitialScaleFactor = 0.5;
  // Maximum flying speed of an asteroid
  static const double AsteroidMaxSpeed = 0.25;
  // Time after which new asteroid will spawn
  // at the start of the game or after player has been killed
  static const int StartingSpawnRateMs = 10000;
  // Fastest spawning time interval
  static const int MaxSpawnRateMs = 1000;
  // Starting time will decrement by this value every spawn
  static const int SpawnRateDeltaMs = 100;
  // Shield restores one bar after this time
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

  // Delay of spawned powerup after it disappears
  static const int PowerupLifetimeMs = 5000;

  // One bullet is fired every these milliseconds (on auto weapon)
  static const int AutoFireRateMs = 100;

  // Big asteroid sprite explosion scale
  static const double BigAsteroidExplosionScale = 1.5;

  // Number of asteroids to destroy (value) after given level (key) is reached
  static std::map<int, int> ExperienceMap =
  {
    {0, 10}, {1, 20}, {2, 40}, {3, 80}, {4, 120},
    {5, 150}, {6, 200}
  };
}

// If config.dat wasn't found, we assume these default values
// Game actually uses only music and sound volume values.
// Others are ignored for now.
namespace DefaultConfigValues
{
  static std::map<std::string, int> DefaultConfigPairs =
  {
    {"sound_volume", 100}, {"music_volume", 50}, {"screen_width", 1280},
    {"screen_height", 1024}, {"fullscreen", 0}
  };
}

// Powerups IDs
namespace Powerups
{
  static const int SHIELD_POWERUP = 0;
  static const int HEALTH_POWERUP = 1;
  static const int LIFE_POWERUP = 2;
}

// Bullets IDs
namespace Bullets
{
  static const int BULLET_LAME = 0;
  static const int BULLET_ONE_SHOT = 1;
  static const int BULLET_FIRE = 2;
  static const int BULLET_FIRE_AUTO = 3;
  static const int BULLET_TOTAL_TYPES = 4;

  // What level gets you what weapon
  static std::map<int, int> LevelToWeaponsMap =
  {
    {0, BULLET_LAME},
    {2, BULLET_ONE_SHOT},
    {4, BULLET_FIRE},
    {6, BULLET_FIRE_AUTO}
  };

  // What bullets have what sprites
  static std::map<int, std::string> BulletsToRolesMap =
  {
    {BULLET_LAME, GlobalStrings::BulletLameRole},
    {BULLET_ONE_SHOT, GlobalStrings::BulletShotSingleRole},
    {BULLET_FIRE, GlobalStrings::BulletShotFireRole},
    {BULLET_FIRE_AUTO, GlobalStrings::BulletShotFireRole}
  };
}

namespace Music
{
  static const int MaxMusicVolume = 100;

  static int TITLE_MUSIC_ID = 0;
}

namespace Sounds
{
  static const int MaxSoundVolume = 200;

  static const int POWERUP_HEALTH = 0;
  static const int SHIP_FIRE_AUTO = 1;
  static const int SHIP_EXPLODE = 2;
  static const int SHIP_HIT = 3;
  static const int POWERUP_SHIELD = 4;
  static const int GAME_OVER = 5;
  static const int SHIELD_HIT_ENERGY = 6;
  static const int ASTEROID_HIT_BIG = 7;
  static const int ASTEROID_SPAWN = 8;
  static const int ASTEROID_HIT_SMALL = 9;
  static const int MENU_MOVE = 10;
  static const int MENU_SELECT = 11;
  static const int MENU_BACK = 12;
  static const int POWERUP_LIFE = 13;
  static const int SHIP_FIRE_LAME = 14;
  static const int ASTEROID_HIT = 15;
  static const int POWERUP_SPAWN = 16;
  static const int SHIP_FIRE_SINGLE = 17;
  static const int ASTEROID_HIT2 = 18;
  static const int WEAPON_UPGRADE = 19;

  // What bullets produce what sounds
  static std::map<int, int> ShotSoundsMap =
  {
    {Bullets::BULLET_LAME,          SHIP_FIRE_LAME},
    {Bullets::BULLET_ONE_SHOT,      SHIP_FIRE_SINGLE},
    {Bullets::BULLET_FIRE,          SHIP_FIRE_AUTO},
    {Bullets::BULLET_FIRE_AUTO,     SHIP_FIRE_AUTO}
  };
}

// Some hardcoded values for GUI
namespace GUI
{
  // Height of top padding on the game screen with GUI elements
  static const int GUITopBackgroundHeight = 32;
  // Length of progress bar that shows when next asteroid spawn occurs
  static const double SpawnTimeMeterLength = 20.0;
}

// Sprite animations IDs
namespace AnimationsIds
{
  static const int EXPLOSION_ASTEROID = 0;
  static const int EXPLOSION_SHIP = 1;
  static const int SPAWN_SMALL = 2;
  static const int SPAWN_BIG = 3;
  static const int BULLET_HIT = 4;
  static const int BULLET_HIT2 = 5;
}

// IDs of title screen menu C++ classes
namespace Forms
{
  static const int MAIN_TITLE = 0;
  static const int OPTIONS = 1;
  static const int HOW_TO_PLAY = 2;
  static const int HIGH_SCORES = 3;
}

#ifndef HIGH_SCORE
#define HIGH_SCORE

// For storing high score
struct HighScore
{
  unsigned int Score;
  unsigned int Wave;
};

#endif // HIGH_SCORE

#endif // GLOBALS_H_INCLUDED
