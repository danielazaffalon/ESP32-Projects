#include <Wire.h>
#include <DFRobot_RGBLCD1602.h>
#include "../debug.h"

#ifndef LCDTASKCODE_H
#define LCDTASKCODE_H

extern int lcdSwitchState;
extern IPAddress IP;

extern double globalTemperature;
extern double globalHumidity;
extern String globalDate;
extern String globalTime;
extern bool backlight;

void createTask();
void lcdTaskcode( void * parameter);

#endif