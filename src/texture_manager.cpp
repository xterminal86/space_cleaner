#include "texture_manager.h"

TextureManager::TextureManager()
{
  _initialized = false;
}

int TextureManager::Init(std::string filename)
{
  if (!_initialized)
  {
    FILE* f = fopen(filename.c_str(), "r");
    if (f == nullptr)
    {
      Logger::Get().LogPrint("!!! ERROR !!! Could not open file %s!\n", filename.c_str());
      exit(1);
    }
    Logger::Get().LogPrint("Loading images...\n");
    char buf[512];
    int index = 0;
    while (!feof(f))
    {
      fscanf(f, "%i %s", &index, buf);
      _textures[index] = std::unique_ptr<PNGLoader>(new PNGLoader(buf));
      Logger::Get().LogPrint("----|_textures[%i] (0x%zX) = %s\n", index, _textures[index].get(), buf);
    }

    fclose(f);
    Logger::Get().LogPrint("*** SUCCESS *** Done creating textures!\n");
    _initialized = true;
  }
  else
  {
    Logger::Get().LogPrint("WARNING!!! Texture Manager already initialized!\n");
  }

  return 0;
}
