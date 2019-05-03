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

#define PURGE_TIME 2000

enum Etat {
  REPOS,
  SUIVI,
  PURGE
};
typedef enum Etat Etat_t ;

Etat_t etat = REPOS;

String tag = "";

float oldOutputVolume = 0; // suivi du volume d'eau sortant
unsigned long purgeStart = millis();

void setup() {
  Serial.begin(9600);

  pinMode(VALVE_OUT, OUTPUT);
  pinMode(VALVE_IN, OUTPUT);
  pinMode(PUMP, OUTPUT);

  setupPresenceSensor();
  setupFlow();
  setupRFIDReader();
  setupRTC();
  setupPressure();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4 , OUTPUT);
  

  bool sdOK = setupSD();
  setupOLED();
  
  if(!sdOK){
    clearScreen();
    showNoSDScreen();
    while(1);
  }

}


void loop() {
  switch (etat) {
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

void goMonitor() {

  if(tag == ""){
    tag = readRFID();
  }
  /*
  bool gone = false;
  uint8_t tries = 10;
  while (!presenceDetected()) {
    delay(100);
    if (!tries--) {
      gone = true;
      break;
    }
  }
  */
  if (!presenceDetected()) {
    etat = PURGE;
    digitalWrite(PUMP, HIGH);
    digitalWrite(VALVE_OUT, HIGH);
    oldOutputVolume = 0;
    purgeStart = millis();
    clearScreen();
    return;
  }

  showRecordingScreen(tag, getVolumeIn());
  //Serial.println(getVolumeIn());
}


void goPurge() {

  showPurgeScreen(getVolumeIn(), getVolumeOut());
  

  if(oldOutputVolume != getVolumeOut() ) {
      purgeStart = millis();
  digitalWrite(PUMP, HIGH);
  digitalWrite(VALVE_OUT, HIGH);
      oldOutputVolume = getVolumeOut();
  }
  
  if (millis() > purgeStart + PURGE_TIME){
    
    etat = REPOS;
    
    float volume = getVolumeIn() - getVolumeOut();
    saveRow(tag, volume);
    resetFlow();
    tag = "";
    delay(100);
    clearScreen();
    digitalWrite(PUMP, LOW);
    digitalWrite(VALVE_OUT, LOW);
    
  }
    
}

void goIdle() {
  if (presenceDetected()) {
    etat = SUIVI;
    clearScreen();
    return;
  }

  String dt = getDatetime();
  dt = dt.substring(0, 5) + " " + dt.substring(9);
  showIdleSreen( getPressureKPa(), dt );
}

void saveRow(const String& tag, float volume) {
  //Serial.println("Saving " + String(volume));
  
  logLine(  getDatetime() + ";" + (tag == "" ? F("INCONNU") : tag) + ";" + String(volume, 4)  );
}
