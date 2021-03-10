#ifndef SavePattern_h
#define SavePattern_h

#include "SPIFFS.h"
#define FORMAT_SPIFFS_IF_FAILED true

class SavePattern
{
  public:
    SavePattern();
    void setupSavePattern();
    String readFileString(fs::FS &fs, const char * path);
    void writeFile(fs::FS &fs, const char * path, const char * message);
    String lastPattern;
    void setLastPattern(String lastPatternString);
  private:
    String theSavedPattern;
};

#endif
