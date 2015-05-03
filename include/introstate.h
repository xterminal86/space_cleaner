#ifndef INTROSTATE_H
#define INTROSTATE_H

#include "gamestate.h"

#include "sprite.h"
#include "star.h"
#include "input_manager.h"
#include "bitmapfont.h"
#include "sound_system.h"
#include "asteroid.h"
#include "animations_manager.h"
#include "powerups_manager.h"
#include "config.h"
#include "forms_manager.h"

// Title screen
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

  private:
    IntroState();
    virtual ~IntroState();
    IntroState(const IntroState&);
    IntroState& operator= (const IntroState&);

    const int _spawnSpread = 100;
    const int _backgroundStars = 100;
    const int _asteroidsCount = 20;

    Application* _gameRef;

    Logger* _logger = &Logger::Get();
    VideoSystem* _videoSystem = &VideoSystem::Get();
    TextureManager* _textureManager = &TextureManager::Get();
    Config* _config = &Config::Get();
    InputManager* _inputManager = &InputManager::Get();
    BitmapFont* _bitmapFont = &BitmapFont::Get();
    SoundSystem* _soundSystem = &SoundSystem::Get();
    AnimationsManager* _animationsManager = &AnimationsManager::Get();
    PowerupsManager* _powerupsManager = &PowerupsManager::Get();
    FormsManager* _formsManager = &FormsManager::Get();

    Uint8* _keyboardState;

    Sprite _backgroundStar;
    Sprite _background;

    std::vector<std::unique_ptr<Asteroid>> _asteroids;
    std::vector<Star> _stars;
    std::vector<Vector2> _spawnPoints;

    // Used to get once-per-press key behaviour
    bool _keyPressed;

    // Shortcuts for VideoSystem::Get().ScreenDimensions().x and y
    int _screenSizeX;
    int _screenSizeY;

    // Size of background image
    int _backgroundX;
    int _backgroundY;

    void LoadBackground();
    void DrawBackground();
    void DrawAsteroids();
};

#endif // INTROSTATE_H
