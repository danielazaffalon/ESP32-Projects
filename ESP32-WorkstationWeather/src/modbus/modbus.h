#include <ArduinoRS485.h>
#include <ModbusRTUClient.h>
#include "../debug.h"

#ifndef MODBUS_h
#define MODBUS_h

bool modbusInitialization();
float modbusTemperature();
float modbusHumidity();
void modbusAcquisition(float (&dataArray) [2]);

#endif