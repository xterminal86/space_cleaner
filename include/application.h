#ifndef APPLICATION_H
#define APPLICATION_H

#include "globals.h"
#include "logger.h"
#include "gametime.h"
#include "video_system.h"
#include "texture_manager.h"
#include "input_manager.h"
#include "bitmapfont.h"
#include "animations_pool.h"
#include "explosion.h"
#include "ship.h"
#include "star.h"
#include "asteroid.h"
#include "vector2pair.h"
#include "util.h"

class Application
{
  public:
    Application();
    virtual ~Application();
    void Start();
  protected:
  private:
    Logger* _logger = &Logger::Get();
    VideoSystem* _videoSystem = &VideoSystem::Get();
    TextureManager* _textureManager = &TextureManager::Get();
    InputManager* _inputManager = &InputManager::Get();
    BitmapFont* _bitmapFont = &BitmapFont::Get();

    const int _screenWidth = 1280;
    const int _screenHeight = 600;
    const int _maxExplosions = 20;
    const int _backgroundStars = 100;
    const int _maxLives = 3;
    const int _maxSpawnedAsteroids = 10;
    const int _spawnSpread = 100;

    const double _spawnTimeMeterLength = 20.0;
    const double _bigAsteroidExplosionScale = 1.5;

    unsigned int _score;
    unsigned int _highScore;
    unsigned int _timePassed;
    unsigned int _currentSpawnRate;

    double _guiSpawnRateNumber;
    double _guiTimeToSpawnNumber;

    std::string _guiSpawnTimeString;

    int _currentLives;
    int _waveCounter;

    int _hitPointsColorDelta;
    int _shipHitPointsHalf;
    int _shieldColorAlphaDelta;
    int _screenSizeX;
    int _screenSizeY;

    bool _running;
    bool _shipHit;
    bool _fireTrigger;

    Sprite _backgroundStar;
    Sprite _background;

    Sprite _guiHeart;
    Sprite _guiShield;
    Sprite _guiLives;

    Ship _ship;

    void LoadBackground();
    void DrawBackground();
    void InitAsteroids();
    void ProcessCollisions();
    void HandleCollisions();
    void ProcessExplosions();
    void ProcessInput();
    void InitGUI();
    void DrawGUI();
    void SpawnAsteroid(int x, int y);
    void CleanAsteroids();

    std::vector<std::unique_ptr<Asteroid>> _asteroids;
    std::vector<Star> _stars;
    std::vector<Vector2> _spawnPoints;

    AnimationsPool _asteroidExplosion;
    AnimationsPool _shipExplosion;
    Explosion _shipDebris;

    Uint8* _keyboardState;
};

#endif // APPLICATION_H
