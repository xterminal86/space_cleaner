#ifndef APPLICATION_H
#define APPLICATION_H

#include "globals.h"
#include "logger.h"
#include "gametime.h"
#include "video_system.h"
#include "texture_manager.h"
#include "input_manager.h"
#include "bitmapfont.h"
#include "ship.h"
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
    const int _maxAsteroids = 3;

    bool _running;
    bool _shipHit;

    Sprite _background;
    Ship _ship;

    void LoadBackground();
    void InitAsteroids();
    void ProcessCollisions();

    std::vector<std::unique_ptr<Asteroid>> _asteroids;
};

#endif // APPLICATION_H
