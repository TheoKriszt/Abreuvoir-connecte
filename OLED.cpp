#include "OLED.h"
#include <Arduino.h>
#include <U8x8lib.h>
#include <Wire.h>

#define RETURN_IF_TOO_SOON if(millis() >= lastRefresh + 1000) {\
    lastRefresh = millis();\
  } else {return;}



#define OLED_ADDRESS 0x3C;
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

unsigned long lastRefresh;

void setupOLED(){
  lastRefresh = millis();
  u8x8.begin();
  //u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);   // 30058 octets (97%)
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_r); // 29026 octets (92%) libres 
  u8x8.clear();
}

void showIdleSreen(const float& pressure, const String& datetime){

  RETURN_IF_TOO_SOON
  
  u8x8.inverse();
  u8x8.setCursor(0,0);
  u8x8.print(F("   En attente   "));
  u8x8.noInverse();
  u8x8.setCursor(0,2);
  u8x8.println(datetime);
  u8x8.setCursor(0,4);
  u8x8.println(F("Pression : "));
  if(pressure >= 0) {
    u8x8.print(String(pressure, 3));
    u8x8.println(F(" KPa"));
  } else {
    u8x8.println(F("    ERREUR"));
  }
  
  
}

void showRecordingScreen(const float& litersIn, const float& litersOut, const int duration){

  RETURN_IF_TOO_SOON
  
  u8x8.inverse();
  u8x8.setCursor(0,0);
  u8x8.print(F("      Suivi       "));
  u8x8.noInverse();
  u8x8.setCursor(0,2);


  u8x8.print(F("IN : "));
  u8x8.print(litersIn);
  u8x8.println(F(" mL"));
  
  u8x8.print(F("OUT : "));
  u8x8.print(litersOut);
  u8x8.println(F(" mL"));

  u8x8.print(F("Duree : "));
  u8x8.print(duration);
  u8x8.println(F(" s"));
}

void showPulsesIn(const int& pulsesIn){

  RETURN_IF_TOO_SOON
  
  u8x8.setCursor(0,4);
  u8x8.println(F("pulses"));
  u8x8.print(pulsesIn);
  
}

void showPurgeScreen(const float& litersIn, const float& litersOut, const int duration){

  RETURN_IF_TOO_SOON
  
  u8x8.inverse();
  u8x8.setCursor(0,0);
  u8x8.print(F(" Enregistrement \n     & purge    "));
  u8x8.noInverse();
  u8x8.setCursor(0,3);

  u8x8.print(F(" IN : "));
  u8x8.print(String(litersIn, 2));
  u8x8.println(F(" mL"));
  u8x8.print(F("OUT : "));
  u8x8.print(String(litersOut, 2));
  u8x8.println(F(" mL"));
  
}

void showNoSDScreen(){
  u8x8.setCursor(1,3);
  u8x8.print(F("ERREUR CARTE SD"));
  
}

void clearScreen(){
  u8x8.clear();
}
