#ifndef APPLICATION_H
#define APPLICATION_H

#include "globals.h"
#include "logger.h"
#include "gametime.h"
#include "video_system.h"
#include "texture_manager.h"
#include "input_manager.h"
#include "bitmapfont.h"
#include "animations_pool.h"
#include "ship.h"
#include "asteroid.h"
#include "vector2pair.h"
#include "util.h"

struct Star
{
private:
  bool _active;

  SDL_Rect _srcRect;
  SDL_Rect _dstRect;

  PNGLoader* _imageRef;
  double _scaleFactor;
  double _alpha;
  double _alphaIncrement;
  Vector2 _position;
  SDL_Color _color;
  int _pauseMs;
  int _msPassed;

public:

  void Init(PNGLoader* imageRef)
  {
    _imageRef = imageRef;

    _srcRect.x = 0;
    _srcRect.y = 0;
    _srcRect.w = _imageRef->Width();
    _srcRect.h = _imageRef->Height();

    _dstRect.x = 0;
    _dstRect.y = 0;
    _dstRect.w = _imageRef->Width();
    _dstRect.h = _imageRef->Height();

    /*
    SDL_Color c;

    c.r = rand() % 200 + 55;
    c.g = rand() % 200 + 55;
    c.b = rand() % 200 + 55;
    c.a = 0;
    */

    SDL_Color c;

    c.r = 255;
    c.g = 255;
    c.b = 255;
    c.a = 0;

    _color = c;
    _scaleFactor = 0.15 / (double)(rand() % 2 + 1);
    _alpha = 0;
    _alphaIncrement = rand() % 5 + 1;
    //_alphaIncrement = 4.0 / (double)(rand() % 10 + 1);

    _msPassed = 0;

    _active = true;

    SetRandomPosition();
  }

  void Pulse()
  {
    if (!_active)
    {
      _msPassed += GameTime::Get().DeltaTime();

      if (_msPassed > _pauseMs)
      {
        _msPassed = 0;
        _active = true;
        SetRandomPosition();
      }

      return;
    }

    _dstRect.x = _position.X() - (_imageRef->Width() * _scaleFactor) / 2;
    _dstRect.y = _position.Y() - (_imageRef->Height() * _scaleFactor) / 2;
    _dstRect.w = _imageRef->Width() * _scaleFactor;
    _dstRect.h = _imageRef->Height() * _scaleFactor;

    _alpha += _alphaIncrement;

    if (_alpha > 255)
    {
      _alpha = 255;
      _alphaIncrement = -_alphaIncrement;
    }

    if (_alpha < 0)
    {
      _alpha = 0;
      _alphaIncrement = -_alphaIncrement;
      _active = false;
      //SetRandomPosition();
    }

    _color.a = _alpha;

    SDL_SetTextureColorMod(_imageRef->Texture(), _color.r, _color.g, _color.b);
    SDL_SetTextureAlphaMod(_imageRef->Texture(), _color.a);

    int res = SDL_RenderCopy(VideoSystem::Get().Renderer(), _imageRef->Texture(), &_srcRect, &_dstRect);
    if (res != 0) Logger::Get().LogPrint("(warning) Render copy error!\nReason: %s\n", SDL_GetError());
  }

  void SetRandomPosition()
  {
    int x = rand() % VideoSystem::Get().ScreenDimensions().x;
    int y = rand() % VideoSystem::Get().ScreenDimensions().y;

    _position.Set(x, y);

    //_pauseMs = rand() % 2000 + 3000;
    _pauseMs = rand() % 1000 + 1000;
  }
};

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
    const int _maxExplosions = 20;
    const int _backgroundStars = 100;
    const double _bigAsteroidExplosionScale = 2.0;

    unsigned int _score;

    bool _running;
    bool _shipHit;

    Sprite _backgroundStar;
    Sprite _background;

    Ship _ship;

    void LoadBackground();
    void DrawBackground();
    void InitAsteroids();
    void ProcessCollisions();

    std::vector<std::unique_ptr<Asteroid>> _asteroids;
    std::vector<Star> _stars;

    AnimationsPool _explosions;
};

#endif // APPLICATION_H
