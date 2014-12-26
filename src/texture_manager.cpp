#include "texture_manager.h"

TextureManager::TextureManager()
{
  _initialized = false;
}

int TextureManager::Init(std::string imagesFilename, std::string relationFilename)
{
  if (!_initialized)
  {
    FILE* f = fopen(imagesFilename.c_str(), "r");
    if (f == nullptr)
    {
      Logger::Get().LogPrint("!!! ERROR !!! Could not open file %s!\n", imagesFilename.data());
      exit(1);
    }
    Logger::Get().LogPrint("Loading images...\n");
    char buf[512];
    int index = 0;
    while (!feof(f))
    {
      std::string png;
      fscanf(f, "%i %s", &index, buf);
      _textures[index] = std::unique_ptr<PNGLoader>(new PNGLoader(buf));
      Logger::Get().LogPrint("----|_textures[%i] (0x%zX) = %s\n", index, _textures[index].get(), buf);
      png = buf;
      LoadCollider(index, png);
      //index++;
    }
    fclose(f);
    Logger::Get().LogPrint("*** SUCCESS *** Done creating textures!\n");
    BuildDatabase(relationFilename);
    _initialized = true;
  }
  else
  {
    Logger::Get().LogPrint("WARNING!!! Texture Manager already initialized!\n");
  }

  return 0;
}

void TextureManager::LoadCollider(int textureIndex, std::string filename)
{
  Logger::Get().LogPrint("Loading collider data...\n");
  filename.replace(filename.end() - 3, filename.end(), "txt");
  FILE* f = fopen(filename.data(), "r");
  if (f == nullptr)
  {
    Logger::Get().LogPrint("(warning) Could not open collider data file %s!\n", filename.data());
    return;
  }

  int x = 0, y = 0;
  while (!feof(f))
  {
    SDL_Point point;
    fscanf(f, "%i %i", &x, &y);
    point.x = x;
    point.y = y;
    _colliders[textureIndex].push_back(point);
  }
  fclose(f);

  Logger::Get().LogPrint("Collider data loaded!\n");
}

void TextureManager::BuildDatabase(std::string filename)
{
  Logger::Get().LogPrint("Establishing sprites relation...\n");
  FILE* f = fopen(filename.data(), "r");
  if (f == nullptr)
  {
    Logger::Get().LogPrint("!!! ERROR !!! Could not open relation file %s!\n", filename.data());
    exit(1);
  }

  int index = 0;
  char buf[512];
  while (!feof(f))
  {
    std::string role;
    fscanf(f, "%i %s", &index, buf);
    role = buf;
    _spritesRelation[index] = role;
  }
  fclose(f);

  Logger::Get().LogPrint("Database loaded!\n");
}
