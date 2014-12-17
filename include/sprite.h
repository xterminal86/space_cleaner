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
    void Draw(int x, int y);
    void SetScaleFactor(float scaleFactor) { _scaleFactor = scaleFactor; }
    const PNGLoader* ImageWrapper() { return _imageWrapper; }
    SDL_Rect* SourceRect() { return &_sourceRect; }
    SDL_Rect* DestinationRect() { return &_destRect; }
  private:
    SDL_Rect _sourceRect;
    SDL_Rect _destRect;
    const PNGLoader* _imageWrapper;
    int _screenX;
    int _screenY;
    float _scaleFactor;
};

#endif // SPRITE_H
