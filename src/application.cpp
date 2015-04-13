#include "application.h"

Application::Application()
{
  _logger->Init(GlobalStrings::LogFilename);
  _videoSystem->Init(_screenWidth, _screenHeight);
  _textureManager->Init(GlobalStrings::ImagesFilename, GlobalStrings::RelationFilename);
  _bitmapFont->Init();

  _asteroidExplosion.Init(GlobalStrings::ExplosionSpriteFilename, _maxExplosions, 25);
  _shipExplosion.Init(GlobalStrings::ExplosionSpriteShipFilename, 1, 25);

  _shipHit = false;
  _fireTrigger = false;

  _score = 0;
  _highScore = 0;
  _currentLives = MaxLives;

  _keyboardState = nullptr;

  _hitPointsColorDelta = 255 / _ship.ShipMaxHitPoints;
  _hitPointsColorDelta *= 2;
  _shipHitPointsHalf = _ship.ShipMaxHitPoints / 2;
  _shieldColorAlphaDelta = 128 / _ship.ShieldMaxPoints;

  _screenSizeX = VideoSystem::Get().ScreenDimensions().x;
  _screenSizeY = VideoSystem::Get().ScreenDimensions().y;

  InitGUI();
}

Application::~Application()
{
  Logger::Get().LogPrint("Closing application...\n");

  _asteroids.clear();
}

void Application::Start()
{
  _running = true;

  SDL_Renderer* renderer = VideoSystem::Get().Renderer();

  LoadBackground();
  InitAsteroids();

  int bgx = _screenWidth / 2;
  int bgy = _screenHeight / 2;

  _ship.Init(0, 0);
  _ship.Scale(0.5);
  _ship.Move(300, 300);

  int res = TextureManager::Get().FindTextureByRole(GlobalStrings::ShipBigRole);
  if (res != -1)
  {
    PNGLoader* imageRef = TextureManager::Get().GetTextureWrapper(res);
    _shipDebris.Init(imageRef, 10, 1500);
  }

  //_shipDebris.Init(_ship.GetSprite().ImageWrapper(), 10, 2000);
  //_shipDebris.SetScale(2.0);

  int tw, th;

  while (_running)
  {
    GameTime::Get().MeasureBefore();

    ProcessInput();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    _background.Draw(bgx, bgy);

    DrawBackground();

    _ship.ComputeBullets();

    _ship.Move();
    _ship.Draw(false);
    //_ship.Draw(true);

    for (int i = 0; i < _asteroids.size(); i++)
    {
      if (!_asteroids[i].get()->Active())
      {
        continue;
      }

      _asteroids[i].get()->Compute();
      //_asteroids[i].get()->Draw(true, true);
      _asteroids[i].get()->Draw(false, false);
    }

    ProcessCollisions();
    HandleCollisions();

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
          if (_asteroids[i].get()->Active() && Util::TestIntersection(_asteroids[i].get()->GetSprite().GetCollisionInfo(), bullet.get()->GetSprite().GetCollisionInfo()))
          {
            _asteroidExplosion.Play(_asteroids[i].get()->Position().X(), _asteroids[i].get()->Position().Y(), _bigAsteroidExplosionScale / (_asteroids[i].get()->CurrentBreakdownLevel() + 1));

            _asteroids[i].get()->ProcessCollision();
            //asteroid.get()->GetSprite().SetColor(Colors::Red);
            //_bitmapFont->SetTextColor(255, 255, 255, 255);
            //_bitmapFont->SetScale(0.5f);
            //_bitmapFont->Printf(_asteroids[i].get()->Position().X(), _asteroids[i].get()->Position().Y(), BitmapFont::AlignLeft, "Asteroid hit: %f", bullet.get()->Angle());

            bullet.get()->SetActive(false);

            _score += _asteroids[i].get()->CurrentBreakdownLevel();

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
      if (asteroid.get()->Active())
      {
        if (_ship.ShieldActive())
        {
         if (asteroid.get()->GetSprite().Convex())
         {
           if (Util::PolygonVsCircle(_ship.Position(),
                                     _ship.DefaultShieldRadius,
                                     asteroid.get()->GetSprite().TranslatedCollider()))
           {
             _asteroidExplosion.Play(asteroid.get()->Position().X(), asteroid.get()->Position().Y(), _bigAsteroidExplosionScale / (asteroid.get()->CurrentBreakdownLevel() + 1));
             asteroid.get()->ProcessCollision();
             _ship.ProcessShieldCollision();
             break;
           }
         }
         else
         {
           if (Util::PolygonVsCircle(_ship.Position(),
                                     _ship.DefaultShieldRadius,
                                     asteroid.get()->GetSprite().TriangulatedTranslatedCollider()))
           {
             _asteroidExplosion.Play(asteroid.get()->Position().X(), asteroid.get()->Position().Y(), _bigAsteroidExplosionScale / (asteroid.get()->CurrentBreakdownLevel() + 1));
             asteroid.get()->ProcessCollision();
             _ship.ProcessShieldCollision();
             break;
           }
         }
        }
        else
        {
          if (Util::TestIntersection(asteroid.get()->GetSprite().GetCollisionInfo(), _ship.GetSprite().GetCollisionInfo()))
          {
            _asteroidExplosion.Play(asteroid.get()->Position().X(), asteroid.get()->Position().Y(), _bigAsteroidExplosionScale / (asteroid.get()->CurrentBreakdownLevel() + 1));

            asteroid.get()->ProcessCollision();

            _shipHit = true;
            break;
          }
        }
      }
    }
  }
}

