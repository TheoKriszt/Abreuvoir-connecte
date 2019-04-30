#ifndef H__OLED
#define H__OLED
  #include <Arduino.h>
  
  void setupOLED();
  void showIdleSreen(const float& pressure);
  void showRecordingScreen(const String& tag, const float& litersIn);
  void showPurgeScreen(const float& litersIn, const float& litersOut);
  void clearScreen();

  
#endif
