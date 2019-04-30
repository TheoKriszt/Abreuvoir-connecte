#include <Arduino.h>

#define PRESSURE_ANALOG A3
static const uint8_t PROGMEM offset = 107;

void setupPressure(){
 pinMode(PRESSURE_ANALOG, INPUT); 
}

float getPressureKpa(){
  
  float voltage = analogRead(PRESSURE_ANALOG) - offset;
  voltage *= 5;
  voltage /= 1024;

  return voltage * 400;
}
