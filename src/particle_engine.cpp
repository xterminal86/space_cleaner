#include "particle_engine.h"

ParticleEngine::ParticleEngine()
{
  //ctor
}

ParticleEngine::~ParticleEngine()
{
  _particles.clear();
}

void ParticleEngine::Init(int particlesNumber, int lifetimeMsMin, int lifetimeMsMax, double particleScaleIncrement, double scaleFactor, PNGLoader* particleImage)
{
  _numParticles = particlesNumber;
  _particleImage = particleImage;
  _particlesLifeTimeMsMin = lifetimeMsMin;
  _particlesLifeTimeMsMax = lifetimeMsMax;
  _particleScaleIncrement = particleScaleIncrement;
  _particleScaleFactor = scaleFactor;

  _srcRect.x = 0;
  _srcRect.y = 0;
  _srcRect.w = _particleImage->Width();
  _srcRect.h = _particleImage->Height();

  _dstRect.x = 0;
  _dstRect.y = 0;
  _dstRect.w = _particleImage->Width();
  _dstRect.h = _particleImage->Height();

  for (int i = 0; i < particlesNumber; i++)
  {
    Particle p;

    p.CurrentLifeTimeMs = 0;
    int lt = Util::RandomNumber() % lifetimeMsMax;
    if (lt == 0) lt = lifetimeMsMin;
    p.MaxLifeTimeMs = lt;
    //p.MaxLifeTimeMs = 1000;
    p.Speed = 0.0;
    //p.Speed = 0.15 / (double)(Util::RandomNumber() % 10 + 2);
    p.ScaleFactor = scaleFactor;

    _particles.push_back(p);
  }
}

void ParticleEngine::SetUp(Vector2 pos, Vector2 dir, double speed, double angle)
{
  _position = pos;
  _direction = dir;
  _speed = speed;
  _active = true;

  _direction.Normalize();

  for (auto& i : _particles)
  {
    i.Direction = dir;
    i.Position = pos;
    i.Speed = speed;
    i.Active = true;
    i.Angle = angle;
  }
}

void ParticleEngine::SetLifeAndSpeed(int lifeTimeMsMin, int lifeTimeMsMax, double speed)
{
  _particlesLifeTimeMsMin = lifeTimeMsMin;
  _particlesLifeTimeMsMax = lifeTimeMsMax;

  // Illusion of trail is done by setting up particle engine with speed 0.0.
  // So, when ship moves, point of particles' origin also moves, thus creating illusion of trail.
  // No speed is used and no new direction is assigned.
  // This was made unintentionally - speaking of Providence. :-)
  // I figured everything out when I tried to increase the speed of trail in ship.cpp
  //
  // Uncomment line below to break everything.

  //_speed = speed;
}

void ParticleEngine::Emit()
{
  for (auto& i : _particles)
  {
    if (!_active && !i.Active)
    {
      i.Position.Set(_position);
      continue;
    }

    double dx = _direction.X() * (i.Speed * GameTime::Get().DeltaTime());
    double dy = _direction.Y() * (i.Speed * GameTime::Get().DeltaTime());

    i.Position.Set(i.Position.X() + dx, i.Position.Y() + dy);

    _dstRect.x = i.Position.X() - (_particleImage->Width() * i.ScaleFactor) / 2;
    _dstRect.y = i.Position.Y() - (_particleImage->Height() * i.ScaleFactor) / 2;
    _dstRect.w = _particleImage->Width() * i.ScaleFactor;
    _dstRect.h = _particleImage->Height() * i.ScaleFactor;

    int res = SDL_RenderCopyEx(VideoSystem::Get().Renderer(), _particleImage->Texture(), &_srcRect, &_dstRect, i.Angle, nullptr, SDL_FLIP_NONE);
    if (res != 0) Logger::Get().LogPrint("(warning) Render copy error!\nReason: %s\n", SDL_GetError());

    i.CurrentLifeTimeMs += GameTime::Get().DeltaTime();
    i.ScaleFactor -= _particleScaleIncrement;

    if (i.ScaleFactor < 0.0) i.ScaleFactor = 0.0;

    if (i.CurrentLifeTimeMs > i.MaxLifeTimeMs)
    {
      i.CurrentLifeTimeMs = 0;
      int lt = Util::RandomNumber() % _particlesLifeTimeMsMax;
      if (lt == 0) lt = _particlesLifeTimeMsMin;
      i.MaxLifeTimeMs = lt;
      i.Position.Set(_position);
      i.ScaleFactor = _particleScaleFactor;
      i.Active = _active;
    }
  }
}
