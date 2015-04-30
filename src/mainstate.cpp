#include "mainstate.h"

MainState::MainState()
{
  _fireTrigger = false;
  _quitFlag = false;
  _keyPressed = false;
  _scoreWritten = false;

  _score = 0;
  _timePassed = 0;
  _autoFireTimePassed = 0;
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

  InitGUI();
  LoadBackground();

  _backgroundX = _screenWidth / 2;
  _backgroundY = _screenHeight / 2;

  _ship.Init(0, 0);
  _ship.Scale(0.5);
  _ship.Move(_screenSizeX / 2, _screenSizeY / 2);

  int res = TextureManager::Get().FindTextureByRole(GlobalStrings::ShipRole);
  if (res != -1)
  {
    PNGLoader* imageRef = TextureManager::Get().GetTextureWrapper(res);
    _shipDebris.Init(imageRef, 10, 1500);
  }

  Asteroid::ResetInstances();
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
  SoundSystem::Get().StopMusic();
  SoundSystem::Get().StopAllSounds();
  AnimationsManager::Get().StopAnimations();
  _asteroids.clear();
  _ship.StopBullets();
  _ship.ResetShieldVisual();
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

    _currentSpawnRate = GameMechanic::StartingSpawnRateMs;
    _timePassed = 0;
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
      if (_ship.AutoFire())
      {
        if (_autoFireValid)
        {
          _ship.Fire();
        }
      }
      else
      {
        if (!_fireTrigger)
        {
          _fireTrigger = true;
          _ship.Fire();
        }
      }
    }

    //if (_keyboardState[SDL_SCANCODE_S])
    //{
    //  _ship.Accelerate(-_ship.AccelerationSpeed * GameTime::Get().DeltaTimeMs());
    //}

    if (!_keyboardState[SDL_SCANCODE_SPACE])
    {
      _fireTrigger = false;
    }

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
  _autoFireTimePassed += GameTime::Get().DeltaTimeMs();
  _timePassed += GameTime::Get().DeltaTimeMs();

  _shieldRechargeTimePassed += GameTime::Get().DeltaTimeMs();
  if (_shieldRechargeTimePassed >= GameMechanic::ShieldRechargeMs)
  {
    _ship.AddShieldPoints(1);
    _shieldRechargeTimePassed = 0;
  }

  _autoFireValid = (_autoFireTimePassed > GameMechanic::AutoFireRateMs);
  if (_autoFireValid)
  {
    _autoFireTimePassed = 0;
  }

  MakeBars();

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

void MainState::TryToSpawnPowerup(int x, int y)
{
  int chance = Util::RandomNumber() % 1000 + 1;
  if (chance > 100) return;

  int type = -1;

  switch (chance)
  {
  case 100:
    type = _currentLives != _maxLivesLimit ? Powerups::LIFE_POWERUP : -1;
    break;

  default:
    if (_ship.ShieldPoints() < (_ship.ShieldMaxPoints / 2))
    {
      if (_ship.HitPoints() < (_ship.ShipMaxHitPoints / 2))
      {
        type = Powerups::HEALTH_POWERUP;
      }
      else
      {
        type = Powerups::SHIELD_POWERUP;
      }
    }
    break;
  }

  if (type == -1) return;

  PowerupsManager::Get().SpawnPowerup(x, y, type);
}

