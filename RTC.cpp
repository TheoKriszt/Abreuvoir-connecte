#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t

tmElements_t tm;

const char * const monthName[12] PROGMEM = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

bool getTime(const char *str){
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str){

  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}

void setupRTC(){
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if (timeStatus() == timeSet) {
    // OK
  } else {
    if (getDate(__DATE__) && getTime(__TIME__)) {
      RTC.write(tm);
    }
  }
}

String padInt(uint8_t t){
  return (t < 10 ? "0" : "") + String(t);
}

String getDatetime(){
  String ret = "";
  ret += padInt(day());
  ret += "/";
  ret += padInt(month());
  ret += "/";
  ret += String(year()).substring(2);
  ret += " ";
  ret += padInt(hour());
  ret += ":";
  ret += padInt(minute());
  ret += ":";
  ret += padInt(second());

  return ret;
    
}
