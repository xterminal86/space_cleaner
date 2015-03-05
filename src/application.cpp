#include "application.h"

Application::Application()
{
  _logger->Init(GlobalStrings::LogFilename);
  _videoSystem->Init(_screenWidth, _screenHeight);
  _textureManager->Init(GlobalStrings::ImagesFilename, GlobalStrings::RelationFilename);
  _bitmapFont->Init(GlobalStrings::BitmapFontFilename);

  _shipHit = false;
}

Application::~Application()
{
  Logger::Get().LogPrint("Closing application...\n");

  _asteroids.clear();
}

void Application::LoadBackground()
{
  int textureId = TextureManager::Get().FindTextureByRole(GlobalStrings::BackgroundRole);
  if (textureId != -1)
  {
    _background.Init(textureId);
  }
}

void Application::InitAsteroids()
{
  int posx = 0, posy = 0;
  int screenx = VideoSystem::Get().ScreenDimensions().x;
  int screeny = VideoSystem::Get().ScreenDimensions().y;

  for (int i = 0; i < _maxAsteroids; i++)
  {
    posx = (rand() % screenx - 40) + 40;
    posy = (rand() % screeny - 40) + 40;

    _asteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(posx, posy)));
  }
}

void Application::ProcessCollisions()
{
  // Perform the following only if needed
  if (_ship.HasBulletsActive())
  {
    for (auto &bullet : _ship.GetBullets())
    {
      // Take into account only active bullets
      if (bullet.get()->Active())
      {
        for (auto &asteroid : _asteroids)
        {
          if (Util::TestIntersection(asteroid.get()->GetSprite(), bullet.get()->GetSprite()))
          {
            printf("%f ", bullet.get()->Angle());
          }
        }
      }
    }
  }

  _shipHit = false;

  for (auto &asteroid : _asteroids)
  {
    if (Util::TestIntersection(asteroid.get()->GetSprite(), _ship.GetSprite()))
    {
        _shipHit = true;
        break;
    }
  }
}

void Application::Start()
{
  srand(time(nullptr));

  double shipAngle = 0.0;

  _running = true;

  SDL_Renderer* renderer = VideoSystem::Get().Renderer();

  LoadBackground();
  InitAsteroids();

  int bgx = _screenWidth / 2;
  int bgy = _screenHeight / 2;

  _ship.Init(0, 0);
  _ship.Move(300, 300);

  bool fireTrigger = false;

  int tw, th;

  Uint8* keyboardState = nullptr;
  while (_running)
  {
    GameTime::Get().MeasureBefore();

    SDL_PumpEvents();

    keyboardState = (Uint8*)SDL_GetKeyboardState(nullptr);

    if (keyboardState[SDL_SCANCODE_ESCAPE])
    {
      _running = false;
    }

    if (keyboardState[SDL_SCANCODE_A])
    {
      shipAngle -= _ship.RotationSpeed * GameTime::Get().DeltaTime();
    }

    if (keyboardState[SDL_SCANCODE_D])
    {
      shipAngle += _ship.RotationSpeed * GameTime::Get().DeltaTime();
    }

    if (keyboardState[SDL_SCANCODE_W])
    {
      _ship.Accelerate(_ship.AccelerationSpeed * GameTime::Get().DeltaTime());
    }

    if (keyboardState[SDL_SCANCODE_SPACE])
    {
      if (!fireTrigger)
      {
        fireTrigger = true;
        _ship.Fire();
      }
    }

    if (!keyboardState[SDL_SCANCODE_SPACE])
    {
      fireTrigger = false;
    }

//    if (keyboardState[SDL_SCANCODE_S])
//    {
//      _ship.Accelerate(-_accelerationSpeed);
//    }

    if (!keyboardState[SDL_SCANCODE_W] && _ship.Speed() > 0.0)
    {
      _ship.Accelerate(-_ship.AccelerationSpeed * GameTime::Get().DeltaTime());
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    _background.Draw(bgx, bgy);

    _ship.Rotate(shipAngle);
    _ship.Move();

    _ship.ComputeBullets();
    _ship.Draw(true);

    for (int i = 0; i < _asteroids.size(); i++)
    {
      _asteroids[i].get()->Compute();
      _asteroids[i].get()->Draw(true, true);
    }

    ProcessCollisions();

    if (_shipHit)
    {

    }

    SDL_RenderPresent(renderer);

    GameTime::Get().MeasureAfter();
  }
}
