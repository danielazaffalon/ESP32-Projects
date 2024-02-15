#ifndef CLOCK_h
#define CLOCK_h

#include <Arduino.h>
#include <RTClib.h>
#include <time.h>
#include <TimeLib.h>
#include <NtpClientLib.h>

#include "../debug.h"

class WMClock {
    RTC_DS3231 rtc;
    public:
        WMClock(); //Class Constructor
        bool startRTC();
        // String rtcStrDateTime();
        void rtcStrDateTime(char * dateTimeStr);
        String rtcStrDate();
        String rtcStrTime();
        void createCallbackNTC();
        void beginNTC();
        String ntcStrDate();
        bool dayOrNight();
    private:
        uint8_t _startDay; //= 6; //Horario trabajo desde 6 am
        uint8_t _endDay; //= 22; //Horario trabajo hasta las 10 pm
        bool _processSyncEvent(NTPSyncEvent_t ntpEvent);
};

extern WMClock wmClock;

#endif