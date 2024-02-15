#include <Wire.h>
#include <DFRobot_RGBLCD1602.h>
#include "lcdTaskCode.h"

DFRobot_RGBLCD1602 lcd(16,2);
TaskHandle_t lcdTask;

int lcdSwitchState;
double globalTemperature;
double globalHumidity;
bool lastHvacState;
bool alertMaxTimeDeviceON;
IPAddress wifiIP;


void lcdTaskcode( void * parameter){
    vTaskDelay(100);
    // lcd.setBacklight(false);
    lcd.init();
    lcd.setRGB(255,255,255);
    lcd.setCursor(0,0);
    // lcdSwitchState = 0;
    for(;;){
        switch (lcdSwitchState) {
            case 0:
            // lcd.setCursor(0,0);
            // lcd.print("LCD Waiting...");            
            break;
            case 1:
            lcd.setBacklight(true);
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("Initializing...");
            vTaskDelay(2000);
            lcdSwitchState = 0;
            break;
            case 2:
            lcd.setBacklight(true);
            lcd.clear();
            lcd.print("Wifi Connected with IP: ");
            lcd.print(wifiIP);
            for (int positionCounter = 0; positionCounter < 40; positionCounter++) {
                lcd.scrollDisplayLeft();
                delay(300);
            }
            break;
            case 3:
            lcd.setBacklight(true);
            lcd.setCursor(0,0);
            lcd.print("WiFi IP: ");
            lcd.print(wifiIP);
            lcd.setCursor(0,1);
            lcd.print("Connecting to Losant...");
            for (int positionCounter = 0; positionCounter < 40; positionCounter++) {
                lcd.scrollDisplayLeft();
                delay(300);
            }
            break;
            case 4:
            if(lastHvacState) lcd.setBacklight(true);
            else lcd.setBacklight(false);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(globalTemperature,2);
            lcd.print((char)223);
            lcd.print("C ");
            lcd.setCursor(12,0);
            lcd.print("ROOM");
            lcd.setCursor(0,1);
            lcd.print(globalHumidity,2);
            lcd.print("%HR");
            lcd.setCursor(13,1);
            lcd.print(lastHvacState ? "ON" : "OFF");
            lcdSwitchState = 0;
            break;
            // case 5:
            // lcd.setBacklight(true);
            // lcdSwitchState = 4;
            // break;
            default:
            lcd.setCursor(0,0);
            lcd.print("Default");
            break;
        }
    delay(10);
    }
}

void createTask(){
    xTaskCreatePinnedToCore(
    lcdTaskcode, /* Function to implement the task */
    "lcdTask", /* Name of the task */
    10000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    1,  /* Priority of the task */
    &lcdTask,  /* Task handle. */
    0); /* Core where the task should run */
}