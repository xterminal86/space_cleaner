#include "png_loader.h"
#include "config.h"

PNGLoader::PNGLoader()
{
  _texture = nullptr;
  _textureFormat = 0;
  _textureAccess = 0;
  _textureWidth = -1;
  _textureHeight = -1;

  Logger::Get().LogPrint("PNGLoader::ctor() 0x%zX\n", this);
}

PNGLoader::PNGLoader(std::string filename, int filterMode, bool fromDisk)
{
  Logger::Get().LogPrint("PNGLoader::ctor(std::string) 0x%zX\n", this);

  // Filtering is applied during texture creation
  //
  // "0" - nearest
  // "1" - linear
  // "2" - anisotropic

  if (filterMode == 0) SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
  else if (filterMode == 2) SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
  else SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

  _texture = nullptr;

  if (fromDisk)
  {
    LoadFromDisk(filename);
  }
  else
  {
    LoadFromMemory(filename);
  }

  Logger::Get().LogPrint("[Created texture for %s (0x%zX)]\n", filename.data(), _texture);
  SDL_QueryTexture(_texture, &_textureFormat, &_textureAccess, &_textureWidth, &_textureHeight);
  Logger::Get().LogPrint("----|Format: %u\n", _textureFormat);
  Logger::Get().LogPrint("----|Access: %i\n", _textureAccess);
  Logger::Get().LogPrint("----|Width:  %i\n", _textureWidth);
  Logger::Get().LogPrint("----|Height: %i\n", _textureHeight);
}

// ==================== Private Methods =================== //

void PNGLoader::LoadFromDisk(std::string& filename)
{
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
}

void PNGLoader::LoadFromMemory(std::string& filename)
{
  auto file = Config::Get().GetFileFromMemory(filename);
  if (file == nullptr)
  {
    Logger::Get().LogPrint("!!! ERROR !!! Could not find file %s in memory!\n", filename.data());
    return;
  }

  SDL_RWops* data = SDL_RWFromMem(file->data(), file->size());
  if (data == nullptr)
  {
    Logger::Get().LogPrint("!!! ERROR !!! Could not create SDL_RWops!\n");
    return;
  }

  SDL_Surface* surface = IMG_Load_RW(data, 1);
  if (surface == nullptr)
  {
    Logger::Get().LogPrint("!!! ERROR !!! Could not create SDL_Surface for %s!\n", filename.data());
    return;
  }

  _texture = SDL_CreateTextureFromSurface(VideoSystem::Get().Renderer(), surface);
  SDL_FreeSurface(surface);
}
