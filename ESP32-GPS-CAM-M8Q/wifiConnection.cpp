#include "wifiConnection.h"
#include "debug.h"

// #define MQTT_MAX_PACKET_SIZE 256 //https://docs.losant.com/devices/commands/
const char *WIFI_SSID = SSID;
const char *WIFI_PASS = PASS;
int connectionTimeout = 2000;

bool wifiConnect(){
  unsigned long startTime = millis();
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("WiFi Status: ");
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  while (WiFi.status() != WL_CONNECTED) {
    printStatus();
    unsigned long actualTime = millis();
    if(actualTime-startTime >= connectionTimeout){
      WiFi.begin(WIFI_SSID, WIFI_PASS);
      startTime = millis();
    }
    delay(100);
  }

  Serial.print("Wifi Connected with IP: ");
  Serial.println(WiFi.localIP());
  return true;
}

void printStatus(){
  if (WiFi.status() == 255) DEBUG_PRINT Serial.printf("NO_SHIELD\r\n");
  else if (WiFi.status() == 0) DEBUG_PRINT Serial.printf("IDLE_STATUS\r\n");
  else if (WiFi.status() == 1) DEBUG_PRINT Serial.printf("NO_SSID_AVAIL\r\n");
  else if (WiFi.status() == 2) DEBUG_PRINT Serial.printf("SCAN_COMPLETED\r\n");
  else if (WiFi.status() == 3) DEBUG_PRINT Serial.printf("CONNECTED\r\n");
  else if (WiFi.status() == 4) DEBUG_PRINT Serial.printf("CONNECT_FAILED\r\n");
  else if (WiFi.status() == 5) DEBUG_PRINT Serial.printf("CONNECTION_LOST\r\n");
  else if (WiFi.status() == 6) DEBUG_PRINT Serial.printf("DISCONNECTED\r\n");
  // Serial.print('.');
}