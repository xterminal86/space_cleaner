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
    const double _bigAsteroidExplosionScale = 1.5;

    unsigned int _score;

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

    std::vector<std::unique_ptr<Asteroid>> _asteroids;
    std::vector<Star> _stars;

    AnimationsPool _asteroidExplosion;
    AnimationsPool _shipExplosion;
    Explosion _shipDebris;

    Uint8* _keyboardState;
};

#endif // APPLICATION_H
