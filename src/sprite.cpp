#include "sprite.h"

Sprite::Sprite()
{
  _screenX = 0;
  _screenY = 0;
  _scaleFactor = 1.0f;
  _imageWrapper = nullptr;
}

Sprite::~Sprite()
{
}

int Sprite::Init(int textureIndex)
{
  _imageWrapper = TextureManager::Get().GetTextureWrapper(textureIndex);

  if (_imageWrapper == nullptr)
  {
    Logger::Get().LogPrint("(warning) Could not find texture with index %i!\n", textureIndex);
    return 1;
  }

  _screenX = 0;
  _screenY = 0;

  _sourceRect.x = 0;
  _sourceRect.y = 0;
  _sourceRect.w = _imageWrapper->Width();
  _sourceRect.h = _imageWrapper->Height();

  _destRect.x = _screenX;
  _destRect.y = _screenY;
  _destRect.w = _imageWrapper->Width() * _scaleFactor;
  _destRect.h = _imageWrapper->Height() * _scaleFactor;

  return 0;
}

void Sprite::Draw(int x, int y)
{
  if (_imageWrapper == nullptr)
  {
    Logger::Get().LogPrint("(warning) Sprite 0x%zX _imageWrapper is NULL, so drawing nothing\n", this);
    return;
  }

  _destRect.x = x;
  _destRect.y = y;

  SDL_RenderClear(VideoSystem::Get().Renderer());
  int res = SDL_RenderCopy(VideoSystem::Get().Renderer(), _imageWrapper->Texture(), &_sourceRect, &_destRect);
  if (res != 0) Logger::Get().LogPrint("(warning) Render copy error!\nReason: %s\n", SDL_GetError());
  SDL_RenderPresent(VideoSystem::Get().Renderer());
}
