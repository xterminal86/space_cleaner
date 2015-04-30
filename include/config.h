#ifndef CONFIG_H
#define CONFIG_H

#include "globals.h"

class Config
{
  public:
    static Config& Get()
    {
      static Config instance;
      return instance;
    }
    void Init();
    int GetValue(std::string key);
  protected:
  private:
    Config();
    virtual ~Config();
    Config(const Config&);
    Config& operator= (const Config&);

    std::map<std::string, int> _config;
};

#endif // CONFIG_H
