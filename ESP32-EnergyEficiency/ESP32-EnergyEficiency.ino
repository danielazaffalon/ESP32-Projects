/**
 * @file ESP32-EnergyEficiency.ino
 *
 * @brief This sketch is connected to CMT-10-A0-RC-P detecting 
 * the states ON and OFF of HVAC and the device current. Also, 
 * it measures the environmental temperature, and humidity using 
 * an SHT20 Modbus sensor. The sketch refresh and show the environmental 
 * and HVAC state values each 10 seconds. It show it through LCD screen 
 * using the ESP-32 second core and sends the information to Losant.
 *
 * @author Daniela Zaffalon.
 */

// https://www.dfrobot.com/product-1609.html
// https://randomnerdtutorials.com/esp32-esp8266-i2c-lcd-arduino-ide/ No funcionó
#include "wifiConnection.h"
#include "modbusDataAcquisition.h"
// #include "rtcFunction.h"
// #include "DHT22Acquisition.h"

// #define btoa(x) ((x)?"true":"false")
static const uint32_t hvacStateInput = pin25;
static const uint32_t voltageInput = pin15;
static const uint32_t relayOutput = pin55;
int maxTimeDeviceON = 60;// 3 horas = 10800 seconds. 1080U;

unsigned long startTime;
// bool relayState;
int counter = 0;

void setup(){
	Serial.begin(115200);
  createTask();
  lastHvacState = false;
  lcdSwitchState = 1;
  alertMaxTimeDeviceON = false;
  delay(5000);
  modbusInitialization();
  // dht22Initialization();
  // rtcInitialization();
  // rtcRead();
  startTime = millis();
  lastHvacState = digitalRead(hvacStateInput);
  device.onCommand(&handleCommand);
}

void loop() {
  unsigned long actualTime = millis();
  float dataArray [2];
  bool hvacState = digitalRead(hvacStateInput);
  int analogValue = analogRead(voltageInput);
  float hvacVoltage = analogValue*10.0f/4095;
  float hvacCurrent = hvacVoltage/2;
  bool maxTimeOn = false;

  if(WiFi.status() != WL_CONNECTED) wifiConnect();
  else if(!device.connected()) losantConnect();
  else if((hvacState != lastHvacState) || (actualTime-startTime >= refreshTime)) {
    // dht22Acquisition();
    modbusAcquisition(dataArray);
    // Serial.printf("SHT20:\t\t%.2f\t\t%.2f\n",dataArray[0],dataArray[1]);
    // Serial.printf("%.2f [V] \t\t %.2f [A] \r\n", hvacVoltage, hvacCurrent);
    // Serial.printf(hvacState ? "ROOM HVAC ON\n" : "ROOM HVAC OFF\n");

    StaticJsonDocument<100> doc;
    doc["temperature"] = globalTemperature = dataArray[0];
    doc["humidity"] = globalHumidity = dataArray[1];
    doc["hvacState"] = hvacState;
    doc["hvacCurrent"] = hvacCurrent;
    JsonObject root = doc.as<JsonObject>();
    device.sendState(root);

    lastHvacState = hvacState;
    lcdSwitchState = 4;
    startTime = millis();
  }

  device.loop();

  if(outputChange) {
    // relayState = !relayState;
    digitalWrite(relayOutput,relayState);
    outputChange = false;
  }
}