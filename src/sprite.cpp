#include "sprite.h"

Sprite::Sprite()
{
  //printf ("Sprite ctor\t");

  _screenX = 0;
  _screenY = 0;
  _scaleFactor = 1.0f;
  _imageWrapper = nullptr;
}

Sprite::~Sprite()
{
  //printf ("Sprite dtor\t");
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

  if (_originalCollider != nullptr)
  {
    for (int i = 0; i < _originalCollider->size(); i++)
    {
      _rotatedCollider.push_back(_originalCollider->at(i));
      _translatedCollider.push_back(_originalCollider->at(i));
    }
  }

  return 0;
}

void Sprite::MoveCollider(double newX, double newY)
{
  int csize = _rotatedCollider.size();
  for (int i = 0; i < csize; i++)
  {
    _translatedCollider[i].x = _rotatedCollider[i].x + newX;
    _translatedCollider[i].y = _rotatedCollider[i].y + newY;
  }
}

void Sprite::Draw(int x, int y, double angle)
{
  if (_imageWrapper == nullptr)
  {
    Logger::Get().LogPrint("(warning) Sprite 0x%zX _imageWrapper is NULL, so drawing nothing\n", this);
    return;
  }

  _destRect.x = x - _imageWrapper->Width() / 2;
  _destRect.y = y - _imageWrapper->Height() / 2;

  int res = SDL_RenderCopyEx(VideoSystem::Get().Renderer(), _imageWrapper->Texture(), &_sourceRect, &_destRect, angle, nullptr, SDL_FLIP_NONE);
  if (res != 0) Logger::Get().LogPrint("(warning) Render copy error!\nReason: %s\n", SDL_GetError());
}

void Sprite::Draw(int x, int y, double angle, std::vector<SDL_Point>* colliderToDraw)
{
  Draw(x, y, angle);

  if (colliderToDraw != nullptr)
  {
    SDL_SetRenderDrawColor(VideoSystem::Get().Renderer(), 255, 255, 0, 255);
    SDL_RenderDrawLines(VideoSystem::Get().Renderer(), colliderToDraw->data(), colliderToDraw->size());
  }
}
