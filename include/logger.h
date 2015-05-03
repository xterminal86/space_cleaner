#ifndef LOGGER_H
#define LOGGER_H

#include "globals.h"

// We drop all console output to the file
class Logger
{
  public:
    static Logger& Get()
    {
      static Logger instance;
      return instance;
    }
    void Init(std::string logfilename, bool prohibitPrint = false);
    void LogPrint(const char* formatString, ...);
  protected:
  private:
    Logger();
    ~Logger();
    Logger(const Logger&);
    Logger& operator= (const Logger&);

    bool _initialized;
    bool _prohibited;
    FILE* _log;
};

#endif // LOGGER_H
