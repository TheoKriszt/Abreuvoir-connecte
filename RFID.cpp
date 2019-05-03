#include <AltSoftSerial.h>
#include <Arduino.h>
AltSoftSerial altser;

String line = "";

void setupRFIDReader() {
  altser.begin(9600);
}

String readRFID() {
  char c;
  //bool complete = false;

  while(altser.available()){
    c = altser.read();
    if(c && c != '\r' ){
      line.concat(c);
    }else if (c == '\r'){
      //complete = true;
      
      String ret = line;
      line = "";
      return ret;
    }
  }
  /*
  if (altser.available()) {
    c = altser.read();

    if (c && c != '\r') {
      line.concat(c);
      Serial.println(c);
    } else if (c == '\r') {
      String ret = line;
      line = "";
      return ret;
    }
  }/**/
  
  return "";
}
