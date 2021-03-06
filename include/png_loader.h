#ifndef PNG_LOADER
#define PNG_LOADER

#include "globals.h"
#include "video_system.h"
#include "logger.h"

// Wrapper class for SDL_Texture
class PNGLoader
{
  unsigned int _textureFormat;

  int _textureAccess;
  int _textureWidth;
  int _textureHeight;

  SDL_Texture* _texture;
public:
  PNGLoader();
  PNGLoader(std::string filename, int filterMode = 1, bool fromDisk = true);
  ~PNGLoader()
  {
    Logger::Get().LogPrint("----|[PNGLoader::dtor()]: 0x%zX (object) 0x%zX (texture)\n", this, _texture);
    SDL_DestroyTexture(_texture);
  }

  int Access() const { return _textureAccess; }
  int Width() const { return _textureWidth; }
  int Height() const { return _textureHeight; }
  SDL_Texture* Texture() const { return _texture; }

  void LoadFromDisk(std::string& filename);
  void LoadFromMemory(std::string& filename);
};

#endif
