#include "particle_engine.h"

ParticleEngine::ParticleEngine()
{
  //ctor
}

ParticleEngine::~ParticleEngine()
{
  _particles.clear();
}

void ParticleEngine::Init(int particlesNumber, int lifetimeMsMin, int lifetimeMsMax, double particleScaleIncrement, PNGLoader* particleImage)
{
  _numParticles = particlesNumber;
  _particleImage = particleImage;
  _particlesLifeTimeMsMin = lifetimeMsMin;
  _particlesLifeTimeMsMax = lifetimeMsMax;
  _particleScaleIncrement = particleScaleIncrement;

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

    p.Active = true;
    p.CurrentLifeTimeMs = 0;
    int lt = rand() % lifetimeMsMax;
    if (lt == 0) lt = lifetimeMsMin;
    p.MaxLifeTimeMs = lt;
    //p.MaxLifeTimeMs = 1000;
    p.Speed = 0.0;
    //p.Speed = 0.15 / (double)(rand() % 10 + 2);
    p.ScaleFactor = 1.0;

    _particles.push_back(p);
  }
}

void ParticleEngine::SetUp(Vector2 pos, Vector2 dir, double bulletSpeed)
{
  _position = pos;
  _direction = dir;
  _bulletSpeed = bulletSpeed;

  _direction.Normalize();

  for (auto& i : _particles)
  {
    i.Direction = dir;
    i.Position = pos;
    i.Speed = bulletSpeed;
  }
}

void ParticleEngine::Emit()
{
  if (!_active) return;

  for (auto& i : _particles)
  {
    double dx = _direction.X() * (i.Speed * GameTime::Get().DeltaTime());
    double dy = _direction.Y() * (i.Speed * GameTime::Get().DeltaTime());

    i.Position.Set(i.Position.X() + dx, i.Position.Y() + dy);

    _dstRect.x = i.Position.X() - (_particleImage->Width() * i.ScaleFactor) / 2;
    _dstRect.y = i.Position.Y() - (_particleImage->Height() * i.ScaleFactor) / 2;
    _dstRect.w = _particleImage->Width() * i.ScaleFactor;
    _dstRect.h = _particleImage->Height() * i.ScaleFactor;

    int res = SDL_RenderCopy(VideoSystem::Get().Renderer(), _particleImage->Texture(), &_srcRect, &_dstRect);
    if (res != 0) Logger::Get().LogPrint("(warning) Render copy error!\nReason: %s\n", SDL_GetError());

    i.CurrentLifeTimeMs += GameTime::Get().DeltaTime();
    i.ScaleFactor -= _particleScaleIncrement;

    if (i.ScaleFactor < 0.0) i.ScaleFactor = 0.0;

    if (i.CurrentLifeTimeMs > i.MaxLifeTimeMs)
    {
      i.CurrentLifeTimeMs = 0;
      int lt = rand() % _particlesLifeTimeMsMax;
      if (lt == 0) lt = _particlesLifeTimeMsMin;
      i.MaxLifeTimeMs = lt;
      i.Position.Set(_position);
      i.ScaleFactor = 1.0;
    }
  }
}
