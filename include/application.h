#ifndef APPLICATION_H
#define APPLICATION_H

#include "globals.h"
#include "logger.h"
#include "gametime.h"
#include "video_system.h"
#include "texture_manager.h"
#include "input_manager.h"
#include "ship.h"
#include "asteroid.h"
#include "vector2pair.h"

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

    const int _screenWidth = 1280;
    const int _screenHeight = 600;
    const int _maxAsteroids = 1;

    bool _running;

    Sprite _background;
    Ship _ship;

    void LoadBackground();
    void InitAsteroids();
    void ProcessCollisions();
    Vector2Pair ProjectPolygon(std::vector<SDL_Point>& polygon, Vector2& axe);

    std::vector<std::unique_ptr<Asteroid>> _asteroids;
};

#endif // APPLICATION_H
