/**
 * @file DS18B20/main.cpp
 * @brief This example of library DallasTemperature and OneWire using.
 * 
 * Setup and read information from two sensors using most of methods of library
 * Begin communication, ask for addresses, read temperature using index and address
 * get and set resolution, get connection state, temperature in ºC and ºF and handle alarms.
 * 
 * Written by Daniela Zaffalon
*/
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/*Pin to connect DQ wire of OneWire*/
const byte pinDQ = 6;

/*Instantiate classes */
/*OneWire is Sensor Data protocol*/
OneWire oneWireObjeto(pinDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);

/*Library data type that is 8 bytes array (uint8_t)*/
DeviceAddress temperatureSensor1;
DeviceAddress temperatureSensor2;

/* Find the number of devices connected*/
void findDevices(){
  Serial.print("Devices found: ");
  Serial.print(sensorDS18B20.getDeviceCount());
  Serial.println(" sensores");
  Serial.println();
}

/*Obtain the sensor address by index number and save it in deviceAddress array*/
void getSensorsAddresses(uint8_t* deviceAddress, uint8_t sensorNumber){
  sensorDS18B20.getAddress(deviceAddress, sensorNumber);
  Serial.print("Device ");
  Serial.print(sensorNumber);
  Serial.print(" Address ");
  for (uint8_t i = 0; i < 8; i++) {
    // If obtein just one digit, add left 0
    if (deviceAddress[i] < 16) Serial.print("0");
    //Print Data in HEX
    Serial.print(deviceAddress[i], HEX);
  }
  Serial.println();
}

/*Get temeprature asking by index number (This example have two sensors)*/
void getTempByIndex(){
  sensorDS18B20.requestTemperatures();
  Serial.println();
  Serial.print("Temperatura Sensor 1 by Index:  ");
  Serial.print(sensorDS18B20.getTempCByIndex(0));
  Serial.print(" C, ");
  Serial.print(sensorDS18B20.getTempFByIndex(0));
  Serial.println(" F");
  Serial.print("Temperatura sensor 2: by Index: ");
  Serial.print(sensorDS18B20.getTempCByIndex(1));
  Serial.print(" C, ");
  Serial.print(sensorDS18B20.getTempFByIndex(1));
  Serial.println(" F.");
  Serial.println();
}

/*Accessing temperature by address number and print in Celsius and Fahrenheit*/
void getTempByAddress(uint8_t* deviceAddress){
  uint8_t sensorNumber;
  if(deviceAddress[7] == temperatureSensor1[7]) sensorNumber = 1;
  else sensorNumber = 2;
  Serial.println();
  Serial.print("Temperature sensor "); 
  Serial.print(sensorNumber);
  Serial.print(" by Address: ");
  Serial.print(sensorDS18B20.getTempC(deviceAddress));
  Serial.print(" C, ");
  Serial.print(sensorDS18B20.getTempF(deviceAddress));
  Serial.print(" F.");
  Serial.println();
}

/* set resolution of a device to 9, 10, 11, or 12 bits
 * if new resolution is out of range, 9 bits is used. */
void sensorResolution(uint8_t* deviceAddress, uint8_t newResolution){
  uint8_t sensorResolution = sensorDS18B20.getResolution(deviceAddress);;
  Serial.print("Sensor resolution is: "); 
  Serial.println(sensorResolution);

  /* Set Sensor to new resolution*/
  sensorDS18B20.setResolution(deviceAddress, newResolution);
  sensorResolution = sensorDS18B20.getResolution(deviceAddress);;
  Serial.print("Sensor 1 new resolution is: "); 
  Serial.println(sensorResolution);
  Serial.println();
}

/* Function to check by address if some sensor is connected*/
void sensorConnection(uint8_t* deviceAddress){
  Serial.println();
  Serial.print("Sensor ");
  for (uint8_t i = 0; i < 8; i++) {
    // If obtein just one digit, add left 0
    if (deviceAddress[i] < 16) Serial.print("0");
    //Print Data in HEX
    Serial.print(deviceAddress[i], HEX);
  }
  if(sensorDS18B20.isConnected(deviceAddress)) Serial.print(" is Connected ");
  else Serial.print(" is Not Connected ");
}

/* Callback function: when alarm is activate print a message*/
/* It is imperative that the args of function are const uint8_t where address
 * will be returned.
 */
void alarmHandler(const uint8_t* deviceAddress){
  Serial.println();
  Serial.print("Alarm activate for device ");
  for (uint8_t i = 0; i < 8; i++) {
    // If obtein just one digit, add left 0
    if (deviceAddress[i] < 16) Serial.print("0");
    //Print Data in HEX
    Serial.print(deviceAddress[i], HEX);
  }
  Serial.print(" with temperature ");
  Serial.print(sensorDS18B20.getTempC(deviceAddress));
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  sensorDS18B20.begin();
  delay(5000);

  /* Using function getDeviceCount and print in screen */
  findDevices();

  /* Using function getAddress and print in screen */
  Serial.println("Temperature Sensors Adresses");
  getSensorsAddresses(temperatureSensor1, 0);
  getSensorsAddresses(temperatureSensor2, 1);

  /* Show if sensor es connected */
  sensorConnection(temperatureSensor1);
  sensorConnection(temperatureSensor2);

  /* Get and set sensor resolution and print in screen*/
  sensorResolution(temperatureSensor1, 12);
  sensorResolution(temperatureSensor2, 12);

  /* Indicate if the connection is in parasite mode and the power supply*/
  if(sensorDS18B20.isParasitePowerMode()) Serial.print("Sensores are connected in parasite mode");

  /* Define the ranges of normal temperatures, out of them alarm is activate*/
  sensorDS18B20.setLowAlarmTemp(temperatureSensor1, 1);
  sensorDS18B20.setHighAlarmTemp(temperatureSensor1, 20);
  sensorDS18B20.setLowAlarmTemp(temperatureSensor2, 1);
  sensorDS18B20.setHighAlarmTemp(temperatureSensor2, 20);
  /* Define callback function when alarm is activate*/
  sensorDS18B20.setAlarmHandler(&alarmHandler);
}

void loop() {
  /* Using function getTempCByIndex with two sensors and print in screen */
  getTempByIndex();

  /*Accessing temperature by adress and print in Celsius and Fahrenheit*/
  getTempByAddress(temperatureSensor1);
  getTempByAddress(temperatureSensor2);

  /* Library Method to check if some alarm is activated*/
  sensorDS18B20.processAlarms();

  delay(10000);
}