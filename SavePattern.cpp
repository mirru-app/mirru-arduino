#include "SavePattern.h"

SavePattern::SavePattern() 
{
}

void SavePattern::setupSavePattern() {
  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
    Serial.println("SPIFFS Mount Failed");
    return;
  }
}

String SavePattern::readFileString(fs::FS &fs, const char * path) {
    Serial.printf("Reading file: %s\r\n", path);
    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
    }

    Serial.println("- read from file:");
    while(file.available()){
      theSavedPattern = file.readStringUntil('\n');
    }
    return theSavedPattern;
}

void SavePattern::writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- frite failed");
    }
}

void SavePattern::setLastPattern(String lastPatternString) {
  lastPattern = lastPatternString;
}
