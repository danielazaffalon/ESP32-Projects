/**
 * @file ESP32ESP32-UnitedGrinding.ino
 *
 * @brief This sketch reads temperature and pressure from BMP280 sensor, 
 * packages it in a JSON object, and sends it through Wi-Fi with HTTP 
 * communication to "express" local server.
 * 
 * Common Errors:
 * 
 * 1) Error in C:\Users\wes0439\AppData\Local\Arduino15\packages\{company}\hardware\esp32\1.0.0\variants\esp32\pins_esp32.h
 * REPLACE:
 * 17. #define A       (17)
 * 18. #define B       (16)
 * 22. static const uint32_t pin11 = A;
 * 23. static const uint32_t pin12 = B;
 * WITH:
 * 17. #define A_RS485       (17)
 * 18. #define B_RS485       (16)
 * 22. static const uint32_t pin11 = A_RS485;
 * 23. static const uint32_t pin12 = B_RS485;
 * 
 * 2) Error in C:\Users\wes0439\Documents\Arduino\libraries\Adafruit_BMP280_Library\Adafruit_BMP280.h
 * REPLACE:
 * 34. #define BMP280_ADDRESS (0x77)
 * WITH:
 * 34. #define BMP280_ADDRESS (0x76) 
 * 
 * 3)Error in C:\Users\wes0439\AppData\Local\Arduino15\packages\{company}\hardware\esp32\1.0.0\libraries\WiFi\src\WiFiClient.cpp
 * REPLACE: 
 * 515. res = recv(fd(), buf, toRead, MSG_DONTWAIT);
 * WITH:
 * 515. res = read(buf, a);
 * https://github.com/espressif/arduino-esp32/issues/6129
 *
 * @author Daniela Zaffalon.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "Secrets.h"
#include <Adafruit_BMP280.h>

const char *WIFI_SSID = SSID; //Wi-Fi SSID
const char *WIFI_PASS = PASS; //Wi-Fi Password

WiFiClient wifiClient;
HTTPClient http;
Adafruit_BMP280 bmp;
unsigned long startTime;

IPAddress serverAddress(192,168,1,43); //Server IP
uint16_t serverPort = 1880; //Server Port
String serverUri = "/"; //Server URL
int sendTime = 5000; //Send HTTP Post each 10 s.

void wifiConnect();

void setup(){
    Serial.begin(115200);
    wifiConnect();
    if (!bmp.begin()) {
        Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
        while (1);
    }
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Modo de operación */
                Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                Adafruit_BMP280::SAMPLING_X16,    /* Presion oversampling */
                Adafruit_BMP280::FILTER_X16,      /* Filtrado. */
                Adafruit_BMP280::STANDBY_MS_500); /* Tiempo Standby. */
    startTime = millis();
}

void loop(){
    unsigned long actualTime = millis();
    if(actualTime-startTime >= sendTime) {
        // Send HTTP POST request
        http.begin(serverAddress.toString(), serverPort, serverUri);
        http.addHeader("Content-Type", "application/json");
        char postData[50];
        sprintf(postData, "{\"Temperature\":%.2f,\"Pressure\":%.0f}\0", bmp.readTemperature(),bmp.readPressure());
        Serial.print("Post Message: ");
        Serial.println(postData);
        int httpResponseCode = http.POST(postData);
        http.end();
        Serial.print("HTTP Response Code: ");
        Serial.println(httpResponseCode);
        startTime = millis();
    }
}

/**
 * Function to connect to WiFi Network.
 */
void wifiConnect(){
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.println("WiFi Status: ");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(100);
    }
    Serial.print("Wifi Connected with IP: ");
    Serial.println(WiFi.localIP());
}
