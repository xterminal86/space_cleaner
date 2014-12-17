#include "logger.h"

Logger::Logger()
{
  _log = nullptr;
  _initialized = false;
}

Logger::~Logger()
{
  if (_log != nullptr) fclose(_log);
}

void Logger::Init(std::string logfilename, bool prohibitPrint)
{
  _prohibited = prohibitPrint;
  if (_prohibited) return;

  _log = fopen(logfilename.c_str(), "w");
  if (_log == nullptr)
  {
     printf("(warning) Could not open file to write stdout!\n");
     return;
  }
  else
  {
    fprintf(_log, "BEGIN LOGGING STDOUT\n");
    fprintf(_log, "********************\n");

    printf ("Begin logging stdout...\n");

     _initialized = true;
  }

  if (!_initialized)
  {
    printf ("(warning) Logger already initialized!\n");
    return;
  }
}

void Logger::LogPrint(const char* formatString, ...)
{
  if (formatString == nullptr || _prohibited) return;

  va_list ap;
  char buf[512];

  va_start(ap, formatString);
  vsprintf(buf, formatString, ap);
  va_end(ap);

  printf ("%s", buf);
  fprintf(_log, "%s", buf);
}
