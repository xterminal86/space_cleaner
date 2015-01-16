#ifndef APPLICATION_H
#define APPLICATION_H

#include "globals.h"
#include "logger.h"
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
    const int _rotateSpeed = 100;
    const double _accelerationSpeed = 4;

    unsigned int _currentTime;
    unsigned int _lastTime;

    double _deltaTime;

    bool _running;

    Sprite _background;

    void LoadBackground();
};

#endif // APPLICATION_H
