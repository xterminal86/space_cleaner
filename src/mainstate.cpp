#include "mainstate.h"

MainState::MainState()
{
  _asteroidExplosion.Init(GlobalStrings::ExplosionSpriteFilename, _maxExplosions, 25);
  _shipExplosion.Init(GlobalStrings::ExplosionSpriteShipFilename, 1, 25);
  _spawnAnimation.Init(GlobalStrings::SpawnAnimationFilename, 1, 25);

  _fireTrigger = false;
  _quitFlag = false;
  _keyPressed = false;
  _scoreWritten = false;

  _score = 0;
  _timePassed = 0;
  _waveCounter = 0;
  _fancyColorPhase = 0;
  _fancyColorCounter = 0;
  _fancyColorDelta = 10;
  _currentLives = _startingLives;
  _currentSpawnRate = GameMechanic::StartingSpawnRateMs;
  _guiSpawnRateNumber = (double)GameMechanic::StartingSpawnRateMs / (double)_currentSpawnRate;

  _fancyTextColor.r = 255;
  _fancyTextColor.g = 0;
  _fancyTextColor.b = 0;
  _fancyTextColor.a = 255;

  _keyboardState = nullptr;

  _hitPointsColorDelta = 255 / _ship.ShipMaxHitPoints;
  _hitPointsColorDelta *= 2;
  _shipHitPointsHalf = _ship.ShipMaxHitPoints / 2;
  _shieldColorAlphaDelta = 128 / _ship.ShieldMaxPoints;

  _screenSizeX = VideoSystem::Get().ScreenDimensions().x;
  _screenSizeY = VideoSystem::Get().ScreenDimensions().y;

  _spawnPoints.push_back(Vector2(_spawnSpread, _spawnSpread));
  _spawnPoints.push_back(Vector2(_screenSizeX - _spawnSpread, _spawnSpread));
  _spawnPoints.push_back(Vector2(_screenSizeX - _spawnSpread, _screenSizeY - _spawnSpread));
  _spawnPoints.push_back(Vector2(_spawnSpread, _screenSizeY - _spawnSpread));

  InitPowerups();
  InitGUI();
  LoadBackground();

  _backgroundX = _screenWidth / 2;
  _backgroundY = _screenHeight / 2;

  _ship.Init(0, 0);
  _ship.Scale(0.5);
  _ship.Move(_screenSizeX / 2, _screenSizeY / 2);

  int res = TextureManager::Get().FindTextureByRole(GlobalStrings::ShipBigRole);
  if (res != -1)
  {
    PNGLoader* imageRef = TextureManager::Get().GetTextureWrapper(res);
    _shipDebris.Init(imageRef, 10, 1500);
  }
}

MainState::~MainState()
{
  //dtor
}

void MainState::Init(Application* game)
{
  RestartGame();
}

void MainState::Cleanup()
{
  _asteroids.clear();
}

void MainState::Pause()
{
}

void MainState::Resume()
{
}

