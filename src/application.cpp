#include "application.h"

Application::Application()
{
  _logger->Init(GlobalStrings::LogFilename);
  _videoSystem->Init(_screenWidth, _screenHeight);
  _textureManager->Init(GlobalStrings::ImagesFilename, GlobalStrings::RelationFilename);
  _bitmapFont->Init(GlobalStrings::BitmapFontFilename);

  _asteroidExplosion.Init(GlobalStrings::ExplosionSpriteFilename, _maxExplosions, 25);
  _shipExplosion.Init(GlobalStrings::ExplosionSpriteShipFilename, 1, 25);

  _shipHit = false;

  _score = 0;
}

Application::~Application()
{
  Logger::Get().LogPrint("Closing application...\n");

  _asteroids.clear();
}

void Application::Start()
{
  double shipAngle = 0.0;

  _running = true;

  SDL_Renderer* renderer = VideoSystem::Get().Renderer();

  LoadBackground();
  InitAsteroids();

  int bgx = _screenWidth / 2;
  int bgy = _screenHeight / 2;

  _ship.Init(0, 0);
  _ship.GetSprite().SetScaleFactor(1.0);
  _ship.Move(300, 300);

  int res = TextureManager::Get().FindTextureByRole(GlobalStrings::ShipBigRole);
  if (res != -1)
  {
    PNGLoader* imageRef = TextureManager::Get().GetTextureWrapper(res);
    _shipDebris.Init(imageRef, 10, 1500);
  }

  //_shipDebris.Init(_ship.GetSprite().ImageWrapper(), 10, 2000);
  //_shipDebris.SetScale(2.0);

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

    if (keyboardState[SDL_SCANCODE_RETURN] && !_ship.Active())
    {
      _ship.SetActive(true);
    }

    if (_ship.Active())
    {
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
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    _background.Draw(bgx, bgy);

    DrawBackground();

    _ship.ComputeBullets();

    if (_ship.Active())
    {
      if (shipAngle > 360) shipAngle -= 360;
      if (shipAngle < 0) shipAngle += 360;

      _ship.Rotate(shipAngle);
      _ship.Move();
      _ship.Draw(true);
      //_ship.Draw(false);
    }

    for (int i = 0; i < _asteroids.size(); i++)
    {
      if (!_asteroids[i].get()->Active())
      {
        continue;
      }

      _asteroids[i].get()->Compute();
      _asteroids[i].get()->Draw(true, true);
      //_asteroids[i].get()->Draw(false, false);
    }

    ProcessCollisions();

    if (_shipHit)
    {
      _shipHit = false;
      _ship.SetSpeed(0.0);
      _ship.SetActive(false);
      _shipExplosion.Play(_ship.Position().X(), _ship.Position().Y(), 1.0);
      _shipDebris.Play(_ship.Position());
    }

    ProcessExplosions();

    DrawGUI();

    SDL_RenderPresent(renderer);

    GameTime::Get().MeasureAfter();
  }
}

// ==================== Private Methods =================== //

void Application::LoadBackground()
{
  int textureId = TextureManager::Get().FindTextureByRole(GlobalStrings::BackgroundStarRole);
  if (textureId != -1)
  {
    _backgroundStar.Init(textureId);
  }

  for (int i = 0; i < _backgroundStars; i++)
  {
    Star s;

    s.Init(_backgroundStar.ImageWrapper());

    _stars.push_back(s);
  }

  textureId = TextureManager::Get().FindTextureByRole(GlobalStrings::BackgroundRole);
  if (textureId != -1)
  {
    _background.Init(textureId);
  }
}

void Application::DrawBackground()
{
  for (auto& i : _stars)
  {
    i.Pulse();
  }
}

void Application::InitAsteroids()
{
  int screenx = VideoSystem::Get().ScreenDimensions().x;
  int screeny = VideoSystem::Get().ScreenDimensions().y;

  for (int i = 0; i < GameMechanic::MaxAsteroids; i++)
  {
    Vector2 pos;

    Util::CreateRandomPosition(pos, screenx, screeny);

    _asteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(pos, 0, &_asteroids)));
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
        for (int i = 0; i < _asteroids.size(); i++)
        {
          if (_asteroids[i].get()->Active() && Util::TestIntersection(_asteroids[i].get()->GetSprite(), bullet.get()->GetSprite()))
          {
            _asteroidExplosion.Play(_asteroids[i].get()->Position().X(), _asteroids[i].get()->Position().Y(), _bigAsteroidExplosionScale / (_asteroids[i].get()->CurrentBreakdownLevel() + 1));

            _asteroids[i].get()->ProcessCollision();
            //asteroid.get()->GetSprite().SetColor(Colors::Red);
            //_bitmapFont->SetTextColor(255, 255, 255, 255);
            //_bitmapFont->SetScale(0.5f);
            //_bitmapFont->Printf(_asteroids[i].get()->Position().X(), _asteroids[i].get()->Position().Y(), BitmapFont::AlignLeft, "Asteroid hit: %f", bullet.get()->Angle());

            bullet.get()->SetActive(false);

            break;
          }
        }
      }
    }
  }

  if (_ship.Active())
  {
    _shipHit = false;

    for (auto &asteroid : _asteroids)
    {
      if (asteroid.get()->Active() && Util::TestIntersection(asteroid.get()->GetSprite(), _ship.GetSprite()))
      {
          _shipHit = true;
          break;
      }
    }
  }
}

void Application::ProcessExplosions()
{
  _asteroidExplosion.Process();
  _shipExplosion.Process();
  _shipDebris.Process();
}

void Application::DrawGUI()
{
  _bitmapFont->SetTextColor(0, 255, 0, 255);
  _bitmapFont->SetScale(0.5f);
  _bitmapFont->Printf(0, 0, BitmapFont::AlignLeft, "Score:%u", _score);

  int x = (int)_ship.Position().X();
  int y = (int)_ship.Position().Y();

  _bitmapFont->SetTextColor(255, 0, 0, 255);
  _bitmapFont->SetScale(0.5f);
  _bitmapFont->Printf(x, y, BitmapFont::AlignLeft, "Ship hit: %i", _shipHit);
}
