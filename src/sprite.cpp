#include "sprite.h"

Sprite::Sprite()
{
  //printf ("Sprite ctor\t");

  _screenX = 0;
  _screenY = 0;

  _angle = 0.0;
  _scaleFactor = 1.0;

  _imageWrapper = nullptr;

  _spriteColor = Colors::AsIs;
}

Sprite::~Sprite()
{
  //printf ("Sprite dtor 0x%x 0x%x\t", this, _imageWrapper);
}

void Sprite::CalculateSATAxes()
{
  _projectionAxes.clear();
  _projectionAxesV2.clear();

  size_t length = _translatedCollider.size();
  for (int i = 0; i < length - 1; i++)
  {
    SDL_Point p1 = _translatedCollider[i];
    SDL_Point p2 = _translatedCollider[i + 1];

    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;

    SDL_Point p;
    Vector2 v;

    p.x = dy;
    p.y = -dx;

    v.Set(p.x, p.y);
    v.Normalize();

    _projectionAxes.push_back(p);
    _projectionAxesV2.push_back(v);
  }
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
      _scaledCollider.push_back(_originalCollider->at(i));
    }
  }

  return 0;
}

void Sprite::SetScaleFactor(double scaleFactor)
{
  _scaleFactor = scaleFactor;

  for (int i = 0; i < _originalCollider->size(); i++)
  {
    double tmpX = (double)_originalCollider->at(i).x * scaleFactor;
    double tmpY = (double)_originalCollider->at(i).y * scaleFactor;

    _scaledCollider[i].x = (int)tmpX;
    _scaledCollider[i].y = (int)tmpY;
  }

  _destRect.w = _imageWrapper->Width() * _scaleFactor;
  _destRect.h = _imageWrapper->Height() * _scaleFactor;
}

void Sprite::SetAngle(double angle)
{
  _angle = angle;

  int cs = _scaledCollider.size();
  Vector2 res;
  for (int i = 0; i < cs; i++)
  {
    Vector2 tmp(_scaledCollider[i].x, _scaledCollider[i].y);
    Vector2::RotateVector(res, Vector2(0.0, 0.0), tmp, angle);

    _rotatedCollider[i].x = res.X();
    _rotatedCollider[i].y = res.Y();
  }

}

// In order to avoid "acceleration" of results, we always offset from some point when performing calculations.
// I.e. first we take original collider, rotate it and remember the values of rotated points,
// then we offset those points to somewhere we want.
// Previously offset was taken from _originalCollider, thus incrementing the wrong position in the first place.
// And if we would just replace _originalCollider with += of the _rotatedCollider, it would "fly away", since
// increment performs every frame. Therefore, three colliders.
void Sprite::MoveCollider(double newX, double newY)
{
  int csize = _rotatedCollider.size();
  for (int i = 0; i < csize; i++)
  {
    _translatedCollider[i].x = _rotatedCollider[i].x + newX;
    _translatedCollider[i].y = _rotatedCollider[i].y + newY;
  }
}

void Sprite::Draw(int x, int y)
{
  if (_imageWrapper == nullptr)
  {
    Logger::Get().LogPrint("(warning) Sprite 0x%zX _imageWrapper is NULL, so drawing nothing\n", this);
    return;
  }

  _destRect.x = x - (_imageWrapper->Width() * _scaleFactor) / 2;
  _destRect.y = y - (_imageWrapper->Height() * _scaleFactor) / 2;

  SDL_SetTextureColorMod(_imageWrapper->Texture(), _spriteColor.r, _spriteColor.g, _spriteColor.b);
  SDL_SetTextureAlphaMod(_imageWrapper->Texture(), _spriteColor.a);

  int res = SDL_RenderCopyEx(VideoSystem::Get().Renderer(), _imageWrapper->Texture(), &_sourceRect, &_destRect, _angle, nullptr, SDL_FLIP_NONE);
  if (res != 0) Logger::Get().LogPrint("(warning) Render copy error!\nReason: %s\n", SDL_GetError());
}

void Sprite::Draw(int x, int y, std::vector<SDL_Point>* colliderToDraw)
{
  Draw(x, y);

  if (colliderToDraw != nullptr)
  {
    SDL_SetRenderDrawColor(VideoSystem::Get().Renderer(), 255, 255, 0, 255);
    SDL_RenderDrawLines(VideoSystem::Get().Renderer(), colliderToDraw->data(), colliderToDraw->size());
  }
}

void Sprite::SetColor(SDL_Color c)
{
  _spriteColor = c;
}