void MainState::HandleEvents(Application* game)
{
  SDL_PumpEvents();

  _keyboardState = (Uint8*)SDL_GetKeyboardState(nullptr);

  if (_keyboardState[SDL_SCANCODE_ESCAPE])
  {
    if (!_keyPressed)
    {
      SoundSystem::Get().PlaySound(Sounds::MENU_SELECT);

      _keyPressed = true;
      _quitFlag = true;
    }
  }

  if (_quitFlag && _keyboardState[SDL_SCANCODE_Y])
  {
    SoundSystem::Get().PlaySound(Sounds::MENU_SELECT);

    _quitFlag = false;
    game->PopState();
  }

  if (_quitFlag && _keyboardState[SDL_SCANCODE_N])
  {
    SoundSystem::Get().PlaySound(Sounds::MENU_BACK);

    _quitFlag = false;
  }

  if (_keyboardState[SDL_SCANCODE_RETURN] && !_ship.Active())
  {
    if (_currentLives < 0)
    {
      RestartGame();
    }

    _ship.Respawn();
  }

  if (_ship.Active())
  {
    if (_keyboardState[SDL_SCANCODE_A])
    {
      double d = _ship.RotationSpeed * GameTime::Get().DeltaTimeMs();
      _ship.Rotate(_ship.Angle() - d);
    }

    if (_keyboardState[SDL_SCANCODE_D])
    {
      double d = _ship.RotationSpeed * GameTime::Get().DeltaTimeMs();
      _ship.Rotate(_ship.Angle() + d);
    }

    if (_keyboardState[SDL_SCANCODE_W])
    {
      _ship.Accelerate(_ship.AccelerationSpeed * GameTime::Get().DeltaTimeMs());
    }

    if (_keyboardState[SDL_SCANCODE_SPACE])
    {
      if (!_fireTrigger)
      {
        SoundSystem::Get().PlaySound(Sounds::SHIP_FIRE);

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
      _ship.Accelerate(-_ship.AccelerationSpeed * GameTime::Get().DeltaTimeMs());
    }
  }

  if (!_keyboardState[SDL_SCANCODE_ESCAPE])
  {
    _keyPressed = false;
  }
}

void MainState::Update(Application* game)
{
  if (_currentLives < 0 && !_scoreWritten)
  {
    _scoreWritten = true;
    HighScore score;
    score.Score = _score;
    score.Wave = _waveCounter;
    game->StoreHighScore(score);
  }
}

void MainState::Draw(Application* game)
{
  SDL_Renderer* renderer = VideoSystem::Get().Renderer();

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

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
  ProcessExplosions();
  ProcessPowerups();

  DrawGUI();

  SDL_RenderPresent(renderer);

  _timePassed += GameTime::Get().DeltaTimeMs();

  TryToSpawnAsteroid();

  CleanAsteroids();
}

// ==================== Private Methods =================== //

void MainState::LoadBackground()
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

void MainState::DrawBackground()
{
  _background.Draw(_backgroundX, _backgroundY);

  for (auto& i : _stars)
  {
    i.Pulse();
  }
}

void MainState::InitAsteroids()
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

void MainState::TryToSpawnPowerup(int x, int y)
{
  int chance = Util::RandomNumber() % 1500 + 1;
  if (chance > 100) return;

  int type = -1;

  if (_ship.ShieldPoints() < (_ship.ShieldMaxPoints / 2)) type = Powerups::SHIELD_POWERUP;
  else if (_ship.HitPoints() < (_ship.ShipMaxHitPoints / 2)) type = Powerups::HEALTH_POWERUP;
  else if (chance == 100) type = Powerups::LIFE_POWERUP;

  if (type == -1) return;

  for (auto& p : _powerupsPool)
  {
    if (!p.Active() && p.Type() == type)
    {
      p.Spawn(Vector2(x, y));
      break;
    }
  }
}

void MainState::TryToSpawnAsteroid()
{
  if (_timePassed >= _currentSpawnRate)
  {
    int index = Util::RandomNumber() % _spawnPoints.size();

    if ( (_asteroidInstances < _maxAsteroidInstances) && _ship.Active())
    {
      SpawnAsteroid((int)_spawnPoints[index].X(), (int)_spawnPoints[index].Y());

      _asteroidInstances++;

      if (_asteroidInstances >= _maxAsteroidInstances)
      {
        _asteroidInstances = _maxAsteroidInstances;
      }

      _currentSpawnRate -= GameMechanic::SpawnRateDeltaMs;

      if (_currentSpawnRate <= GameMechanic::MaxSpawnRateMs)
      {
        _currentSpawnRate = GameMechanic::MaxSpawnRateMs;
      }

      _waveCounter++;
    }

    _timePassed = 0;
  }
}

void MainState::SpawnAsteroid(int x, int y)
{
  int dx = Util::RandomNumber() % _spawnSpread;
  int dy = Util::RandomNumber() % _spawnSpread;

  int posx = x + dx;
  int posy = y + dy;

  if (posx < 0) posx = 1;
  if (posx > _screenSizeX) posx = _screenSizeX - 1;
  if (posy < 0) posy = 1;
  if (posy > _screenSizeY) posy = _screenSizeY - 1;

  Vector2 pos(posx, posy);
  _asteroids.push_back(std::unique_ptr<Asteroid>(new Asteroid(pos, 0, &_asteroids)));

  _spawnAnimation.Play(posx, posy, 2.0);

  SoundSystem::Get().PlaySound(Sounds::ASTEROID_SPAWN);
}

void MainState::CleanAsteroids()
{
  for (int i = 0; i < _asteroids.size(); i++)
  {
    if (!_asteroids[i].get()->Active())
    {
      if (_asteroids[i].get()->CurrentBreakdownLevel() == 1)
      {
        _asteroidInstances--;
        if (_asteroidInstances <= 0) _asteroidInstances = 0;

        _asteroids.erase(_asteroids.begin() + i);
      }
    }
  }
}

void MainState::ProcessCollisions()
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
            if (_asteroids[i].get()->CurrentBreakdownLevel() > 1)
            {
              SoundSystem::Get().PlaySound(Sounds::ASTEROID_HIT_SMALL);
            }
            else
            {
              SoundSystem::Get().PlaySound(Sounds::ASTEROID_HIT_BIG);
            }

            _asteroidExplosion.Play(_asteroids[i].get()->Position().X(), _asteroids[i].get()->Position().Y(), _bigAsteroidExplosionScale / (_asteroids[i].get()->CurrentBreakdownLevel() + 1));

            // Look for comments below in ship branch
            TryToSpawnPowerup(_asteroids[i].get()->Position().X(), _asteroids[i].get()->Position().Y());

            _asteroids[i].get()->ProcessCollision();

            _score += _asteroids[i].get()->CurrentBreakdownLevel();

            bullet.get()->SetActive(false);

            break;
          }
        }
      }
    }
  }

  if (_ship.Active())
  {
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
              SoundSystem::Get().PlaySound(Sounds::SHIELD_HIT_ENERGY);

              _asteroidExplosion.Play(asteroid.get()->Position().X(), asteroid.get()->Position().Y(), _bigAsteroidExplosionScale / (asteroid.get()->CurrentBreakdownLevel() + 1));

              // Since Asteroid::ProcessCollision() involves push_back operation, and
              // we have CleanupAsteroids method, that deletes inactive asteroids,
              // it is better to do everything related to object's address beforehand.
              _ship.ProcessShieldCollision(asteroid.get());

              asteroid.get()->ProcessCollision();
              break;
            }
          }
          else
          {
            if (Util::PolygonVsCircle(_ship.Position(),
                                      _ship.DefaultShieldRadius,
                                      asteroid.get()->GetSprite().TriangulatedTranslatedCollider()))
            {
              SoundSystem::Get().PlaySound(Sounds::SHIELD_HIT_ENERGY);

              _asteroidExplosion.Play(asteroid.get()->Position().X(), asteroid.get()->Position().Y(), _bigAsteroidExplosionScale / (asteroid.get()->CurrentBreakdownLevel() + 1));
               _ship.ProcessShieldCollision(asteroid.get());
              asteroid.get()->ProcessCollision();
              break;
            }
          }
        }
        else
        {
          if (Util::TestIntersection(asteroid.get()->GetSprite().GetCollisionInfo(), _ship.GetSprite().GetCollisionInfo()))
          {
            SoundSystem::Get().PlaySound(Sounds::SHIP_HIT);

            _asteroidExplosion.Play(asteroid.get()->Position().X(), asteroid.get()->Position().Y(), _bigAsteroidExplosionScale / (asteroid.get()->CurrentBreakdownLevel() + 1));

            HandleShipCollision(asteroid.get());

            asteroid.get()->ProcessCollision();

            break;
          }
        }
      }
    }
  }
}

