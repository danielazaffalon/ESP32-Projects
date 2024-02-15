#include "rtcFunction.h"
RTC_DS3231 rtc;

void rtcInitialization(){
  while (! rtc.begin()) Serial.print(" RTC ");
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // rtc.begin();
}
void rtcRead(){
  DateTime now = rtc.now();
  Serial.print(now.day());
  Serial.print('/');
  Serial.print(now.month());
  Serial.print('/');
  Serial.print(now.year());
  Serial.print(" ");
  Serial.print(now.hour());
  Serial.print(':');
  Serial.print(now.minute());
  Serial.print(':');
  Serial.print(now.second());
  Serial.println();
}