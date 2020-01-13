#include <Arduino.h>

#define PRESSURE_ANALOG A3
#define SAMPLES 10
#define VREF 4.944
#define OFFSET 0.495 

void setupPressure(){
 pinMode(PRESSURE_ANALOG, INPUT); 
}

float getPressureKPa(){
  
  unsigned int voltage = 0;
  for(int i = 0; i < SAMPLES; i++){
    voltage += analogRead(PRESSURE_ANALOG)  ;
  }
  
  float pressure =  (voltage / SAMPLES + 0.5) * VREF / 1024.0  ;
  pressure = (pressure - OFFSET) * 400;
  //pressure = (pressure - offset) * 400;
  
  
  if(pressure < 0 && pressure > -1) {
    pressure = 0;
  }
  
  return pressure;
}

float getPressureBar(){
  return getPressureKPa() / 100;
}

float getPressureMeters(){
  return (getPressureKPa() / 9.81);
}
