#include "config.h"

Config::Config()
{
  //ctor
}

Config::~Config()
{
  //dtor
}

// Init() method checks existence of config.dat and assumes default values if it doesn't exist
void Config::Init()
{
  Logger::Get().LogPrint("Loading game files...\n");

  LoadGameArchive();

  Logger::Get().LogPrint("Reading config...\n");

  std::ifstream configFile(GlobalStrings::ConfigFilename);
  if (!configFile.is_open())
  {
    std::ofstream f(GlobalStrings::ConfigFilename);

    Logger::Get().LogPrint("(warning) Config could not be opened - assuming default values\n");

    for (auto& i : DefaultConfigValues::DefaultConfigPairs)
    {
      _config[i.first.data()] = i.second;
    }

    /*
    _config["screen_width"] = DefaultConfigValues::DefaultConfigPairs["screen_width"];
    _config["screen_height"] = DefaultConfigValues::DefaultConfigPairs["screen_height"];
    _config["sound_volume"] = DefaultConfigValues::DefaultConfigPairs["sound_volume"];
    _config["music_volume"] = DefaultConfigValues::DefaultConfigPairs["music_volume"];
    _config["fullscreen_flag"] = DefaultConfigValues::DefaultConfigPairs["fullscreen_flag"];
    _config["video_driver"] = DefaultConfigValues::DefaultConfigPairs["video_driver"];
    */

    for (auto& item : _config)
    {
      f << item.first.data() << " " << item.second << "\n";
    }

    f.close();
  }
  else
  {
   std::string key;
   int value = 0;
   while (!configFile.eof())
   {
     configFile >> key;
     configFile >> value;
     _config[key] = value;
   }

   for (auto& i : _config)
   {
     Logger::Get().LogPrint("----| %s = %i\n", i.first.data(), i.second);
   }
  }

  configFile.close();
}

int Config::GetValue(std::string key)
{
  if (_config.count(key) == 1)
  {
    return _config[key];
  }

  return -1;
}

void Config::SetValue(std::string key, int value)
{
  if (_config.count(key) == 1)
  {
    _config[key] = value;
  }
}

void Config::WriteConfig()
{
  std::ofstream f(GlobalStrings::ConfigFilename);
  if (f.is_open())
  {
    for (auto& i : _config)
    {
      f << i.first.data() << " " << i.second << "\n";
    }
  }
  f.close();
}

std::vector<char>* Config::GetFileFromMemory(std::string& filename)
{
  GetFileFromMemory((char*)filename.data());
}

std::vector<char>* Config::GetFileFromMemory(char* filename)
{
  return (_gameData.find(filename) == _gameData.end()) ? nullptr : &_gameData[filename];
}

std::string& Config::ConvertFileToAscii(std::string& filename)
{
  return ConvertFileToAscii((char*)filename.data());
}

std::string& Config::ConvertFileToAscii(char* filename)
{
  if (_gameData.find(filename) != _gameData.end())
  {
    _asciiFile.clear();
    auto fileBytes = GetFileFromMemory(filename);
    for (int i = 0; i < fileBytes->size(); i++)
    {
      _asciiFile.push_back(fileBytes->at(i));
    }
  }

  return _asciiFile;
}

long Config::GetFileFromMemorySize(std::string& filename)
{
  GetFileFromMemorySize((char*)filename.data());
}

long Config::GetFileFromMemorySize(char* filename)
{
  return (_gameDataSizes.find(filename) == _gameDataSizes.end()) ? -1 : _gameDataSizes[filename];
}

// ==================== Private Methods =================== //

void Config::LoadGameArchive()
{
  char _emptyBlock[512];
  for (int i = 0; i < 512; i++)
  {
    _emptyBlock[i] = '\0';
  }

  FILE* f = fopen(GlobalStrings::GameDataArchive.data(), "rb");

  if (f == nullptr)
  {
    Logger::Get().LogPrint("!!! ERROR !!! Could not found game data archive!\n");
    exit(1);
  }

  int count = 0;
  while (!feof(f))
  {
    if (count == 2)
    {
      Logger::Get().LogPrint("Game files loaded\n");
      break;
    }

    char fileSize[12];

    TarHeader header;
    fread(&header, sizeof(TarHeader), 1, f);

    for (int i = 0; i < sizeof(header.FileSizeBytesOctalBase); i++)
    {
      fileSize[i] = header.FileSizeBytesOctalBase[i];
    }

    long fileLengthBytes = strtol(fileSize, nullptr, 8);

    // File binary data
    if (fileLengthBytes != 0)
    {
      int blocks = (fileLengthBytes / sizeof(TarHeader)) + 1;
      char* fileData = new char[blocks * sizeof(TarHeader)];
      fread(fileData, blocks * sizeof(TarHeader), 1, f);
      FillTempVector(fileData, blocks * sizeof(TarHeader));
      _gameData[header.Filename] = _tmpVector;
      _gameDataSizes[header.Filename] = fileLengthBytes;
      delete fileData;
    }

    if (CompareBlocks(_emptyBlock, &header))
    {
      count++;
    }
  }

  fclose(f);
}

void Config::FillTempVector(char* array, int size)
{
  _tmpVector.clear();
  for (int i = 0; i < size; i++)
  {
    char byte = array[i];
    _tmpVector.push_back(byte);
  }
}

bool Config::CompareBlocks(void* block1, void* block2)
{
  char* b1 = (char*)block1;
  char* b2 = (char*)block2;

  for (int i = 0; i < 512; i++)
  {
    if (b1[i] != b2[i]) return false;
  }

  return true;
}
