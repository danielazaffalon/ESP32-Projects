#include "DHT22Acquisition.h"

#define DHTTYPE DHT22
const int DHTPin = pin41;
DHT dht(DHTPin, DHTTYPE);
StaticJsonDocument<50> doc;

void dht22Initialization(){
  dht.begin();
}
void dht22Acquisition(){
  Serial.printf("DHT22:\t\t%.2f\t\t%.2f\n",dht.readTemperature(),dht.readHumidity());
}