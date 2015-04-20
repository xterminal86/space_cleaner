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
#include "animations_pool.h"
#include "explosion.h"
#include "ship.h"
#include "star.h"
#include "asteroid.h"
#include "powerup.h"
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
    const int _maxExplosions = 20;
    const int _backgroundStars = 100;
    const int _maxLives = 0;
    const int _maxAsteroidInstances = 10;
    const int _spawnSpread = 100;
    const int _fancyTextColorChangeSpeed = 5;
    const int _powerupsPoolSize = 10;

    const double _spawnTimeMeterLength = 20.0;
    const double _bigAsteroidExplosionScale = 1.5;

    unsigned int _score;
    unsigned int _highScore;
    unsigned int _highWave;
    unsigned int _timePassed;
    unsigned int _currentSpawnRate;
    int _fancyColorCounter;
    unsigned int _fancyColorPhase;
    int _fancyColorDelta;
    int _asteroidInstances;

    double _guiSpawnRateNumber;
    double _guiTimeToSpawnNumber;

    std::string _guiSpawnTimeString;

    bool _quitFlag;
    bool _keyPressed;
    bool _scoreWritten;

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

    Ship _ship;

    SDL_Color _fancyTextColor;

    void LoadBackground();
    void DrawBackground();
    void InitAsteroids();
    void ProcessCollisions();
    void HandleShipCollision(Asteroid* collidedAsteroid);
    void ProcessExplosions();
    void InitGUI();
    void InitPowerups();
    void ProcessPowerups();
    void DrawGUI();
    void SpawnAsteroid(int x, int y);
    void CleanAsteroids();
    void CalculateFancyTextColor();
    void TryToSpawnAsteroid();
    void TryToSpawnPowerup(int x, int y);
    void RestartGame();

    void HandleEvents();
    void Update();
    void Draw();

    std::vector<std::unique_ptr<Asteroid>> _asteroids;
    std::vector<Star> _stars;
    std::vector<Vector2> _spawnPoints;
    std::vector<Powerup> _powerupsPool;

    AnimationsPool _asteroidExplosion;
    AnimationsPool _shipExplosion;
    AnimationsPool _spawnAnimation;

    Explosion _shipDebris;
};

#endif // MAINSTATE_H
