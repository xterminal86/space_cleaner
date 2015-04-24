#ifndef MAINSTATE_H
#define MAINSTATE_H

#include "gamestate.h"

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
  protected:
    MainState();
    virtual ~MainState();

  private:
    Logger* _logger = &Logger::Get();
    VideoSystem* _videoSystem = &VideoSystem::Get();
    TextureManager* _textureManager = &TextureManager::Get();
    InputManager* _inputManager = &InputManager::Get();
    BitmapFont* _bitmapFont = &BitmapFont::Get();
    SoundSystem* _soundSystem = &SoundSystem::Get();

    Uint8* _keyboardState;

    const int _screenWidth = 1280;
    const int _screenHeight = 600;
    const int _backgroundStars = 100;
    const int _startingLives = 1;
    const int _maxLivesLimit = 3;
    const int _maxAsteroidInstances = 10;
    const int _spawnSpread = 100;
    const int _fancyTextColorChangeSpeed = 5;

    unsigned int _score;
    unsigned int _timePassed;
    unsigned int _currentSpawnRate;
    unsigned int _autoFireTimePassed;
    int _fancyColorCounter;
    unsigned int _fancyColorPhase;
    int _fancyColorDelta;
    int _asteroidInstances;

    double _guiSpawnRateNumber;
    double _guiTimeToSpawnNumber;

    std::string _guiSpawnTimeString;
    std::string _shieldPointsBar;
    std::string _hitPointsBar;

    bool _quitFlag;
    bool _keyPressed;
    bool _scoreWritten;
    bool _autoFireValid;

    int _currentLives;
    int _waveCounter;

    int _hitPointsColorDelta;
    int _shipHitPointsHalf;
    int _shieldColorAlphaDelta;
    int _screenSizeX;
    int _screenSizeY;

    int _backgroundX;
    int _backgroundY;

    bool _fireTrigger;

    Sprite _backgroundStar;
    Sprite _background;

    Sprite _guiHeart;
    Sprite _guiShield;
    Sprite _guiLives;
    Sprite _guiWeaponFrame;
    Sprite _guiBlackBack;
    Sprite _guiDivider;
    Sprite _guiExtraLifeOutline;

    Ship _ship;

    SDL_Color _fancyTextColor;

    void LoadBackground();
    void DrawBackground();
    void InitAsteroids();
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
