#ifndef LCDTASKCODE_H
#define LCDTASKCODE_H

extern int lcdSwitchState;
extern double globalTemperature;
extern double globalHumidity;
extern bool lastHvacState;
extern IPAddress wifiIP;
extern bool alertMaxTimeDeviceON;

void createTask();
void lcdTaskcode( void * parameter);

#endif