void Application::HandleCollisions()
{
  if (_shipHit)
  {
    _shipHit = false;
    _ship.ProcessCollision();
    if (_ship.HitPoints() <= 0)
    {
      _ship.SetSpeed(0.0);
      _ship.SetActive(false);
      _shipExplosion.Play(_ship.Position().X(), _ship.Position().Y(), 1.0);
      _shipDebris.Play(_ship.Position());
      _currentLives--;
    }
  }
}

void Application::ProcessExplosions()
{
  _asteroidExplosion.Process();
  _shipExplosion.Process();
  _shipDebris.Process();
}

void Application::ProcessInput()
{
  SDL_PumpEvents();

  _keyboardState = (Uint8*)SDL_GetKeyboardState(nullptr);

  if (_keyboardState[SDL_SCANCODE_ESCAPE])
  {
    _running = false;
  }

  if (_keyboardState[SDL_SCANCODE_RETURN] && !_ship.Active())
  {
    if (_currentLives == 0)
    {
      _currentLives = MaxLives;
      if (_score > _highScore) _highScore = _score;
      _score = 0;
    }

    _ship.Respawn();
  }

  if (_ship.Active())
  {
    if (_keyboardState[SDL_SCANCODE_A])
    {
      double d = _ship.RotationSpeed * GameTime::Get().DeltaTime();
      _ship.Rotate(_ship.Angle() - d);
    }

    if (_keyboardState[SDL_SCANCODE_D])
    {
      double d = _ship.RotationSpeed * GameTime::Get().DeltaTime();
      _ship.Rotate(_ship.Angle() + d);
    }

    if (_keyboardState[SDL_SCANCODE_W])
    {
      _ship.Accelerate(_ship.AccelerationSpeed * GameTime::Get().DeltaTime());
    }

    if (_keyboardState[SDL_SCANCODE_SPACE])
    {
      if (!_fireTrigger)
      {
        _fireTrigger = true;
        _ship.Fire();
      }
    }

    if (!_keyboardState[SDL_SCANCODE_SPACE])
    {
      _fireTrigger = false;
    }

//    if (keyboardState[SDL_SCANCODE_S])
//    {
//      _ship.Accelerate(-_accelerationSpeed);
//    }

    if (!_keyboardState[SDL_SCANCODE_W] && _ship.Speed() > 0.0)
    {
      _ship.Accelerate(-_ship.AccelerationSpeed * GameTime::Get().DeltaTime());
    }
  }
}

void Application::InitGUI()
{
  int index = TextureManager::Get().FindTextureByRole(GlobalStrings::GUIHeartRole);
  if (index != -1)
  {
    _guiHeart.Init(index);
  }

  index = TextureManager::Get().FindTextureByRole(GlobalStrings::GUIShieldRole);
  if (index != -1)
  {
    _guiShield.Init(index);
  }

  index = TextureManager::Get().FindTextureByRole(GlobalStrings::ShipBigRole);
  if (index != -1)
  {
    _guiLives.Init(index, true);
  }

  _guiHeart.SetScaleFactor(0.25);
  _guiShield.SetScaleFactor(0.25);
  _guiLives.SetScaleFactor(0.2);
}

void Application::DrawGUI()
{
  for (int i = 0; i < _currentLives; i++)
  {
    _guiLives.Draw(i * (_guiLives.ImageWrapper()->Width() * 0.2) + 10, (_guiLives.ImageWrapper()->Height() * 0.2) / 2);
  }

  _bitmapFont->SetTextColor(255, 255, 255, 255);
  _bitmapFont->SetScale(1.0);
  _bitmapFont->Printf(0, 32, BitmapFont::AlignLeft, "Record: %u", _highScore);

  _bitmapFont->SetTextColor(255, 255, 255, 255);
  _bitmapFont->SetScale(2.0);
  _bitmapFont->Printf(_screenSizeX / 2, 0, BitmapFont::AlignCenter, "%u", _score);

  int r = (_ship.ShipMaxHitPoints - _ship.HitPoints()) * _hitPointsColorDelta;
  int g = 255;

  if (_ship.HitPoints() <= _shipHitPointsHalf)
  {
    r = 255;
    g = 255 - ((_ship.ShipMaxHitPoints - _shipHitPointsHalf) - _ship.HitPoints()) * _hitPointsColorDelta;
  }

  _bitmapFont->SetTextColor(r, g, 0, 255);
  _bitmapFont->SetScale(1.0);
  _bitmapFont->Printf(_screenSizeX - 25, 0, BitmapFont::AlignRight, (char*)_ship.HitPointsBar().data());

  _guiHeart.Draw(_screenSizeX - 16, 8);
  _guiShield.Draw(_screenSizeX - 16, 25);

  int a = 255 - (_ship.ShieldMaxPoints - _ship.ShieldPoints()) * _shieldColorAlphaDelta;

  _bitmapFont->SetTextColor(0, 255, 255, a);
  _bitmapFont->SetScale(1.0);
  _bitmapFont->Printf(_screenSizeX - 25, 16, BitmapFont::AlignRight, (char*)_ship.ShieldPointsBar().data());

  if (!_ship.Active())
  {
    _bitmapFont->SetTextColor(255, 255, 0, 255);
    _bitmapFont->SetScale(4.0);
    if (_currentLives > 0)
    {
      _bitmapFont->Printf(_screenSizeX / 2, _screenSizeY / 2 - _bitmapFont->LetterWidth,
                                               BitmapFont::AlignCenter, "PRESS ENTER TO RESPAWN");
    }
    else
    {
      _bitmapFont->Printf(_screenSizeX / 2, _screenSizeY / 2 - _bitmapFont->LetterWidth,
                                               BitmapFont::AlignCenter, "GAME OVER");
    }
  }
}
