#include "wifiConnection.h"

// #define MQTT_MAX_PACKET_SIZE 256 //https://docs.losant.com/devices/commands/
const char *WIFI_SSID = SSID;
const char *WIFI_PASS = PASS;
const char *losantRootCA = rootCABuff;
const char *LOSANT_ACCESS_KEY = ACCESS_KEY;
const char *LOSANT_ACCESS_SECRET = ACCESS_SECRET;
const char *LOSANT_DEVICE_ID = DEVICE_ID;

bool receivedCommand;
int refreshTime = 60000;
bool outputChange = true;
bool relayState = false;

LosantDevice device(LOSANT_DEVICE_ID);
WiFiClientSecure wifiClientSecure;

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
  wifiIP = WiFi.localIP();
  lcdSwitchState = 2;
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
    lcdSwitchState = 3;
    delay(500);
    Serial.print(".");
    losantCounter ++;
    if(losantCounter >= 20){
      device.connectSecure(wifiClientSecure, LOSANT_ACCESS_KEY, LOSANT_ACCESS_SECRET);
      losantCounter = 0;
    }
  }
  Serial.println("Connected!");
  lcdSwitchState = 4;
  return true;
}

void handleCommand(LosantCommand *command){
  receivedCommand = true;

  // Serial.print("Command received: ");
  // Serial.println(command->name);
  // Serial.print("Payload received: ");
  // JsonObject& payload = *command->payload;
  // String state = payload["toggleButton"];
  // Serial.println(state);

  if(strcmp(command->name, "SendStateTimer") == 0){
    JsonObject& payload = *command->payload;
    refreshTime = payload["delay"];
    Serial.printf("The Refresh time was set to: %i\r\n",refreshTime); 
  }

  if(strcmp(command->name, "toggleButton") == 0){
    JsonObject& payload = *command->payload;
    relayState = payload["state"];
    Serial.printf("The Refresh time was set to: %s \r\n",relayState? "ON":"OFF");
    outputChange = true;
  }
}

  //ERROR LOSANT POR ENVIOS MUY FRECUENTES Connection <outbound|inbound> throughput limit exceeded
  // https://docs.losant.com/devices/overview/
    // Serial.println(device.mqttClient.lastError()); //LWMQTT_NETWORK_FAILED_READ = -5,
    // Serial.println(device.mqttClient.returnCode()); //LWMQTT_UNKNOWN_RETURN_CODE = 6
