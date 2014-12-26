#ifndef SPRITE_H
#define SPRITE_H

#include "globals.h"
#include "png_loader.h"
#include "texture_manager.h"

class Sprite
{
  public:
    Sprite();
    virtual ~Sprite();
    int Init(int textureIndex);
    void Draw(int x, int y, double angle = 0.0, bool drawCollider = false);
    void SetScaleFactor(float scaleFactor) { _scaleFactor = scaleFactor; }
    const PNGLoader* ImageWrapper() { return _imageWrapper; }
    SDL_Rect* SourceRect() { return &_sourceRect; }
    SDL_Rect* DestinationRect() { return &_destRect; }
    std::vector<SDL_Point>* OriginalCollider() { return _originalCollider; }
  private:
    SDL_Rect _sourceRect;
    SDL_Rect _destRect;
    const PNGLoader* _imageWrapper;
    int _screenX;
    int _screenY;
    float _scaleFactor;
    std::vector<SDL_Point>* _originalCollider;
};

#endif // SPRITE_H
