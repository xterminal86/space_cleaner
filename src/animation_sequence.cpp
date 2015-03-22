#include "animation_sequence.h"

AnimationSequence::AnimationSequence(AnimationsPool* owner)
{
  _owner = owner;
  _spriteSheetRef = _owner->SpriteSheet();

  _src.x = 0;
  _src.y = 0;
  _src.w = _owner->FrameWidthX();
  _src.h = _owner->FrameWidthY();

  _dst.x = 0;
  _dst.y = 0;
  _dst.w = _owner->FrameWidthX();
  _dst.h = _owner->FrameWidthY();

  _framesPlayed = 0;

  _active = false;

  _currentMsPassed = 0;
}

AnimationSequence::~AnimationSequence()
{
  //dtor
}

void AnimationSequence::Play(int x, int y, double scale)
{
  _active = true;
  _framesPlayed = 0;

  _dst.x = x - (_owner->FrameWidthX() * scale) / 2;
  _dst.y = y - (_owner->FrameWidthY() * scale) / 2;

  _dst.w = _owner->FrameWidthX() * scale;
  _dst.h = _owner->FrameWidthY() * scale;
}

void AnimationSequence::Draw()
{
  if (!_active) return;

  int res = SDL_RenderCopy(VideoSystem::Get().Renderer(), _spriteSheetRef->Texture(), &_src, &_dst);
  if (res != 0) Logger::Get().LogPrint("(warning) Render copy error!\nReason: %s\n", SDL_GetError());

  _currentMsPassed += GameTime::Get().DeltaTime();

  if (_currentMsPassed > _owner->SpeedMs)
  {
    _currentMsPassed = 0.0;

    _framesPlayed++;

    int dy = _framesPlayed / _owner->FramesNumX();
    int dx = _framesPlayed % _owner->FramesNumY();

    _src.x = dx * _owner->FrameWidthX();
    _src.y = dy * _owner->FrameWidthY();

    if (_framesPlayed > _owner->TotalFrames())
    {
      _active = false;
    }
  }
}
