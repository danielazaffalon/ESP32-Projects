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
 
char outputString[1024]; 
int i;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
}

void loop(){
  //Instance of library that build the Json Structure
  JsonBuilder jsonGen;
  //Create variable to save String with Json output
  char outputString[1024];
  
  //Convert doc into String and Printing. This keys are empty: "Object_A1_Field1" and "Object_A3_Field1"
  jsonGen.stringifyJSON(outputString, sizeof(outputString));
  Serial.println(outputString);
  Serial.println();

  // Assign new values from variables to JsonDocument keys
  String MainObjectA1_Field1_Value = "MainObjectA1_Field1_Value";
  String Object_A3_Field1_Value = "Object_A3_Field1_Value";
  jsonGen.doc["Object_A_Name"]["Object_A1_Name"]["Object_A1_Field1"] = MainObjectA1_Field1_Value;
  jsonGen.doc["Object_A_Name"]["Object_A3_Name"]["Object_A3_Field1"] = Object_A3_Field1_Value;

  //Convert this new doc into String and Printing
  jsonGen.stringifyJSON(outputString, sizeof(outputString));
  Serial.println(outputString);
  Serial.println();

  //Update value of "Object_A3_Field1" key
  jsonGen.doc["Object_A_Name"]["Object_A3_Name"]["Object_A3_Field1"] = 1050;

  //Convert this new doc into Pretty Format String and Printing
  jsonGen.stringifyJSONPretty(outputString, sizeof(outputString));
  Serial.println(outputString);
  Serial.println();

  delay (5000); 
}

// void loop(){
//   char jsonOutput[1024]; // Should be > 512 Bytes to this example
//   if(i>1) {
//     char key[] = "Object_A_Name.Object_A3_Name.Object_A3_Field1";
//     jsonGen.setJsonValue(key, 1050);
//   }
//   jsonGen.getPrettyPayload(jsonOutput, sizeof(jsonOutput));
//   String printableOut = jsonOutput;
//   Serial.println(printableOut);
//   i++;
//   delay (5000);
// } 