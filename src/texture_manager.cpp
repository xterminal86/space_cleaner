#include "texture_manager.h"
#include "config.h"

TextureManager::TextureManager()
{
  _initialized = false;
}

// TODO: we need to relate different .png's to certain classes somehow (e.g. ship.png is ship class and so on).
// Maybe via another .txt file that will describe such correspondence.
int TextureManager::Init(std::string imagesFilename, std::string relationFilename, bool fromDisk)
{
  if (!_initialized)
  {
    LoadImages(imagesFilename, relationFilename, fromDisk);
    _initialized = true;
  }
  else
  {
    Logger::Get().LogPrint("WARNING!!! Texture Manager already initialized!\n");
  }

  return 0;
}

void TextureManager::LoadCollider(int textureIndex, std::string filename, bool fromDisk)
{
  Logger::Get().LogPrint("Loading collider data...\n");

  filename.replace(filename.end() - 3, filename.end(), "txt");

  if (fromDisk)
  {
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

      // Collider points are given relative to pixels of picture.
      // We offset collider coordinates by image's width and height, which will position it centered around (0; 0).
      // Thus we can call some Draw(x, y) method and our sprite will position centered around that given point.

      point.x = x - _textures[textureIndex]->Width() / 2;
      point.y = y - _textures[textureIndex]->Height() / 2;

      _colliders[textureIndex].push_back(point);
    }
    fclose(f);
  }
  else
  {
    char buf[512];
    int x = 0, y = 0;

    std::string asciiFile = Config::Get().ConvertFileToAscii(filename);

    std::istringstream iss(asciiFile);
    while (iss.getline(buf, 512))
    {
      SDL_Point point;
      sscanf(buf, "%i %i", &x, &y);

      point.x = x - _textures[textureIndex]->Width() / 2;
      point.y = y - _textures[textureIndex]->Height() / 2;

      _colliders[textureIndex].push_back(point);
    }
  }

  Logger::Get().LogPrint("Collider data loaded!\n");

  PrintColliderData(textureIndex);
}

void TextureManager::BuildDatabase(std::string filename, bool fromDisk)
{
  Logger::Get().LogPrint("Establishing sprites relation...\n");

  if (fromDisk)
  {
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
  }
  else
  {
    int index = 0;
    char buf[512];

    std::string asciiFile = Config::Get().ConvertFileToAscii(filename);

    std::istringstream iss(asciiFile);
    while (iss.getline(buf, 512))
    {
      char localbuf[256];
      std::string role;
      sscanf(buf, "%i %s", &index, localbuf);
      role = localbuf;
      _spritesRelation[index] = role;
    }
  }

  Logger::Get().LogPrint("Database loaded!\n");
}

int TextureManager::FindTextureByRole(std::string rolename)
{
  for (int i = 0; i < _spritesRelation.size(); i++)
  {
    if (_spritesRelation[i] == rolename)
    {
      return i;
    }
  }

  Logger::Get().LogPrint("(warning) Could not find texture for role %s in images database!\n", rolename.data());

  return -1;
}

void TextureManager::PrintColliderData(int textureIndex)
{
  Logger::Get().LogPrint("Collider data for _colliders[%i]\n", textureIndex);

  int counter = 1;
  for (auto& point : _colliders[textureIndex])
  {
    Logger::Get().LogPrint("\t%i -> [%i ; %i] \n", counter, point.x, point.y);
    counter++;
  }
}

void TextureManager::LoadImages(std::string& imagesFilename, std::string& relationFilename, bool fromDisk)
{
  Logger::Get().LogPrint("Loading images...\n", imagesFilename.data());

  if (fromDisk)
  {
    FILE* f = fopen(imagesFilename.c_str(), "r");
    if (f == nullptr)
    {
      Logger::Get().LogPrint("!!! ERROR !!! Could not open file %s!\n", imagesFilename.data());
      exit(1);
    }

    char buf[512];
    int index = 0, filterMode = 0;
    while (!feof(f))
    {
      std::string png;
      fscanf(f, "%i %s %i", &index, buf, &filterMode);
      _textures[index] = std::unique_ptr<PNGLoader>(new PNGLoader(buf, filterMode));
      Logger::Get().LogPrint("----|_textures[%i] (0x%zX) = %s\n", index, _textures[index].get(), buf);
      png = buf;
      LoadCollider(index, png, fromDisk);
    }
    fclose(f);
  }
  else
  {
    char buf[512];
    int index = 0, filterMode = 0;

    std::string asciiFile = Config::Get().ConvertFileToAscii(imagesFilename);

    std::istringstream iss(asciiFile);
    while (iss.getline(buf, 512))
    {
      char filename[256];
      std::string png;
      sscanf(buf, "%i %s %i", &index, filename, &filterMode);
      _textures[index] = std::unique_ptr<PNGLoader>(new PNGLoader(filename, filterMode, false));
      Logger::Get().LogPrint("----|_textures[%i] (0x%zX) = %s\n", index, _textures[index].get(), filename);
      png = filename;
      LoadCollider(index, png, fromDisk);
    }
  }

  Logger::Get().LogPrint("*** SUCCESS *** Done creating textures!\n");
  BuildDatabase(relationFilename, fromDisk);
}
