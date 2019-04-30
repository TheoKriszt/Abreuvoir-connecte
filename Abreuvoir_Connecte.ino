#include "presence.h"
#include "RFID.h"
#include "flow.h"
#include "RTC.h"
#include "pressure.h"
#include "SD.h"
#include "OLED.h"

#define VALVE_OUT 4
#define PUMP 5
#define VALVE_IN 6

enum Etat
  {
    REPOS,
    SUIVI,
    PURGE
  };
typedef enum Etat Etat_t ;

Etat_t etat = REPOS;
String tag = "";

float oldOutputVolume = 0; // suivi du volume d'eau sortant

void setup() {

  pinMode(VALVE_OUT, OUTPUT);
  pinMode(VALVE_IN, OUTPUT);
  pinMode(PUMP, OUTPUT);
  
  setupPresenceSensor();
  setupFlow();
  setupRFIDReader();
  setupRTC();
  setupPressure();
  
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4 , OUTPUT);
  Serial.println("Datetime : " + getDatetime());

  setupSD();
  setupOLED();
}


void loop() {
  // put your main code here, to run repeatedly:
//  Serial.println(distanceMm());
//  digitalWrite(LED_BUILTIN, presenceDetected());
//  digitalWrite(4, presenceDetected());
  
//  String id = readRFID();
//  if(id != ""){
//    Serial.println(id);  
//  }
  
//  Serial.println(getPulse(0) + ";" + getPulse(1));
//  Serial.println(getPressureKpa());

//  showPurgeScreen(0.425, 0.3333);

  switch (etat){
    case SUIVI:
      goMonitor();
    break;
    case PURGE:
      goPurge();
    break;
    case REPOS:
    default:
    goIdle();
  }

}

void goMonitor(){

  bool gone = false;
  uint8_t tries = 10;
  while(!presenceDetected()){
    delay(100);
    if(!tries--){
      gone = true;
      break;
    }
  }
  
  if(gone){
    etat = PURGE;
    oldOutputVolume = 0;
    clearScreen();
    return;
  }

  showRecordingScreen(tag, getVolumeIn());
  Serial.println(getVolumeIn());
}


void goPurge(){
  if(oldOutputVolume == getVolumeOut()){
    etat = REPOS;
    resetFlow();
    saveRow(tag, getVolumeIn() - getVolumeOut());
    tag = "";
    clearScreen();
  }
}

void goIdle(){
  if(presenceDetected()){
    etat = SUIVI;
    clearScreen();
    return;
  }
  showIdleSreen(getPressureKpa());
}

void saveRow(String tag, float volume){
  if(tag == ""){
    tag = F("INCONNU");
  }
  Serial.print(F("Saving "));
  Serial.print(getDatetime() + ";");
  Serial.print(tag + ";");
  Serial.println(volume);
  logLine(  getDatetime() + ";" + tag + ";" + String(volume, 4)  );
}
