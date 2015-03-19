#include "sprite_animated.h"

SpriteAnimated::SpriteAnimated()
{
  _spriteSheet = std::unique_ptr<PNGLoader>(new PNGLoader(GlobalStrings::ExplosionSpriteFilename));

  std::string fname = GlobalStrings::ExplosionSpriteFilename;
  fname.replace(fname.end() - 3, fname.end(), "txt");
  FILE* f = fopen(fname.data(), "r");
  while (!feof(f))
  {
    fscanf(f, "%i %i", &_lengthX, &_lengthY);
  }

  _frames = _lengthX * _lengthY;

  _wx = _spriteSheet.get()->Width() / _lengthX;
  _wy = _spriteSheet.get()->Height() / _lengthY;

  _src.x = 0;
  _src.y = 0;
  _src.w = _wx;
  _src.h = _wy;

  _dst.x = 0;
  _dst.y = 0;
  _dst.w = _wx;
  _dst.h = _wy;

  _framesPlayed = 0;

  _active = false;

  _currentMsPassed = 0;
}

SpriteAnimated::~SpriteAnimated()
{
  //dtor
}

void SpriteAnimated::Play(int x, int y)
{
  _active = true;
  _framesPlayed = 0;

  _dst.x = x - _wx / 2;
  _dst.y = y - _wy / 2;
}

void SpriteAnimated::Draw()
{
  if (!_active) return;

  int res = SDL_RenderCopy(VideoSystem::Get().Renderer(), _spriteSheet.get()->Texture(), &_src, &_dst);
  if (res != 0) Logger::Get().LogPrint("(warning) Render copy error!\nReason: %s\n", SDL_GetError());

  _currentMsPassed += GameTime::Get().DeltaTime();

  if (_currentMsPassed > SpeedMs)
  {
    _currentMsPassed = 0.0;

    _framesPlayed++;

    int dy = _framesPlayed / _lengthX;
    int dx = _framesPlayed % _lengthY;

    _src.x = dx * _wx;
    _src.y = dy * _wy;

    if (_framesPlayed > _frames)
    {
      _active = false;
    }
  }
}
