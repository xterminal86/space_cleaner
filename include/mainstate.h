#ifndef MAINSTATE_H
#define MAINSTATE_H

#include "gamestate.h"

#include "config.h"
#include "logger.h"
#include "gametime.h"
#include "video_system.h"
#include "texture_manager.h"
#include "input_manager.h"
#include "bitmapfont.h"
#include "sound_system.h"
#include "explosion.h"
#include "ship.h"
#include "star.h"
#include "asteroid.h"
#include "animations_manager.h"
#include "powerups_manager.h"
#include "vector2pair.h"
#include "util.h"

// Main game state class
class MainState : public GameState
{
  public:
    static MainState& Get()
    {
      static MainState _instance;
      return _instance;
    }

    void Init(Application* game) override;
    void Cleanup() override;

    void Pause() override;
    void Resume() override;

    void HandleEvents(Application* game) override;
    void Update(Application* game) override;
    void Draw(Application* game) override;

    void TryToSpawnPowerup(int x, int y);
    void AddScore(unsigned int value) { _score += value; }
    void AddKills(int value) { _ship.AddKills(value); }
  private:
    MainState();
    virtual ~MainState();
    MainState(const MainState&);
    MainState& operator= (const MainState&);

    Logger* _logger = &Logger::Get();
    VideoSystem* _videoSystem = &VideoSystem::Get();
    TextureManager* _textureManager = &TextureManager::Get();
    InputManager* _inputManager = &InputManager::Get();
    BitmapFont* _bitmapFont = &BitmapFont::Get();
    SoundSystem* _soundSystem = &SoundSystem::Get();

    Uint8* _keyboardState;

    const int _backgroundStars = 100;
    const int _startingLives = 0;
    const int _maxLivesLimit = 3;
    const int _maxAsteroidInstances = 20;
    const int _spawnSpread = 100;
    const int _fancyTextColorChangeSpeed = 5;

    unsigned int _score;
    unsigned int _timePassed;
    unsigned int _shieldRechargeTimePassed;
    unsigned int _currentSpawnRate;
    unsigned int _autoFireTimePassed;
    int _fancyColorCounter;
    unsigned int _fancyColorPhase;
    int _fancyColorDelta;

    double _guiSpawnRateNumber;
    double _guiTimeToSpawnNumber;

    std::string _guiSpawnTimeString;
    std::string _shieldPointsBar;
    std::string _hitPointsBar;

    // Checked to show quit dialog
    bool _quitFlag;
    // Used to get once-per-press key behaviour
    bool _keyPressed;
    // It is set after we write high score
    bool _scoreWritten;
    // Flag to check if we can fire another shot on auto mode
    bool _autoFireValid;

    // Number of extra lives
    int _currentLives;
    // Number of asteroid spawns passed
    int _waveCounter;

    // Decrement of hit points bar color
    int _hitPointsColorDelta;
    // If hitpoints drop below this value, we can spawn a health powerup
    int _shipHitPointsHalf;
    // Shield alpha decrement
    int _shieldColorAlphaDelta;

    // Shortcuts for VideoSystem::Get().ScreenDimensions().x and y
    int _screenSizeX;
    int _screenSizeY;

    // Size of the background image
    int _backgroundX;
    int _backgroundY;

    // Set when we hit SPACE
    bool _fireTrigger;

    Sprite _backgroundStar;
    Sprite _background;

    Sprite _guiHeart;
    Sprite _guiShield;
    Sprite _guiLives;
    Sprite _guiWeaponFrame;
    // Black padding at the top of the screen
    Sprite _guiBlackBack;
    Sprite _guiDivider;
    Sprite _guiExtraLifeOutline;

    Ship _ship;

    // Text color that changes from red to violet (cycled)
    SDL_Color _fancyTextColor;

    void LoadBackground();
    void DrawBackground();
    void ProcessCollisions();
    void HandleShipCollision(Asteroid* collidedAsteroid);
    void ProcessExplosions();
    void InitGUI();
    void ProcessPowerups();
    void MakeBars();
    void DrawGUI();
    void SpawnAsteroid(int x, int y);
    void CleanAsteroids();
    void CalculateFancyTextColor();
    void TryToSpawnAsteroid();
    void RestartGame();

    void HandleEvents();
    void Update();
    void Draw();

    std::vector<std::unique_ptr<Asteroid>> _asteroids;
    std::vector<Star> _stars;
    std::vector<Vector2> _spawnPoints;

    Explosion _shipDebris;
};

#endif // MAINSTATE_H
