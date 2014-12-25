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

    std::string _imagesFilename = "images.txt";
    std::string _logFilename = "output_log.txt";

    bool _running;
};

#endif // APPLICATION_H
