#include <Arduino.h>
#define FLOW_IN 2
#define FLOW_OUT 3

volatile int pulseCountIn = 0;
volatile int pulseCountOut = 0;

float volumeIn = 0; // Volume du réservoir à l'auge
float volumeOut = 0; // Volume Retiré de l'auge après utilisation

const float PROGMEM calibrationFactor = 5.5;

void pulseFlowIn(){
  pulseCountIn++;
}

void pulseFlowOut(){
  pulseCountOut++;
}

void startFlow(){
  attachInterrupt(digitalPinToInterrupt(FLOW_IN), pulseFlowIn, FALLING);
  attachInterrupt(digitalPinToInterrupt(FLOW_OUT), pulseFlowOut, FALLING);
}

void stopFlow(){
  detachInterrupt(FLOW_IN);
  detachInterrupt(FLOW_OUT);
}

void resetFlow(){
  startFlow();
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
  stopFlow();
  float v = pulseCountIn * calibrationFactor;
  startFlow();
  return v;
}

float getVolumeOut(){
  stopFlow();
  float v = pulseCountOut * calibrationFactor;
  startFlow();
  return v;
}
