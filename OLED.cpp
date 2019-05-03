#include "OLED.h"
#include <Arduino.h>
#include <U8x8lib.h>
#include <Wire.h>


#define OLED_ADDRESS 0x3C;
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

void setupOLED(){
  u8x8.begin();
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);    
  u8x8.clear();
}

void showIdleSreen(const float& pressure, const String& datetime){
  
  u8x8.inverse();
  u8x8.setCursor(0,0);
  u8x8.print(F("   En attente   "));
  u8x8.noInverse();
  u8x8.setCursor(0,2);
  u8x8.println(datetime);
  u8x8.setCursor(0,4);
  u8x8.println(F("Pression : "));
  u8x8.print(String(pressure, 3));
  u8x8.println(F(" KPa"));
  
}

void showRecordingScreen(const String& tag, const float& litersIn){
  
  u8x8.inverse();
  u8x8.setCursor(0,0);
  u8x8.print(F("      Suivi       "));
  u8x8.noInverse();
  u8x8.setCursor(0,2);
  
  u8x8.print(F("Tag : "));
  u8x8.println(tag == "" ? F("INCONNU") : tag);
  u8x8.print(F("IN : "));
  u8x8.print(litersIn);
  u8x8.println(F("mL"));
}

void showPurgeScreen(const float& litersIn, const float& litersOut){
  
  u8x8.inverse();
  u8x8.setCursor(0,0);
  u8x8.print(F(" Enregistrement \n     & purge    "));
  u8x8.noInverse();
  u8x8.setCursor(0,3);

  u8x8.print(F(" IN : "));
  u8x8.print(String(litersIn, 3));
  u8x8.println(F(" mL"));
  u8x8.print(F("OUT : "));
  u8x8.print(String(litersOut, 3));
  u8x8.println(F(" mL"));
  
}

void showNoSDScreen(){
  u8x8.setCursor(0,0);
  u8x8.print(F("ERREUR CARTE SD"));
  
}

void clearScreen(){
  u8x8.clear();
}