void MainState::HandleShipCollision(Asteroid* collidedAsteroid)
{
  _ship.ProcessCollision(collidedAsteroid);
  if (_ship.HitPoints() <= 0)
  {
    SoundSystem::Get().PlaySound(Sounds::SHIP_EXPLODE);

    _ship.SetSpeed(0.0);
    _ship.SetActive(false);
    _shipExplosion.Play(_ship.Position().X(), _ship.Position().Y(), 1.0);
    _shipDebris.Play(_ship.Position());
    _currentLives--;

    if (_currentLives < 0)
    {
      SoundSystem::Get().PlaySound(Sounds::GAME_OVER);
    }
  }
}

void MainState::ProcessExplosions()
{
  _asteroidExplosion.Process();
  _shipExplosion.Process();
  _spawnAnimation.Process();
  _shipDebris.Process();
}

void MainState::ProcessPowerups()
{
  for (auto& powerup : _powerupsPool)
  {
    if (!powerup.Active()) continue;

    powerup.Process();

    if (_ship.Active() && Util::TestIntersection(_ship.GetSprite().GetCollisionInfo(), powerup.GetSprite().GetCollisionInfo()))
    {
      if (powerup.Type() == Powerups::HEALTH_POWERUP)
      {
        SoundSystem::Get().PlaySound(Sounds::POWERUP_HEALTH);

        _ship.AddHitPoints(_ship.ShipMaxHitPoints / 2);
      }
      else if (powerup.Type() == Powerups::SHIELD_POWERUP)
      {
        SoundSystem::Get().PlaySound(Sounds::POWERUP_SHIELD);

        _ship.AddShieldPoints(_ship.ShieldMaxPoints / 2);
      }
      else if (powerup.Type() == Powerups::LIFE_POWERUP)
      {
        SoundSystem::Get().PlaySound(Sounds::POWERUP_LIFE);

        _currentLives++;

        if (_currentLives > _maxLivesLimit) _currentLives = _maxLivesLimit;
      }

      powerup.SetActive(false);
    }
  }
}

