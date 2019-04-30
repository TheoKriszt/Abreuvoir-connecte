#include <Arduino.h>

#define PRESENCE_THRESHOLD 30.0 // en cm

#define TRIGGER A1 
#define ECHO A0

const float PROGMEM SOUND_SPEED = 340.0 / 1000;
const unsigned long PROGMEM MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

void setupPresenceSensor(){
  pinMode(TRIGGER, OUTPUT);
  digitalWrite(TRIGGER, LOW);
  pinMode(ECHO, INPUT);
  
}

float distanceMm(){
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  unsigned long measure = pulseIn(ECHO, HIGH, MEASURE_TIMEOUT);
  
  return measure / 2.0 * SOUND_SPEED;
}

bool presenceDetected(){
  return distanceMm() / 10 <= PRESENCE_THRESHOLD;
}
