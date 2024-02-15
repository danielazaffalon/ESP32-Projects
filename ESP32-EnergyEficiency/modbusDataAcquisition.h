#ifndef MODBUSDATAACQUISITION_h
#define MODBUSDATAACQUISITION_h
#include <ArduinoRS485.h>
#include <ModbusRTUClient.h>

// extern int lcdSwitchState;
void modbusInitialization();
void modbusAcquisition(float (&dataArray) [2]);

#endif