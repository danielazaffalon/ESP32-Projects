#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include <esp_crt_bundle.h>
#include <WiFiClientSecure.h>
#include <Losant.h>
#include "Secrets.h"
#include "lcdTaskCode.h"

extern WiFiClientSecure wifiClientSecure;
extern LosantDevice device;
extern bool receivedCommand;
extern int refreshTime;
extern bool outputChange;
extern bool relayState;

bool wifiConnect();
bool losantConnect();
void handleCommand(LosantCommand *command);

#endif