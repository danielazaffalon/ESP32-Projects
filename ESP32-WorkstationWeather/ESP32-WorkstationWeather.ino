/**
 * @file ESP32-Almacen.ino
 *
 * @brief This sketch connect to Wi-Fi, read from NTC the unix date 
 * and update the RTC clock each reboot. In the cycle, the sketch 
 * read from modbus rtu the temperature and humidity from XY-MD02 Sensor
 * print in LCD Screen and save it in csv Logfile in uSD.
 * The sketch detect the uSD and when is not, print error message in the
 * screen.
 *
 * @author Daniela Zaffalon.
 * 
 * @see [https://randomnerdtutorials.com/esp32-microsd-card-arduino/]
 */

#include "src/display/lcdTaskCode.h"
#include "src/internet/internet.h"
#include "src/modbus/modbus.h"
#include "src/sdCard/sdCard.h"
#include "src/clock/clock.h"
#include "src/debug.h"

int processSwitchState;
unsigned long startTime;
unsigned long dataLogStartTime;
int appendCounter = 0;
bool writenErrorFlag = false;
int refreshTime = 10000; //Cada 10 segundos (para que detecte la sd fácil)
int dataLogrefreshTime = 60000; // Cada Hora 3600000
int appendRetries = 3;
bool isday = true;
bool firstTime = true;
char dateTimeStr[50];

void setup(){
    Serial.begin(115200);
    createTask();
    lcdSwitchState = 0;
    processSwitchState = 0;
	delay(2000);// Optional delay. Some board do need more time after init to be ready, see Readme
    wmClock.createCallbackNTC();
}

void loop(){
    unsigned long actualTime = millis();
    if(Ethernet.linkStatus() != LinkON) internetConnection();
    switch (processSwitchState){
        case 0: {//Clocks begin
            if(Ethernet.linkStatus() == LinkON) {
                wmClock.startRTC();   
                processSwitchState = 1;
                lcdSwitchState = 1;
            }
            break;
        }
        case 1: {//Modbus Initialization
            if(modbusInitialization()) {
                startTime = dataLogStartTime = millis();
                processSwitchState = 2;
            }
            break;
        }
        case 2: {//Waiting to refresh info
            if(firstTime){ // The first time just wait 30 s to see the IP and refresh the data.
                delay(10000); // firstTime = false;
                processSwitchState = 3;
            }
            else if((actualTime-startTime >= refreshTime)) {
                processSwitchState = 3;
            }
            break;
        }
        case 3: {//Data acquisition
            processSwitchState = 4;
            globalDate = wmClock.rtcStrDate(); //For LCD Print format
            globalTime = wmClock.rtcStrTime(); //For LCD Print format
            globalTemperature = modbusTemperature();
            globalHumidity = modbusHumidity();
            wmClock.rtcStrDateTime(dateTimeStr); //For uSD TimeStamp format
            DEBUG_PRINT Serial.printf("%s %s\t%.2f [°C] %.2f [%HR]\r\n",globalDate,globalTime,globalTemperature,globalHumidity);
            startTime = millis();
            break;
        }
        case 4: {// Day or Night backligh
            bool dayToggle;
            processSwitchState = 5;
            dayToggle =  wmClock.dayOrNight(); //If true is working time, false is night
            if(backlight != dayToggle) backlight = dayToggle;
            DEBUG_PRINT Serial.printf("It is %s\r\n",backlight ? "Day":"Night");
            break;
        }
        case 5: {//Datalogger
            if((actualTime-dataLogStartTime >= dataLogrefreshTime) || firstTime) {
                if(firstTime) firstTime = false;
                bool sdStarted;
                sdStarted = startSD();
                // if(!sdStarted && (lcdSwitchState != 3)) lcdSwitchState = 3; //Print uSD not found
                // else 
                if(sdStarted && (lcdSwitchState != 2)){ //Creo que sobre lcdSwithcState != 2 porque siempre viene siendo 0
                    if(!fileExist(SD, "/logFile.csv") || writenErrorFlag){
                        if(!writeFile(SD, "/logFile.csv", "Timestamp;Temperature [°C];Humidity [%HR]\r\n\0")) writenErrorFlag = true; //El nombre no puede ser mayor de 12 caracteres incluyendo la extensión
                        else writenErrorFlag = false;
                    }
                    else {
                        DEBUG_PRINT Serial.printf("dateTimeStr: %s\r\n",dateTimeStr);
                        char sdData[50];
                        sprintf(sdData, "%s;%.2f;%.2f\r\n\0",dateTimeStr,globalTemperature,globalHumidity);
                        if(!appendFile(SD, "/logFile.csv", sdData)) appendCounter++; //Count when it cant appened
                        else {
                            appendCounter = 0;
                            dataLogStartTime = millis();
                        }
                    }
                    if(sdStarted && appendCounter >= appendRetries) lcdSwitchState = 4; //Print uSD is Full
                    else if(sdStarted && writenErrorFlag) {
                        lcdSwitchState = 5; //Print Error SD
                        // processSwitchState = 6; //Borrar
                        // break; //Borrar
                    }
                    else lcdSwitchState = 2; //Print Temp y hum.
                }
            }
            processSwitchState = 6;
            break;
        }
        case 6: {// SD State comprobation
            processSwitchState = 2;
            bool sdStarted;
            sdStarted = startSD();
            if(!sdStarted && (lcdSwitchState != 3)) lcdSwitchState = 3; //Print uSD not found
            else lcdSwitchState = 2;
            break;
        }
        default:{
            break;
        }
    }
}