#include <esp_crt_bundle.h>
#include <WiFi.h>

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
#include "Secrets.h"
#include "../display/lcdTaskCode.h"

#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

extern WiFiClass WiFi;

bool wifiConnect();

void printStatus();

void modeAP();

#endif