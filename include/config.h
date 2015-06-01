#ifndef CONFIG_H
#define CONFIG_H

#include "globals.h"
#include "logger.h"

// Singleton class of holding game config values
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
    void SetValue(std::string key, int value);
    void WriteConfig();
    std::vector<char>* GetFileFromMemory(std::string& filename);
    std::vector<char>* GetFileFromMemory(char* filename);
    std::string& ConvertFileToAscii(char* filename);
    std::string& ConvertFileToAscii(std::string& filename);
    long GetFileFromMemorySize(char* filename);
    long GetFileFromMemorySize(std::string& filename);
  protected:
  private:
    Config();
    virtual ~Config();
    Config(const Config&);
    Config& operator= (const Config&);

    void LoadGameArchive();
    void FillTempVector(char* array, int size);
    bool CompareBlocks(void* block1, void* block2);

    std::vector<char> _tmpVector;

    std::string _asciiFile;

    std::map<std::string, int> _config;
    std::map<std::string, std::vector<char>> _gameData;
    std::map<std::string, long> _gameDataSizes;
};

struct TarHeader
{
  char Filename[100];
  char Filemode[8];
  char OwnerUserId[8];
  char GroupUserId[8];
  char FileSizeBytesOctalBase[12];
  char LastModifiedUnixTimeOctalBase[12];
  char Checksum[8];
  char FileType;
  char NameOfLinkedFile[100];
  char Padding[255];  // filled with NULL to make 512 size block
};

#endif // CONFIG_H
