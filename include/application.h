#ifndef APPLICATION_H
#define APPLICATION_H

#include "globals.h"
#include "logger.h"
#include "gametime.h"
#include "video_system.h"
#include "texture_manager.h"
#include "input_manager.h"
#include "ship.h"

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

    // FIXME: Move to Ship class and access through methods
    const double _rotateSpeed = 0.15;
    const double _accelerationSpeed = 0.01;

    bool _running;

    Sprite _background;

    void LoadBackground();
};

#endif // APPLICATION_H
