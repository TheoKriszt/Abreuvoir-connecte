#include <Arduino.h>

#define PRESSURE_ANALOG A3
#define SAMPLES 100
static const float PROGMEM offset = 0.495;

void setupPressure(){
 pinMode(PRESSURE_ANALOG, INPUT); 
}

float getPressureKPa(){
  
  float voltage = 0;
  for(int i = 0; i < SAMPLES; i++){
    voltage += analogRead(PRESSURE_ANALOG)  ;
    delay(5);
  }
  voltage /= SAMPLES;
 voltage *= (5.00 / 1024);
  //Serial.print(F("Voltage : "));
  //Serial.println(voltage, 4);
  return (voltage - offset) * 400;
}

float getPressureMeters(){
  return (getPressureKPa() / 9.81);
}
