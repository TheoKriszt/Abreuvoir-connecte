#include <Arduino.h>
#define FLOW_IN 2
#define FLOW_OUT 3

volatile int pulseCountIn = 0;
volatile int pulseCountOut = 0;

float volumeIn = 0; // Volume du réservoir à l'auge
float volumeOut = 0; // Volume Retiré de l'auge après utilisation

//const float PROGMEM mililiterPerPulse = 5.5;

//const float PROGMEM mililiterPerPulse = 3.8;

const float PROGMEM calibrationFactorIn = 2.642 * (483.49/404.23); // OK sous pression 0.7 Bar
const float PROGMEM calibrationFactorOut = 2.642; // OK descente gravitaire

void pulseFlowIn(){
  pulseCountIn++;
}

void pulseFlowOut(){
  pulseCountOut++;
}

void startMonitoring(){
  attachInterrupt(digitalPinToInterrupt(FLOW_IN), pulseFlowIn, FALLING);
  attachInterrupt(digitalPinToInterrupt(FLOW_OUT), pulseFlowOut, FALLING);
}

void stopMonitoring(){
  detachInterrupt(FLOW_IN);
  detachInterrupt(FLOW_OUT);
}

void resetFlow(){
  startMonitoring();
  volumeIn = 0;
  volumeOut = 0;
  pulseCountIn = 0;
  pulseCountOut = 0;
}

void setupFlow(){
  pinMode(FLOW_IN, INPUT_PULLUP);
  pinMode(FLOW_OUT, INPUT_PULLUP);
  
  resetFlow();
}

int getPulse(uint8_t port){
  if(port == 0){
    return pulseCountIn;
  }
  return pulseCountOut;
}

float getVolumeIn(){
  stopMonitoring();
  float v = pulseCountIn * calibrationFactorIn;
  startMonitoring();
  return v;
}

int getPulseCountIn(){
  return pulseCountIn;
}

float getVolumeOut(){
  stopMonitoring();
  float v = pulseCountOut * calibrationFactorOut;
  startMonitoring();
  return v;
}
