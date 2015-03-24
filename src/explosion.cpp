#include "explosion.h"

Explosion::Explosion()
{
  //ctor
}

Explosion::~Explosion()
{
  //dtor
}

void Explosion::Init(PNGLoader* imageRef, int subdivisionsPower, int lifeTimeMs)
{
  _imageRef = imageRef;
  _subdivisionsPower = subdivisionsPower;
  _lifeTimeMs = lifeTimeMs;

  _scaleFactor = 1.0;
  _alphaValue = 0.0;

  int totalSubs = subdivisionsPower * subdivisionsPower;

  _divX = imageRef->Width() / _subdivisionsPower;
  _divY = imageRef->Height() / _subdivisionsPower;

  int x = 0;
  int y = 0;
  double constantSpeed = 0.1 / (double)(Util::RandomNumber() % GameMechanic::RandomSpeedSpread + 1);
  for (int i = 0; i <= totalSubs; i++)
  {
    Fragment f;

    f.Angle = 0.0;
    f.RandomAngleIncrement = Util::CreateRandomRotation();
    Util::CreateRandomDirection(f.Direction);
    //f.Speed = GameMechanic::AsteroidMaxSpeed / (double)(Util::RandomNumber() % GameMechanic::RandomSpeedSpread + 1);
    f.Speed = constantSpeed;

    f.Color.r = 255;
    f.Color.g = 255;
    f.Color.b = 255;

    f.SrcRect.x = x * _divX;
    f.SrcRect.y = y * _divY;
    f.SrcRect.w = _divX;
    f.SrcRect.h = _divY;

    f.DstRect.x = 0;
    f.DstRect.y = 0;
    f.DstRect.w = _divX;
    f.DstRect.h = _divY;

    _fragments.push_back(f);

    x++;

    if (x > 2)
    {
      x = 0;
      y++;
    }
  }
}

void Explosion::Play(Vector2 pos)
{
  _active = true;
  _msPassed = 0;
  _alphaValue = 0;

  for (auto& i : _fragments)
  {
    i.Color.a = 255;
    i.Position = pos;
    Util::CreateRandomDirection(i.Direction);
    i.Active = true;
  }
}

void Explosion::Process()
{
  if (!_active) return;

  _msPassed += GameTime::Get().DeltaTime();
  _alphaValue = 255.0 * ( (double)_msPassed / (double)_lifeTimeMs );

  bool allDone = true;
  for (auto& i : _fragments)
  {
    if (!i.Active) continue;

    allDone = false;

    double newX = i.Position.X() + i.Direction.X() * (i.Speed * GameTime::Get().DeltaTime());
    double newY = i.Position.Y() + i.Direction.Y() * (i.Speed * GameTime::Get().DeltaTime());

    i.DstRect.x = i.Position.X() - (_divX * _scaleFactor) / 2;
    i.DstRect.y = i.Position.Y() - (_divY * _scaleFactor) / 2;

    i.Angle += i.RandomAngleIncrement;

    i.Color.a = 255 - (int)_alphaValue;

    if (_alphaValue > 255)
    {
      i.Color.a = 0;
      i.Active = false;
    }

    SDL_SetTextureColorMod(_imageRef->Texture(), i.Color.r, i.Color.g, i.Color.b);
    SDL_SetTextureAlphaMod(_imageRef->Texture(), i.Color.a);

    i.Position.Set(newX, newY);

    int res = SDL_RenderCopyEx(VideoSystem::Get().Renderer(), _imageRef->Texture(), &i.SrcRect, &i.DstRect, i.Angle, nullptr, SDL_FLIP_NONE);
    if (res != 0) Logger::Get().LogPrint("(warning) Render copy error!\nReason: %s\n", SDL_GetError());
  }

  _active = !allDone;
}
