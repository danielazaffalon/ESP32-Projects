#include <Arduino.h>
#include <ArduinoJson.h>
#include "secrets.h"
#include <PubSubClient.h>
#include <GPRS.h>
// #include <Losant.h>

const char* GPRS_PIN = PIN;
const char* GPRS_APN = APN;
const char* GPRS_USERNAME = USERNAME;
const char* GPRS_PASSWORD = PASSWORD;

// Losant credentials.
const char* LOSANT_DEVICE_ID = SECRET_DEVICE_ID;
const char* LOSANT_ACCESS_KEY = SECRET_ACCESS_KEY;
const char* LOSANT_ACCESS_SECRET = SECRET_ACCESS;

//MQTT Connection based on: https://docs.losant.com/mqtt/overview/
const char* server = "broker.losant.com";
int port = 1883;
GPRSClient gprsclient;
PubSubClient pubsubclient(server, port, gprsclient);

enum networkStatus {NOT_REGISTERED=0, REGISTERED, SEARCHING, REGISTRATION_DENIED, UNKNOWN, REGISTERED_ROAMING};

bool gprsConnected = false;
unsigned long lastStatusTime =  millis();

char topic[38] = "losant/";

void connectGPRS(){
  static uint32_t lastStatusTime = millis();

   if (!GPRS.begin()) {
    Serial.println("Impossible to begin GPRS");
    while(true);
  }

  delay(2000); // To avoid Bloqued SIM
  int pinRequired = GPRS.isPINRequired();
  if (pinRequired == 1) {
    if (!GPRS.unlockSIM(PIN)) {
      Serial.println("Invalid PIN");
      while (true);
    }
  }
  else if (pinRequired != 0) {
    Serial.println("Blocked SIM");
    while (true);
  }

  while(!gprsConnected){
    if (millis() - lastStatusTime > 2000) {
      uint8_t networkStatus = GPRS.getNetworkStatus();
      lastStatusTime = millis();

      if ((networkStatus == REGISTERED) || (networkStatus == REGISTERED_ROAMING)) {
        int GPRSStatus = GPRS.getGPRSStatus();
        if (GPRSStatus == 0) {
          if (!GPRS.enableGPRS(APN, USERNAME, PASSWORD)) {
            Serial.println("GPRS not enabled");
          }
        }
        else if (GPRSStatus == 1) {
          if (GPRS.connected()) {
          gprsConnected = true;
          Serial.println("GPRS is connected");
          }
        }
      }
    }
  }
}

void connectLosant(){
  Serial.println();
  Serial.print("Connecting to Losant");
  pubsubclient.connect(LOSANT_DEVICE_ID,LOSANT_ACCESS_KEY,LOSANT_ACCESS_SECRET);
  while(!pubsubclient.connected()) { //The scketch never comme into this
    Serial.print(".");
    delay(1000);
  }
}

void setup() {
  Serial.begin(9600UL);
  pinMode(Q0_3, OUTPUT); //Used to notify connection to Losant
  
  connectGPRS();
  connectLosant();

  digitalWrite(Q0_3, HIGH);

  //Create Topic
  char* topic2 = "/state";
  strcat(topic,LOSANT_DEVICE_ID);
  strcat(topic,topic2);
}

void loop() {
  //Create Payload
  StaticJsonDocument <50> doc;
  char payload[50];
  
  doc.createNestedObject("data");
  doc["data"]["state"] = "CONNECTED";

  serializeJson(doc, payload, sizeof(payload));

  Serial.println(payload);
  // pubsubclient.publish(topic,payload);

  delay(5000);
}