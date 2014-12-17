#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "png_loader.h"

class TextureManager
{
  public:
    static TextureManager& Get()
    {
      static TextureManager instance;
      return instance;
    }
    int Init(std::string filename);
    SDL_Texture* GetSDLTexture(int index)
    {
      if (_textures.count(index)) return _textures[index]->Texture();
      else return nullptr;
    }
    PNGLoader* GetTextureWrapper(int index)
    {
      if (_textures.count(index)) return _textures[index].get();
      else return nullptr;
    }
  protected:
  private:
    bool _initialized;

    std::map<int, std::unique_ptr<PNGLoader>> _textures;

    TextureManager();
    TextureManager(const TextureManager&);
    ~TextureManager()
    {
      Logger::Get().LogPrint("Deallocating texture memory...\n");
      _textures.clear();
    }
    TextureManager& operator=(const TextureManager&);
};

#endif // TEXTUREMANAGER_H
