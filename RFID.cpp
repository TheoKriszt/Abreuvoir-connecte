#include <AltSoftSerial.h>
#include <Arduino.h>
AltSoftSerial altser;

String line = "";

void setupRFIDReader() {
  altser.begin(9600);
}

String readRFID() {
  char c;
  if (altser.available()) {
    c = altser.read();

    if (c && c != '\r') {
      line.concat(c);
    } else if (c == '\r') {
      String ret = line;
      line = "";
      return ret;
    }
  }

  return "";
}
