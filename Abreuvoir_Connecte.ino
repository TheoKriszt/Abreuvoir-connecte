#include "presence.h"
#include "RFID.h"
#include "flow.h"


void setup() {
  setupPresenceDetector();
  setupFlow();
  setupRFIDReader();
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4
  , OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, presenceDetected());
  digitalWrite(4, presenceDetected());
  String id = readRFID();
  if(id != ""){
    Serial.println(id);  
  }
  Serial.println(getPulse(0) + "\t" + String(analogRead(A7)));
  delay(100);

}