void MainState::TryToSpawnAsteroid()
{
  if (_timePassed >= _currentSpawnRate)
  {
    int index = Util::RandomNumber() % _spawnPoints.size();

    if ( (Asteroid::Instances() < _maxAsteroidInstances) && _ship.Active())
    {
      SpawnAsteroid((int)_spawnPoints[index].X(), (int)_spawnPoints[index].Y());

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

  AnimationsManager::Get().Play(AnimationsIds::SPAWN_BIG, posx, posy, 2.0);
  SoundSystem::Get().PlaySound(Sounds::ASTEROID_SPAWN);
}

void MainState::CleanAsteroids()
{
  for (int i = 0; i < _asteroids.size(); i++)
  {
    if (!_asteroids[i].get()->Active())
    {
      if (_asteroids[i].get()->CurrentBreakdownLevel() == (GameMechanic::AsteroidMaxBreakdownLevel + 1))
      {
        _asteroids.erase(_asteroids.begin() + i);
        break;
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
            _asteroids[i].get()->ProcessCollision(bullet.get());

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

              AnimationsManager::Get().Play(AnimationsIds::EXPLOSION_ASTEROID,
                                            asteroid.get()->Position().X(),
                                            asteroid.get()->Position().Y(),
                                            GameMechanic::BigAsteroidExplosionScale / (asteroid.get()->CurrentBreakdownLevel() + 1));

              // Since Asteroid::ProcessCollision() involves push_back operation, and
              // we have CleanupAsteroids method, that deletes inactive asteroids,
              // it is better to do everything related to object's address beforehand.
              _ship.ProcessShieldCollision(asteroid.get());

              asteroid.get()->ProcessCollision(nullptr);
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

              AnimationsManager::Get().Play(AnimationsIds::EXPLOSION_ASTEROID,
                                            asteroid.get()->Position().X(),
                                            asteroid.get()->Position().Y(),
                                            GameMechanic::BigAsteroidExplosionScale / (asteroid.get()->CurrentBreakdownLevel() + 1));

              _ship.ProcessShieldCollision(asteroid.get());

              asteroid.get()->ProcessCollision(nullptr);
              break;
            }
          }
        }
        else
        {
          if (Util::TestIntersection(asteroid.get()->GetSprite().GetCollisionInfo(), _ship.GetSprite().GetCollisionInfo()))
          {
            SoundSystem::Get().PlaySound(Sounds::SHIP_HIT);

            AnimationsManager::Get().Play(AnimationsIds::EXPLOSION_ASTEROID,
                                          asteroid.get()->Position().X(),
                                          asteroid.get()->Position().Y(),
                                          GameMechanic::BigAsteroidExplosionScale / (asteroid.get()->CurrentBreakdownLevel() + 1));

            HandleShipCollision(asteroid.get());

            asteroid.get()->ProcessCollision(nullptr);

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
    AnimationsManager::Get().Play(AnimationsIds::EXPLOSION_SHIP, _ship.Position().X(), _ship.Position().Y(), 1.0);
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
  AnimationsManager::Get().Process();
  _shipDebris.Process();
}

void MainState::ProcessPowerups()
{
  PowerupsManager::Get().Process();

  for (auto& powerup : PowerupsManager::Get().PowerupsPool())
  {
    if (!powerup.Active()) continue;

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
  Asteroid::ResetInstances();
  _ship.Move(_screenSizeX / 2, _screenSizeY / 2);
  _ship.Respawn();
  SoundSystem::Get().PlayMusic();
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

  index = TextureManager::Get().FindTextureByRole(GlobalStrings::ShipRole);
  if (index != -1)
  {
    _guiLives.Init(index, true);
  }

  index = TextureManager::Get().FindTextureByRole(GlobalStrings::GUIWeaponFrame);
  if (index != -1)
  {
    _guiWeaponFrame.Init(index, true);
  }

  index = TextureManager::Get().FindTextureByRole(GlobalStrings::GUIBlackBack);
  if (index != -1)
  {
    _guiBlackBack.Init(index, true);
  }

  index = TextureManager::Get().FindTextureByRole(GlobalStrings::GUIDivider);
  if (index != -1)
  {
    _guiDivider.Init(index, true);
  }

  index = TextureManager::Get().FindTextureByRole(GlobalStrings::GUIShipOutline);
  if (index != -1)
  {
    _guiExtraLifeOutline.Init(index, true);
  }

  _guiHeart.SetScaleFactor(0.25);
  _guiShield.SetScaleFactor(0.25);
  _guiLives.SetScaleFactor(0.2);
  _guiExtraLifeOutline.SetScaleFactor(0.2);
  _guiWeaponFrame.SetScaleFactor(1.0);
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

void MainState::MakeBars()
{
  _hitPointsBar.clear();
  for (int i = 0; i < _ship.HitPoints(); i++)
  {
    _hitPointsBar.append(">");
  }

  _shieldPointsBar.clear();
  for (int i = 0; i < _ship.ShieldPoints(); i++)
  {
    _shieldPointsBar.append(">");
  }
}

void MainState::DrawGUI()
{
  CalculateFancyTextColor();

  _guiSpawnRateNumber = (double)GameMechanic::StartingSpawnRateMs / (double)_currentSpawnRate;
  _guiTimeToSpawnNumber = (double)_timePassed / (double)_currentSpawnRate;

  _guiBlackBack.Draw(0, 0, _screenSizeX, GUI::GUITopBackgroundHeight);
  _guiDivider.Draw(0, GUI::GUITopBackgroundHeight, _screenSizeX, _guiDivider.ImageWrapper()->Height(), true);

  int meter = (int)(GUI::SpawnTimeMeterLength * _guiTimeToSpawnNumber);
  _guiSpawnTimeString.clear();
  for (int i = 0; i < meter; i++)
  {
    _guiSpawnTimeString.append("=");
  }

  for (int i = 0; i < _maxLivesLimit; i++)
  {
    _guiExtraLifeOutline.Draw(i * (_guiExtraLifeOutline.ImageWrapper()->Width() * 0.2) + 10, (_guiExtraLifeOutline.ImageWrapper()->Height() * 0.2) / 2);
  }

  for (int i = 0; i < _currentLives; i++)
  {
    _guiLives.Draw(i * (_guiLives.ImageWrapper()->Width() * 0.2) + 10, (_guiLives.ImageWrapper()->Height() * 0.2) / 2);
  }

  _bitmapFont->SetTextColor(255, 255, 255, 255);
  _bitmapFont->SetScale(1.0);
  _bitmapFont->Printf(_screenSizeX / 2, 0, BitmapFont::AlignCenter, "SCORE", _score);
  _bitmapFont->Printf(_screenSizeX / 2, 16, BitmapFont::AlignCenter, "%u", _score);

  int r = (_ship.ShipMaxHitPoints - _ship.HitPoints()) * _hitPointsColorDelta;
  int g = 255;

  if (_ship.HitPoints() <= _shipHitPointsHalf)
  {
    r = 255;
    g = 255 - ((_ship.ShipMaxHitPoints - _shipHitPointsHalf) - _ship.HitPoints()) * _hitPointsColorDelta;
  }

  _bitmapFont->SetTextColor(r, g, 0, 255);
  _bitmapFont->SetScale(1.0);
  _bitmapFont->Printf(_screenSizeX - 25, 0, BitmapFont::AlignRight, (char*)_hitPointsBar.data());

  _guiHeart.Draw(_screenSizeX - 16, 8);
  _guiShield.Draw(_screenSizeX - 16, 25);
  _guiWeaponFrame.Draw(_screenSizeX - 210, 16);
  _ship.CurrentWeaponImage().Draw(_screenSizeX - 210, 16);

  _bitmapFont->SetTextColor(255, 255, 255, 255);

  if (_ship.Level() == (GameMechanic::ExperienceMap.size() - 1))
  {
    _bitmapFont->Print(_screenSizeX - 240, 8, BitmapFont::AlignRight, "MAX");

    _bitmapFont->SetTextColor(255, 255, 0, 255);
    _bitmapFont->Print(_screenSizeX - 210, 16, BitmapFont::AlignCenter, "auto");
  }
  else
  {
    _bitmapFont->Printf(_screenSizeX - 240, 0, BitmapFont::AlignRight, "LEVEL %i", _ship.Level() + 1);
    _bitmapFont->Printf(_screenSizeX - 240, 16, BitmapFont::AlignRight, "%i / %i", _ship.Kills(), GameMechanic::ExperienceMap[_ship.Level()]);
  }

  int a = 255 - (_ship.ShieldMaxPoints - _ship.ShieldPoints()) * _shieldColorAlphaDelta;

  _bitmapFont->SetTextColor(0, 255, 255, a);
  _bitmapFont->SetScale(1.0);
  _bitmapFont->Printf(_screenSizeX - 25, 16, BitmapFont::AlignRight, (char*)_shieldPointsBar.data());

  _bitmapFont->SetTextColor(255, 255, 255, 255);
  _bitmapFont->SetScale(1.0);
  _bitmapFont->Printf(92, 16, BitmapFont::AlignLeft, "[");
  _bitmapFont->Printf(100, 18, BitmapFont::AlignLeft, (char*)_guiSpawnTimeString.data());
  _bitmapFont->Printf(100 + (GUI::SpawnTimeMeterLength - 1) * 8, 16, BitmapFont::AlignLeft, "]");
  _bitmapFont->Printf(180, 0, BitmapFont::AlignCenter, "WAVE %i", _waveCounter);
  _bitmapFont->Printf(270, 0, BitmapFont::AlignLeft, "S: %i / %i", Asteroid::Instances(), _maxAsteroidInstances);
  _bitmapFont->Printf(270, 16, BitmapFont::AlignLeft, "R: %.2f", _guiSpawnRateNumber);

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
