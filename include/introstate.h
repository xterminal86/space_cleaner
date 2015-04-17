#ifndef INTROSTATE_H
#define INTROSTATE_H

#include "gamestate.h"

#include "sprite.h"
#include "star.h"
#include "input_manager.h"
#include "bitmapfont.h"
#include "sound_system.h"
#include "asteroid.h"

class IntroState : public GameState
{
  public:
    static IntroState& Get()
    {
      static IntroState _instance;
      return _instance;
    }

    void Init(Application* game) override;
    void Cleanup() override;

    void Pause() override;
    void Resume() override;

    void HandleEvents(Application* game) override;
    void Update(Application* game) override;
    void Draw(Application* game) override;

  protected:
    IntroState();
    virtual ~IntroState();

  private:
    const int _spawnSpread = 100;
    const int _backgroundStars = 100;
    const int _screenWidth = 1280;
    const int _screenHeight = 600;
    const int _asteroidsCount = 5;
    const double _menuItemDefaultScale = 2.0;
    const double _menuItemMinimalScale = 1.5;
    const double _menuItemMaximumScale = 2.25;

    double _menuItemScaleIncrement = 0.005;

    Logger* _logger = &Logger::Get();
    VideoSystem* _videoSystem = &VideoSystem::Get();
    TextureManager* _textureManager = &TextureManager::Get();
    InputManager* _inputManager = &InputManager::Get();
    BitmapFont* _bitmapFont = &BitmapFont::Get();
    SoundSystem* _soundSystem = &SoundSystem::Get();

    Uint8* _keyboardState;

    Sprite _backgroundStar;
    Sprite _background;

    std::vector<std::unique_ptr<Asteroid>> _asteroids;
    std::vector<Star> _stars;
    std::vector<Vector2> _spawnPoints;

    std::string _version;

    std::vector<std::string> _menuStrings;
    std::map<int, void(IntroState::*)()> _menuMap;

    bool _keyPressed;

    int _menuIndex;

    int _screenSizeX;
    int _screenSizeY;

    int _backgroundX;
    int _backgroundY;

    double _menuItemScaleFactor;

    void LoadBackground();
    void DrawBackground();
    void DrawAsteroids();
    void DrawMenu();
    void ExitGame();
};

#endif // INTROSTATE_H
