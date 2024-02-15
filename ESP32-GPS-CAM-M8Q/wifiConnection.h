#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include <esp_crt_bundle.h>
// #include <WiFiClientSecure.h>
#include <WiFi.h>
#include "Secrets.h"

// extern WiFiClientSecure wifiClientSecure;
extern WiFiClass WiFi;

bool wifiConnect();

void printStatus();

#endif