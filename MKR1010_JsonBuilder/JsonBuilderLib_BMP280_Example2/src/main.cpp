/**
 * @file Json Builder
 *
 * @brief This example use the class to buil a JSON Object Generic format
 * write some values into JSON Object 
 * and Print it in Monitor Screen as pretty and string format
 *
 * @author Daniela Zaffalon.
 * Contact: dannyzaffalon@hotmail.com
 */
#include <Arduino.h>
#include <jsonBuilder.h>
#include <Adafruit_BMP280.h>

// Remember adjust sensor address
Adafruit_BMP280 bmp;
//Creation of Static JSON Document and clear this memory space each time.
StaticJsonDocument <1024> doc; //DynamicJsonDocument doc (2048); (for Dynamic)
//Create variable to save String with Json output
char outputString[1024];

int i=0;

void setup() {
  Serial.begin(9600);

  if (!bmp.begin()) {
    delay(2000);
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
 
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Modo de operación */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Presion oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtrado. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Tiempo Standby. */
  //Create Json structure
  // createJsonStructure(doc);

  //Create Json structure by String using ArduinoJson
  char input[]= "{\"name\":\"projects/foodchain-9ffe3/databases/(default)/documents/Items/P001\",\"fields\":{\"ubi\":{\"arrayValue\":{\"values\":[\"Sant Just Desvern\",\"Sant Just Desvern\",\"Sant Just Desvern\",\"Sant Just Desvern\",\"Sant Just Desvern\"]}},\"name\":{\"stringValue\":\"Rap\"},\"date\":{\"arrayValue\":{\"values\":[40280,42281,44282,46282,48283]}},\"category\":{\"stringValue\":\"Lactics\"},\"temp\":{\"arrayValue\":{\"values\":[19.43,19.42,19.42,19.42,19.42]}},\"cad\":{\"stringValue\":\"25/03/2021\"}},\"createTime\":220,\"updateTime\":48283}";
  createJsonDocByString(doc, input);
}

void loop(){

  if(i==5) {
    doc["fields"]["cad"]["stringValue"] = "25/03/2021";
    doc["updateTime"] = millis();
    //Convert doc into String by original Arduino Json Library
    // serializeJson(doc, outputString);
    // Serial.println(outputString);

    //Convert doc into String by a function
    stringifyJSON(doc, outputString, sizeof(outputString), true);
    Serial.println(outputString);
    i=0;
  }
  else {
    //Add new values to JsonDocument
    char place[] = "Sant Just Desvern";
    fillJson(doc, bmp.readTemperature(), millis(), place, i);
    i++;
  }

  delay (2000); 
}