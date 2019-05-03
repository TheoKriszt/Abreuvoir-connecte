#include "SdFat.h"
#define CS 10 // chip select
#define SD_FREQ 50

SdFat SD;
File logFile;
#define FILENAME "abreuvoir.csv"
#define HEADER "Date;ID;Qty"
//File presenceTest;
//boolean SD_presence = false;



bool logLine(const String& line){
  logFile = SD.open(FILENAME, FILE_WRITE);
  //Serial.print("Saving");
  //Serial.println(line);
  if (logFile) {                     // if the file is available, write to it:
    logFile.println(line);
    logFile.close();  
    return true;
  } else {
    //SD.errorPrint();
    //Serial.println(F("Fichier inaccessible"));
    return false;
  }
}

bool setupSD(){

  if (!SD.begin(CS, SD_SCK_MHZ(SD_FREQ))) {
    SD.errorPrint();
    //Serial.println(F("SD inaccessible"));
    return false;
  }else {
    //Serial.println(F("SD OK"));
    if(!SD.exists(FILENAME)){
      logLine(HEADER); 
    }
    return true;
  }
}
