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
  Logger::Get().LogPrint("Reading config...\n");

  std::ifstream f(GlobalStrings::ConfigFilename);
  if (!f.is_open())
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
   while (!f.eof())
   {
     f >> key;
     f >> value;
     _config[key] = value;
   }

   for (auto& i : _config)
   {
     Logger::Get().LogPrint("----| %s = %i\n", i.first.data(), i.second);
   }
  }

  f.close();
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
