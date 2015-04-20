#include "star.h"

Star::Star()
{
  //ctor
}

Star::~Star()
{
  //dtor
}

void Star::Init(PNGLoader* imageRef)
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

  c.r = Util::RandomNumber() % 200 + 55;
  c.g = Util::RandomNumber() % 200 + 55;
  c.b = Util::RandomNumber() % 200 + 55;
  c.a = 0;
  */

  SDL_Color c;

  c.r = 255;
  c.g = 255;
  c.b = 255;
  c.a = 0;

  _color = c;
  _scaleFactor = 0.15 / (double)(Util::RandomNumber() % 2 + 1);
  _alpha = 0;
  _alphaIncrement = (Util::RandomNumber() % 50 + 1) / 100.0;
  //_alphaIncrement = 4.0 / (double)(Util::RandomNumber() % 10 + 1);

  _msPassed = 0;

  _active = true;

  SetRandomPosition();
}

void Star::Pulse()
{
  if (!_active)
  {
    _msPassed += GameTime::Get().DeltaTimeMs();

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

  _alpha += _alphaIncrement * GameTime::Get().DeltaTimeMs();

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

void Star::SetRandomPosition()
{
  int x = Util::RandomNumber() % VideoSystem::Get().ScreenDimensions().x;
  int y = Util::RandomNumber() % VideoSystem::Get().ScreenDimensions().y;

  _position.Set(x, y);

  //_pauseMs = Util::RandomNumber() % 2000 + 3000;
  _pauseMs = Util::RandomNumber() % 1000 + 1000;
}
