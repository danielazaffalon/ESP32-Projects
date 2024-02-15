#include <WiFiClientSecure.h>
#include <Losant.h>
#include "Secrets.h"

const char *WIFI_SSID = SSID;
const char *WIFI_PASS = PASS;
const char *losantRootCA = rootCABuff;
const char *LOSANT_ACCESS_KEY = ACCESS_KEY;
const char *LOSANT_ACCESS_SECRET = ACCESS_SECRET;
const char *LOSANT_DEVICE_ID = DEVICE_ID;

LosantDevice device(LOSANT_DEVICE_ID);
WiFiClientSecure wifiClientSecure;

unsigned long startTime;
int refreshTime = 10000;

bool wifiConnect();
bool losantConnect();
void handleCommand(LosantCommand *command);

void setup()
{
	Serial.begin(115200);
    startTime = millis();
}

void loop()
{
    unsigned long actualTime = millis();
    if(WiFi.status() != WL_CONNECTED) wifiConnect();
    else if(!device.connected()) losantConnect();
    else if(actualTime-startTime >= refreshTime) {
        Serial.println(analogRead(35));
        StaticJsonDocument<100> doc;
        doc["moisture"] = analogRead(35);
        JsonObject root = doc.as<JsonObject>();
        device.sendState(root);
        device.loop();
        startTime = millis();
        delay(500);
    }
}

bool wifiConnect(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("WiFi Status: ");
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(WiFi.status()); // Serial.print('.');
    delay(100);
  }
  wifiClientSecure.setCACert(losantRootCA);
  Serial.print("Wifi Connected with IP: ");
  Serial.println(WiFi.localIP());
  return true;
}

bool losantConnect(){
  int losantCounter = 0;
  Serial.println();
  Serial.print("Connecting to Losant...");
  device.connectSecure(wifiClientSecure, LOSANT_ACCESS_KEY, LOSANT_ACCESS_SECRET);
  delay(500);
  while(!device.connected()) {
    delay(500);
    Serial.print(".");
    losantCounter ++;
    if(losantCounter >= 20){
      device.connectSecure(wifiClientSecure, LOSANT_ACCESS_KEY, LOSANT_ACCESS_SECRET);
      losantCounter = 0;
    }
  }
  Serial.println("Connected!");
  return true;
}

void handleCommand(LosantCommand *command){
  Serial.print("Command received: ");
  Serial.println(command->name);
}