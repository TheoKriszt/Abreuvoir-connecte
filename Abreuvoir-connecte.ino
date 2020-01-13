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
float oldInputVolume = 0; // suivi du volume d'eau entrant
unsigned long purgeStart = millis(); // temps du démarrage de la purge
unsigned long drinkStart = millis(); // temps du démarrage de la session d'abreuvage
unsigned long lastDrink = millis(); // temps du dernier pulse de liquide entrant lu
#define INACTIVITY_TIMEOUT 30000 // Temps passé sans s'abreuver avant de  passer à phase de purge



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

int getDrinkDuration() {
  return (int)((lastDrink - drinkStart) / 1000);
}


void goMonitor() {
  float currentInputVolume = getVolumeIn();
  
  if(oldInputVolume < currentInputVolume) { // Si du liquide vient de passer en entrée, noter son temps de passage
    lastDrink = millis();
  }

  if(millis() - lastDrink >= INACTIVITY_TIMEOUT) {
    etat = PURGE;
    digitalWrite(PUMP, HIGH);
    digitalWrite(VALVE_OUT, HIGH);
    oldOutputVolume = 0; // préparer le volume sortant
    purgeStart = millis();
    clearScreen();
    return;
  }

  showRecordingScreen(getVolumeIn(), getVolumeOut(), getDrinkDuration());

  oldInputVolume = currentInputVolume;
}


void goPurge() {

  showPurgeScreen(getVolumeIn(), getVolumeOut(), getDrinkDuration());

  if(oldOutputVolume != getVolumeOut() ) {
      purgeStart = millis();
  digitalWrite(PUMP, HIGH);
  digitalWrite(VALVE_OUT, HIGH);
      oldOutputVolume = getVolumeOut();
  }
  
  if (millis() > purgeStart + PURGE_TIME){
    
    etat = REPOS;
    
    saveRow(tag, getVolumeIn(), getVolumeOut(), getDrinkDuration());
    resetFlow();
    tag = "";
    delay(100);
    clearScreen();
    digitalWrite(PUMP, LOW);
    digitalWrite(VALVE_OUT, LOW); 
  }
    
}

void goIdle() {
  if ( getPulseCountIn() > 0 ) {
    etat = SUIVI;
    drinkStart = millis();
    lastDrink = drinkStart;
    clearScreen();
    return;
  }

  String dt = getDatetime();
  dt = dt.substring(0, 5) + " " + dt.substring(9);
  showIdleSreen( getPressureKPa(), dt );
}

void saveRow(const String& tag, const float& volumeIn, const float& volumeOut, const float& duration) {
  char pv = ';';
  logLine(getDatetime(), false);
  logLine(pv + (tag == "" ? F("INCONNU") : tag) + pv + String(volumeIn, 4) + pv + String(volumeOut, 4) + pv + String(duration), true);
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