void MainState::InitPowerups()
{
  int poolSizeHalf = _powerupsPoolSize / 2;

  for (int i = 0; i < _powerupsPoolSize; i++)
  {
    Powerup p;

    if (i % 8 == 0)
    {
      p.Create(Powerups::LIFE_POWERUP);
    }
    else if (i % 2 == 0)
    {
      p.Create(Powerups::HEALTH_POWERUP);
    }
    else if (i % 2 != 0)
    {
      p.Create(Powerups::SHIELD_POWERUP);
    }

    _powerupsPool.push_back(p);
  }
}

void MainState::RestartGame()
{
  _scoreWritten = false;
  _currentLives = _startingLives;
  _score = 0;
  _timePassed = 0;
  _currentSpawnRate = GameMechanic::StartingSpawnRateMs;
  _guiSpawnRateNumber = (double)GameMechanic::StartingSpawnRateMs / (double)_currentSpawnRate;
  _waveCounter = 0;
  _asteroids.clear();
  _asteroidInstances = 0;
  _ship.Move(_screenSizeX / 2, _screenSizeY / 2);
}

void MainState::InitGUI()
{
  int index = TextureManager::Get().FindTextureByRole(GlobalStrings::GUIHeartRole);
  if (index != -1)
  {
    _guiHeart.Init(index, true);
  }

  index = TextureManager::Get().FindTextureByRole(GlobalStrings::GUIShieldRole);
  if (index != -1)
  {
    _guiShield.Init(index, true);
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

void MainState::CalculateFancyTextColor()
{
  if (_ship.Active()) return;

  _fancyColorCounter += _fancyColorDelta;

  if (_fancyColorCounter <= 0)
  {
    _fancyColorCounter = 0;
    _fancyColorPhase++;
    _fancyColorDelta *= -1;
  }

  if (_fancyColorCounter >= 255)
  {
    _fancyColorCounter = 255;
    _fancyColorPhase++;
    _fancyColorDelta *= -1;
  }

  if (_fancyColorPhase > 5) _fancyColorPhase = 0;

  if (_fancyColorPhase == 0 || _fancyColorPhase == 3)
  {
    _fancyTextColor.g = _fancyColorCounter;
  }

  if (_fancyColorPhase == 1 || _fancyColorPhase == 4)
  {
    _fancyTextColor.r = _fancyColorCounter;
  }

  if (_fancyColorPhase == 2 || _fancyColorPhase == 5)
  {
    _fancyTextColor.b = _fancyColorCounter;
  }
}

void MainState::DrawGUI()
{
  CalculateFancyTextColor();

  _guiSpawnRateNumber = (double)GameMechanic::StartingSpawnRateMs / (double)_currentSpawnRate;
  _guiTimeToSpawnNumber = (double)_timePassed / (double)_currentSpawnRate;

  int meter = (int)(_spawnTimeMeterLength * _guiTimeToSpawnNumber);
  _guiSpawnTimeString.clear();
  for (int i = 0; i < meter; i++)
  {
    _guiSpawnTimeString.append("=");
  }

  for (int i = 0; i < _currentLives; i++)
  {
    _guiLives.Draw(i * (_guiLives.ImageWrapper()->Width() * 0.2) + 10, (_guiLives.ImageWrapper()->Height() * 0.2) / 2);
  }

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

  _bitmapFont->SetTextColor(255, 255, 255, 255);
  _bitmapFont->SetScale(1.0);
  _bitmapFont->Printf(_screenSizeX - 25, 32, BitmapFont::AlignRight, "Wave: %i", _waveCounter);
  _bitmapFont->Printf(_screenSizeX - _spawnTimeMeterLength * (_bitmapFont->LetterWidth / 2), 48, BitmapFont::AlignLeft, (char*)_guiSpawnTimeString.data());
  _bitmapFont->SetScale(1.0);
  _bitmapFont->Printf(_screenSizeX - 25, 64, BitmapFont::AlignRight, "Spawned: %i/%i", _asteroidInstances, _maxAsteroidInstances);
  _bitmapFont->SetScale(0.75);
  _bitmapFont->Printf(_screenSizeX - 25, 80, BitmapFont::AlignRight, "(spawn rate: %.2f)", _guiSpawnRateNumber);

  if (!_ship.Active())
  {
    if (_currentLives >= 0)
    {
      _bitmapFont->SetTextColor(_fancyTextColor);
      _bitmapFont->SetScale(2.0);
      _bitmapFont->Printf(_screenSizeX / 2, _screenSizeY - _bitmapFont->LetterWidth * _bitmapFont->ScaleFactor(),
                                               BitmapFont::AlignCenter, "Hit ENTER to respawn");
    }
    else
    {
      _bitmapFont->SetTextColor(255, 255, 0, 255);
      _bitmapFont->SetScale(4.0);
      _bitmapFont->Printf(_screenSizeX / 2, 100, BitmapFont::AlignCenter, "GAME OVER");

      _bitmapFont->SetTextColor(_fancyTextColor);
      _bitmapFont->SetScale(2.0);
      _bitmapFont->Printf(_screenSizeX / 2, _screenSizeY - _bitmapFont->LetterWidth * _bitmapFont->ScaleFactor(),
                                            BitmapFont::AlignCenter, "Hit ENTER to start over");
    }
  }

  if (_quitFlag)
  {
    _bitmapFont->SetTextColor(255, 255, 0, 255);
    _bitmapFont->SetScale(2.0);
    _bitmapFont->Printf(_screenSizeX / 2, _screenSizeY / 2 - _bitmapFont->LetterWidth * _bitmapFont->ScaleFactor(),
                                          BitmapFont::AlignCenter, "Are you sure you want to exit this game?\n(Y/N)");
  }
}
