#include <Arduino.h>
#define FLOW1 2
#define FLOW2 3

volatile int pulseCount1 = 0;
volatile int pulseCount2 = 0;

float volume1 = 0;
float volume2 = 0;

const float calibrationFactor = 5.5;

void pulseFlow1(){
  pulseCount1++;
  digitalWrite(LED_BUILTIN, pulseCount1  % 2);
}

void pulseFlow2(){
  pulseCount2++;
}

void resetFlow(){
  attachInterrupt(digitalPinToInterrupt(FLOW1), pulseFlow1, FALLING);
  attachInterrupt(digitalPinToInterrupt(FLOW2), pulseFlow2, FALLING);
  volume1 = 0;
  volume2 = 0;
  pulseCount1 = 0;
  pulseCount2 = 0;
}

void stopFlow(){
  detachInterrupt(FLOW1);
  detachInterrupt(FLOW2);
}

void setupFlow(){
  pinMode(FLOW1, INPUT_PULLUP);
  pinMode(FLOW2, INPUT_PULLUP);
  
  resetFlow();
}

int getPulse(uint8_t port){
  if(port == 0){
    return pulseCount1;
  }
  return pulseCount2;
}
