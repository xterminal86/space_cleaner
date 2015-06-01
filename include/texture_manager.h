#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "png_loader.h"

// Loads and stores all sprites
// Provides methods for searching them.
class TextureManager
{
  public:
    static TextureManager& Get()
    {
      static TextureManager instance;
      return instance;
    }
    int Init(std::string imagesFilename, std::string relationFilename, bool fromDisk = true);
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
    int FindTextureByRole(std::string rolename);
  protected:
  private:
    static TextureManager instance;

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

      _spritesRelation.clear();
    }
    TextureManager& operator=(const TextureManager&);

    void LoadCollider(int textureIndex, std::string filename, bool fromDisk = true);
    void BuildDatabase(std::string filename, bool fromDisk = true);
    void PrintColliderData(int textureIndex);
    void LoadImages(std::string& filename, std::string& relationFilename, bool fromDisk = true);
};

#endif // TEXTUREMANAGER_H
