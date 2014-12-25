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

  _originalCollider = TextureManager::Get().GetCollider(textureIndex);

  return 0;
}

void Sprite::Draw(int x, int y, double angle, bool drawCollider)
{
  if (_imageWrapper == nullptr)
  {
    Logger::Get().LogPrint("(warning) Sprite 0x%zX _imageWrapper is NULL, so drawing nothing\n", this);
    return;
  }

  _destRect.x = x;
  _destRect.y = y;

  int res = SDL_RenderCopyEx(VideoSystem::Get().Renderer(), _imageWrapper->Texture(), &_sourceRect, &_destRect, angle, nullptr, SDL_FLIP_NONE);
  if (res != 0) Logger::Get().LogPrint("(warning) Render copy error!\nReason: %s\n", SDL_GetError());
  if (drawCollider)
  {
    SDL_SetRenderDrawColor(VideoSystem::Get().Renderer(), 255, 255, 0, 255);
    SDL_RenderDrawLines(VideoSystem::Get().Renderer(), _originalCollider->data(), _originalCollider->size());
  }
}
