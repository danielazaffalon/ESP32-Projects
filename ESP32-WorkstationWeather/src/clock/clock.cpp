// https://www.luisllamas.es/reloj-y-calendario-en-arduino-con-los-rtc-ds1307-y-ds3231/
// https://github.com/gmag11/NtpClient
// https://www.programiz.com/cpp-programming/constructor-overloading

/*
Se incluye y modifica la libreria NtpClientLib.cpp en la linea 93 y 93
// #define NETWORK_TYPE NETWORK_ESP32 // Se comenta porque la librería detecta la board usada
(ESp32) he incluye por defecto las librerías de WiFi y deseamos que incluya las de ETHERNET que
estan ligadas a las boards de arduino en esta librería.
De tal manera que agregamos la siguiente línea:
#define NETWORK_TYPE NETWORK_W5100 
Que implica que al detectar la board de esp32, defina el tipo W5100 que es el tipo del shield
Usando esto con los pines de weidmueller hace la conexión correcta.
*/

#include "clock.h"

WMClock wmClock;

enum WeekDays{
    SUNDAY,
    MONDAY,
    TUESDAY,
    WENESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY
};

// NTP
boolean syncEventTriggered = false; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent; // Last triggered event

#define NTP_TIMEOUT 1500

int8_t timeZone = 1;
int8_t minutesTimeZone = 0;
const PROGMEM char *ntpServer = "pool.ntp.org";

/*
* Class Constructor
*/
WMClock::WMClock(){
    WMClock::_startDay = 6; //Horario trabajo desde 6 am
    WMClock::_endDay = 22; //Horario trabajo hasta las 10 pm
}

bool WMClock::startRTC(){
    if (!WMClock::rtc.begin()) {
        DEBUG_PRINT Serial.println("No RTC Module");
        return false;
    }
    else {
        this->beginNTC();
        DateTime date(NTP.getLastNTPSync());
        // DateTime date(2023, 2, 24, 23, 58, 0); // BOORAR. PARA PRUEBAS
        rtc.adjust(date); //Asignar al RTC el valor del NTC
        return true; 
    }
}

void WMClock::rtcStrDateTime(char * dateTimeStr){
    DateTime date = rtc.now();
    sprintf(dateTimeStr, "%4u-%02u-%02uT%02u:%02u:%02u\0",date.year(),date.month(),date.day(),date.hour(),date.minute(),date.second());
    return;
}

String WMClock::rtcStrDate(){
    char dateTimeStr[12];
    DateTime date = rtc.now();
    sprintf(dateTimeStr,"%02u/%02u\0",date.day(),date.month());
    return dateTimeStr;
}

String WMClock::rtcStrTime(){
    char dateTimeStr[12];
    DateTime date = rtc.now();
    sprintf(dateTimeStr,"%02u:%02u\0",date.hour(),date.minute());
    return dateTimeStr;
}

void WMClock::createCallbackNTC(){
    NTP.onNTPSyncEvent ([](NTPSyncEvent_t event) {
        ntpEvent = event;
        syncEventTriggered = true;
    });
}

void WMClock::beginNTC(){
    NTP.setInterval (63);
    NTP.setNTPTimeout (NTP_TIMEOUT);
    NTP.begin(ntpServer, timeZone, true, minutesTimeZone);
    while(!syncEventTriggered){} //Sincronizarlo con mi programa porque es
    if (syncEventTriggered){
        if(this->_processSyncEvent(ntpEvent)) syncEventTriggered = false;
        else this->beginNTC();
    }
}

String WMClock::ntcStrDate(){
    this->beginNTC();
    char dateTimeStr[22];
    sprintf(dateTimeStr, "%s-%s\t",NTP.getDateStr(NTP.getLastNTPSync()),NTP.getTimeStr(NTP.getLastNTPSync()));
    return dateTimeStr;
}

bool WMClock::dayOrNight(){
    DateTime date = rtc.now();
    if (date.dayOfTheWeek() == SUNDAY || date.dayOfTheWeek() == SATURDAY) return false;
    else if(date.hour() >= this->_endDay || date.hour() <= this->_startDay) return false;
    else return true;
}

bool WMClock::_processSyncEvent(NTPSyncEvent_t ntpEvent) {
    if (ntpEvent < 0) {
        DEBUG_PRINT Serial.printf ("Time Sync error %d: ", ntpEvent);
        if(ntpEvent == noResponse)
            DEBUG_PRINT Serial.println("NTP server not reachable");
        else if(ntpEvent == invalidAddress)
            DEBUG_PRINT Serial.println("Invalid NTP server address");
        else Serial.println();
        return false;
    } 
    else if (!ntpEvent) {
        // DEBUG_PRINT Serial.print ("Got NTP time: ");
        // DEBUG_PRINT Serial.printf("%s-%s\t",NTP.getDateStr(NTP.getLastNTPSync()),NTP.getTimeStr(NTP.getLastNTPSync()));
        return true;
    } 
    else {
        DEBUG_PRINT Serial.println ("NTP request Sent");
        return false;
    }
}