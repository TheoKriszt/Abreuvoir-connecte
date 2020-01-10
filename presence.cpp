#include <Arduino.h>

// Use Time of flight sensor instead of ultrasonic HS-SR04
//#define USE_TOF

#define PRESENCE_THRESHOLD 30.0 // en cm

#define TRIGGER A1
#define ECHO A0

#ifdef USE_TOF
  #include <Wire.h>
  #include <VL53L0X.h>
  VL53L0X sensor;
#else
const float PROGMEM SOUND_SPEED = 340.0 / 1000;
const unsigned long PROGMEM MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s
#endif

void setupPresenceSensor() {

#ifdef USE_TOF
  Wire.begin();
  sensor.setTimeout(500);
  sensor.setMeasurementTimingBudget(200000);
  if (!sensor.init())
  {
    while (1) {}
  }
#else
  pinMode(TRIGGER, OUTPUT);
  digitalWrite(TRIGGER, LOW);
  pinMode(ECHO, INPUT);
#endif
}

float distanceMm() {

#ifdef USE_TOF
  float range = sensor.readRangeSingleMillimeters();
  if (sensor.timeoutOccurred()) {
    return 999;
  } else {
    return range;
  }
#else
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);
  unsigned long measure = pulseIn(ECHO, HIGH, MEASURE_TIMEOUT);
  return measure / 2.0 * SOUND_SPEED;
#endif

}

bool presenceDetected() {
  uint8_t tries = 10;
  while (tries--) {
    if (distanceMm() / 10 > PRESENCE_THRESHOLD) {
      return false;
    }
    delay(100);
  }
  return true;
}
