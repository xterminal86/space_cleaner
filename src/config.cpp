#include "config.h"

Config::Config()
{
  //ctor
}

Config::~Config()
{
  //dtor
}

void Config::Init()
{
  FILE* f = fopen(GlobalStrings::ConfigFilename.data(), "r");
  if (f == nullptr)
  {
    f = fopen(GlobalStrings::ConfigFilename.data(), "w");

    _config["screen_width"] = DefaultConfigValues::DefaultConfigPairs["screen_width"];
    _config["screen_height"] = DefaultConfigValues::DefaultConfigPairs["screen_height"];
    _config["sound_volume"] = DefaultConfigValues::DefaultConfigPairs["sound_volume"];
    _config["music_volume"] = DefaultConfigValues::DefaultConfigPairs["music_volume"];
    _config["fullscreen_flag"] = DefaultConfigValues::DefaultConfigPairs["fullscreen_flag"];

    /*
    for (auto& item : _config)
    {
      fprintf(f, "%s = %i\n", item.first.data(), item.second);
    }
    */
  }
  else
  {
    /*
    while (!feof())
    {
      fscanf(f, "%s = %i",
    }
    */
  }

  fclose(f);
}

int Config::GetValue(std::string key)
{
  if (_config.count(key) == 1)
  {
    return _config[key];
  }

  return -1;
}
