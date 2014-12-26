#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "png_loader.h"

// TODO: we need to relate different .png's to certain classes somehow (e.g. ship.png is ship class and so on).
// Maybe via another .txt file that will describe such correspondence.
class TextureManager
{
  public:
    static TextureManager& Get()
    {
      static TextureManager instance;
      return instance;
    }
    int Init(std::string imagesFilename, std::string relationFilename);
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
    std::vector<SDL_Point>* GetCollider(int textureIndex)
    {
      if (_colliders.count(textureIndex)) return &_colliders[textureIndex];
      else return nullptr;
    }
    std::map<int, std::string>& Relation() { return _spritesRelation; }
  protected:
  private:
    bool _initialized;

    std::map<int, std::unique_ptr<PNGLoader>> _textures;
    std::map<int, std::vector<SDL_Point>> _colliders;
    std::map<int, std::string> _spritesRelation;

    TextureManager();
    TextureManager(const TextureManager&);
    ~TextureManager()
    {
      Logger::Get().LogPrint("Deallocating texture memory...\n");
      _textures.clear();

      for (int i = 0; i < _colliders.size(); i++)
      {
        _colliders[i].clear();
      }

      _colliders.clear();
    }
    TextureManager& operator=(const TextureManager&);

    void LoadCollider(int textureIndex, std::string filename);
    void BuildDatabase(std::string filename);
};

#endif // TEXTUREMANAGER_H
