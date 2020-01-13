#ifndef H__OLED
#define H__OLED
  #include <Arduino.h>
  
  void setupOLED();
  void showIdleSreen(const float& pressure, const String& datetime);
  void showRecordingScreen(const float& litersIn, const float& litersOut, const int duration);
  void showPulsesIn(const int& pulsesIn);
  void showPurgeScreen(const float& litersIn, const float& litersOut,  const int duration);
  void showNoSDScreen();
  void clearScreen();

  
#endif
