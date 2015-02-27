#include "application.h"

Application::Application()
{
  _logger->Init(GlobalStrings::LogFilename);
  _videoSystem->Init(_screenWidth, _screenHeight);
  _textureManager->Init(GlobalStrings::ImagesFilename, GlobalStrings::RelationFilename);
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

Vector2Pair Application::ProjectPolygon(std::vector<SDL_Point>& polygon, Vector2& axe)
{
  Vector2Pair proj;

  Vector2 projMin;
  Vector2 projMax;

  double min = polygon[0].x * axe.X() + polygon[0].y * axe.Y();
  double max = min;
  for (auto &i : polygon)
  {
    Vector2 tmp;
    tmp.Set(i.x, i.y);

    double scalar = tmp * axe;

    if (scalar < min) min = scalar;
    else if (scalar > max) max = scalar;

    //double x = axe.X() * scalar;
    //double y = axe.Y() * scalar;
  }

  projMin.Set(axe.X() * min, axe.Y() * min);
  projMax.Set(axe.X() * max, axe.Y() * max);

  proj.Set(projMin, projMax);

  return proj;
}

void Application::ProcessCollisions()
{
  // Perform the following only if needed
  if (_ship.HasBulletsActive())
  {
    for (auto &i : _ship.GetBullets())
    {
      // Take into account only active bullets
      if (i.get()->Active())
      {
        _asteroids.at(0).get()->GetSprite().CalculateSATAxes();
        i.get()->GetSprite().CalculateSATAxes();

        auto asteroidAxes = _asteroids.at(0).get()->GetSprite().GetAxesV2();
        auto bulletAxes = i.get()->GetSprite().GetAxesV2();

        bool collisionFlag = true;

        for (auto &i2 : bulletAxes)
        {
          Vector2Pair tmp = ProjectPolygon(i.get()->GetSprite().TranslatedCollider(), i2);
          Vector2Pair tmp2 = ProjectPolygon(_asteroids.at(0).get()->GetSprite().TranslatedCollider(), i2);

          collisionFlag = (collisionFlag && Vector2Pair::TestIntersection(tmp, tmp2));

          if (!collisionFlag)
          {
            break;
          }
        }

        for (auto &i3 : asteroidAxes)
        {
          Vector2Pair tmp = ProjectPolygon(i.get()->GetSprite().TranslatedCollider(), i3);
          Vector2Pair tmp2 = ProjectPolygon(_asteroids.at(0).get()->GetSprite().TranslatedCollider(), i3);

          collisionFlag = (collisionFlag && Vector2Pair::TestIntersection(tmp, tmp2));

          if (!collisionFlag)
          {
            break;
          }
        }

        if (collisionFlag)
        {
          printf("%f ", i.get()->Angle());
        }
      }
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

    SDL_RenderPresent(renderer);

    GameTime::Get().MeasureAfter();
  }
}
