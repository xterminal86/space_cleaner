#include "png_loader.h"

PNGLoader::PNGLoader()
{
  _texture = nullptr;
  _textureFormat = 0;
  _textureAccess = 0;
  _textureWidth = -1;
  _textureHeight = -1;

  Logger::Get().LogPrint("PNGLoader::ctor() 0x%zX\n", this);
}

PNGLoader::PNGLoader(std::string filename)
{
  Logger::Get().LogPrint("PNGLoader::ctor(std::string) 0x%zX\n", this);

  // Filtering is applied during texture creation
  //
  // "0" - nearest
  // "1" - linear
  // "2" - anisotropic

  if (filename == "assets/fonts/font_fixed.png")
  {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
  }
  else
  {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
  }

  _texture = nullptr;
  SDL_Surface* surface = IMG_Load(filename.c_str());
  if (surface == nullptr)
  {
     Logger::Get().LogPrint("(warning) Could not load %s!\nReason: %s\n", filename.data(), IMG_GetError());
     return;
  }
  _texture = SDL_CreateTextureFromSurface(VideoSystem::Get().Renderer(), surface);
  SDL_FreeSurface(surface);
  if (_texture == nullptr)
  {
     Logger::Get().LogPrint("(warning) Could not create texture from surface!\nReason: %s\n", SDL_GetError());
     SDL_FreeSurface(surface);
     return;
  }
  Logger::Get().LogPrint("[Created texture for %s (0x%zX)]\n", filename.data(), _texture);
  SDL_QueryTexture(_texture, &_textureFormat, &_textureAccess, &_textureWidth, &_textureHeight);
  Logger::Get().LogPrint("----|Format: %u\n", _textureFormat);
  Logger::Get().LogPrint("----|Access: %i\n", _textureAccess);
  Logger::Get().LogPrint("----|Width:  %i\n", _textureWidth);
  Logger::Get().LogPrint("----|Height: %i\n", _textureHeight);
}
