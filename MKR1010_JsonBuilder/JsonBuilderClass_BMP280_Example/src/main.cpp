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
#include <JsonBuilder.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;
//Instance of library that build the Json Structure
JsonBuilder jsonGen;
//Create variable to save String with Json output
char outputString[1024];

float temperatureHistoric[5];
float preassureHistoric[5];
float altitudeHistoric[5];

int i=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
 
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Modo de operación */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Presion oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtrado. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Tiempo Standby. */

  jsonGen.doc["createTime"] = millis();
}

void loop() 
{
  jsonGen.doc["fields"]["temp"]["arrayValue"]["values"][i] = bmp.readTemperature();
  jsonGen.doc["fields"]["date"]["arrayValue"]["values"][i] = millis();
  jsonGen.doc["fields"]["ubi"]["arrayValue"]["values"][i] = "Sant Just Desvern";
  jsonGen.doc["fields"]["cad"]["stringValue"] = "25/03/2021";
  
  if(i == 4){
    jsonGen.doc["updateTime"] = millis();
    //Convert this new doc into Pretty Format String and Printing
    jsonGen.stringifyJSONPretty(outputString, sizeof(outputString));
    Serial.println(outputString);
    i=0;
  }
  else i++;

  Serial.println();
  delay(2000);
}