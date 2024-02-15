#include <Arduino.h>
#include <jsonBuilder.h>

void setup() {
  Serial.begin(9600);
}

void loop(){
  //Creation of Static JSON Document and clear this memory space each time.
  StaticJsonDocument <1024> doc; //DynamicJsonDocument doc (2048); (for Dynamic) 

  //Create variable to save String with Json output
  char outputString[1024];

  //Decentralized Function to create Json structure
  //Note: In this structur the objects Object_A1_Name and Object_A3_Name are empty
  // createJsonStructure(doc);

   //Decentralized Function to create Json structure by String using ArduinoJson
  char input[]= "{\"Object_A_Name\":{\"Object_A1_Name\":{\"Object_A1_Field1\":\"MainObjectA1_Field1_Value\"},\"Object_A2_Name\":{\"Object_A2_1_Name\":{\"ArrayObject_A2_1a_Name\":[\"ArrayObject_A2_1a_Field1\",\"ArrayObject_A2_1a_Field2\"]}},\"Object_A3_Name\":{\"Object_A3_Field1\":\"Object_A3_Field1_Value\"}},\"Object_B_Name\":{\"Object_B1_Name\":{\"Object_B1_1_Name\":{\"ArrayObject_B1_1a_Name\":[\"ArrayObject_B1_1a_Field1\",\"ArrayObject_B1_1a_Field2\",\"ArrayObject_B1_1a_Field3\",\"ArrayObject_B1_1a_Field4\"]}}}}";
  createJsonDocByString(doc, input);

  //Add new values to JsonDocument
  String MainObjectA1_Field1_Value = "MainObjectA1_Field1_Value";
  String Object_A3_Field1_Value = "Object_A3_Field1_Value";
  doc["Object_A_Name"]["Object_A1_Name"]["Object_A1_Field1"] = MainObjectA1_Field1_Value;
  doc["Object_A_Name"]["Object_A3_Name"]["Object_A3_Field1"] = Object_A3_Field1_Value;

  //Convert doc into String by original Arduino Json Library
  serializeJson(doc, outputString);
  Serial.println(outputString);

  //Update variable value to interger
  doc["Object_A_Name"]["Object_A3_Name"]["Object_A3_Field1"] = 1050;

  //Convert doc into String by a function
  stringifyJSON(doc, outputString, sizeof(outputString), true);
  
  Serial.println(outputString);
  delay (5000); 
}