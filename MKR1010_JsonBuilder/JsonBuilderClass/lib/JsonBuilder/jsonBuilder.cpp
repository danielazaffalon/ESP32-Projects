/**
 * @file ESP32_HTTPClientPOSTTest.ino
 *
 * @brief This sketch 
 *
 * @author Daniela Zaffalon.
 * Contact: dannyzaffalon@hotmail.com
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#include "jsonBuilder.h"

JsonBuilder::JsonBuilder(){
  this->_createJsonStructure();
}

void JsonBuilder::stringifyJSONPretty(char* output, size_t outputSize){
  serializeJsonPretty(this->doc, output, outputSize);
}

void JsonBuilder::stringifyJSON(char* output, size_t outputSize){
  serializeJson(this->doc, output, outputSize);
}

void JsonBuilder::_createJsonStructure(){

   //Create Main JSON Object
  JsonObject Object_Main = this->doc.to<JsonObject>();
    
    //JSON Object A (inside Main)
    JsonObject Object_A = Object_Main.createNestedObject("Object_A_Name"); 
      
      //JSON Object A1 (inside A)   
      Object_A.createNestedObject("Object_A1_Name");

      //JSON Object A2 (inside A)
      JsonObject Object_A2 = Object_A.createNestedObject("Object_A2_Name");      
        JsonObject Object_A2_1 = Object_A2.createNestedObject("Object_A2_1_Name");
          //Create Array Object
          JsonArray ArrayObject_A2_1a = Object_A2_1.createNestedArray("ArrayObject_A2_1a_Name");
            ArrayObject_A2_1a.add("ArrayObject_A2_1a_Field1");
            ArrayObject_A2_1a.add("ArrayObject_A2_1a_Field2");           
      //JSON Object A3 (inside A)
      Object_A.createNestedObject("Object_A3_Name");         

    //JSON Object B (inside Main)
    JsonObject Object_B = Object_Main.createNestedObject("Object_B_Name");
      //JSON Object B1 (inside B)
      JsonObject Object_B1 = Object_B.createNestedObject("Object_B1_Name");      
        JsonObject Object_B1_1 = Object_B1.createNestedObject("Object_B1_1_Name");
          //Create Array Object
          JsonArray ArrayObject_B1_1a = Object_B1_1.createNestedArray("ArrayObject_B1_1a_Name");
            ArrayObject_B1_1a.add("ArrayObject_B1_1a_Field1");
            ArrayObject_B1_1a.add("ArrayObject_B1_1a_Field2");  
            ArrayObject_B1_1a.add("ArrayObject_B1_1a_Field3");  
            ArrayObject_B1_1a.add("ArrayObject_B1_1a_Field4");  
}

// Function to Update some value (Is almost the same that use directly ArduinoJson Library)

// void JsonBuilder::setJsonValue(char* key, int value){ //, int value
//   JsonObject jsonObjectMain;
//   char* firstChar;
//   int i = 0;
//   int count = 0;

//   // Count hoy many times delimiter "." appears
//   char* pointer = key;
//   while(*pointer != 0){
//     if (*pointer == '.') count++;
//     pointer++;
//   }

//   // On a first call, strtok expects a string as argument, 
//   // whose first character is used as the starting location to scan for tokens. 
//   // In subsequent calls, the function expects a null pointer and uses the position 
//   // right after the end of the last token as the new starting location for scanning.

//   firstChar = strtok(key,".");

//   while(firstChar != 0){
//     if (i == 0) {
//       jsonObjectMain = this->doc[firstChar];
//     }
//     else if(i == count){
//       jsonObjectMain[firstChar] = value;
//     }
//     else{
//       jsonObjectMain = jsonObjectMain[firstChar];
//     }
//     firstChar = strtok(0,".");
//     i++;
//   }
// }