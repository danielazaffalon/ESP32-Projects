// https://forum.arduino.cc/t/print-degree-symbol-on-lcd/19073/7

#include "lcdTaskCode.h"

DFRobot_RGBLCD1602 lcd(16,2);
TaskHandle_t lcdTask;

int lcdSwitchState;
IPAddress IP;
double globalTemperature;
double globalHumidity;
String globalDate;
String globalTime;
bool backlight = true;

/**
 *  @brief set RGB
 *  @param r  red   range(0-255)
 *  @param g  green range(0-255)
 *  @param b  blue  range(0-255)
 */  

void lcdTaskcode( void * parameter){
    vTaskDelay(100);
    // lcd.setBacklight(false);
    lcd.init();
    lcd.setRGB(255,255,255);
    lcd.setCursor(0,0);
    for(;;){
        switch (lcdSwitchState) {
            case 0:
                // base state        
            break;
            case 1:
                lcdSwitchState = 0;
                lcd.setBacklight(backlight);
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Connected to");
                lcd.setCursor(0,1);
                lcd.print(IP);
            break;
            case 2:
                // if(lastHvacState) lcd.setBacklight(true);
                // else
                lcdSwitchState = 0;
                lcd.setBacklight(backlight);
                lcd.clear();

                lcd.setCursor(0,0);
                lcd.print(globalTemperature,2);
                lcd.print((char)223);
                lcd.print("C ");
                lcd.setCursor(11,0);
                lcd.print(globalDate);
                
                lcd.setCursor(0,1);
                lcd.print(globalHumidity,2);
                lcd.print("%HR");
                lcd.setCursor(11,1);
                lcd.print(globalTime);
            break;
            case 3:
                lcdSwitchState = 0;
                lcd.setBacklight(backlight);
                lcd.setRGB(255, 128, 0);
                lcd.clear();

                lcd.setCursor(0,0);
                lcd.print(globalTemperature,2);
                lcd.print((char)223);
                lcd.print("C ");
                lcd.setCursor(9,0);
                lcd.print("uSD NOT");
                
                lcd.setCursor(0,1);
                lcd.print(globalHumidity,2);
                lcd.print("%HR");
                lcd.setCursor(10,1);
                lcd.print("FOUND");
            break;
            case 4:
                lcdSwitchState = 0;
                lcd.setBacklight(backlight);
                lcd.setRGB(255, 128, 0);
                lcd.clear();

                lcd.setCursor(0,0);
                lcd.print(globalTemperature,2);
                lcd.print((char)223);
                lcd.print("C ");
                lcd.setCursor(10,0);
                lcd.print("uSD IS");
                
                lcd.setCursor(0,1);
                lcd.print(globalHumidity,2);
                lcd.print("%HR");
                lcd.setCursor(11,1);
                lcd.print("FULL");
            break;
            case 5:
                lcdSwitchState = 0;
                lcd.setBacklight(backlight);
                lcd.setRGB(255, 128, 0);
                lcd.clear();

                lcd.setCursor(0,0);
                lcd.print(globalTemperature,2);
                lcd.print((char)223);
                lcd.print("C ");
                lcd.setCursor(10,0);
                lcd.print("uSD");
                
                lcd.setCursor(0,1);
                lcd.print(globalHumidity,2);
                lcd.print("%HR");
                lcd.setCursor(9,1);
                lcd.print("FAILURE");
            break;
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