#include <Arduino.h>

#define PRESENCE_THRESHOLD 15.0
#define presenceAnalogPin A7

void setupPresenceDetector(){
  pinMode(presenceAnalogPin, INPUT);
}

bool presenceDetected(){
  return analogRead(presenceAnalogPin) < PRESENCE_THRESHOLD;
}
