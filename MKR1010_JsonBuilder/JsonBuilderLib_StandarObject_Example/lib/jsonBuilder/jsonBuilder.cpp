#include "jsonBuilder.h"

void createJsonStructure(JsonDocument& doc) {
  //Create Main JSON Object
  JsonObject Object_Main = doc.to<JsonObject>();
    
    //JSON Object A (inside Main)
    JsonObject Object_A = Object_Main.createNestedObject("Object_A_Name"); 
      
      //JSON Object A1 (inside A)   
      Object_A.createNestedObject("Object_A1_Name"); //Not starter value assignet to this key

      //JSON Object A2 (inside A)
      JsonObject Object_A2 = Object_A.createNestedObject("Object_A2_Name");      
        JsonObject Object_A2_1 = Object_A2.createNestedObject("Object_A2_1_Name");
          //Create Array Object
          JsonArray ArrayObject_A2_1a = Object_A2_1.createNestedArray("ArrayObject_A2_1a_Name");
            ArrayObject_A2_1a.add("ArrayObject_A2_1a_Field1");
            ArrayObject_A2_1a.add("ArrayObject_A2_1a_Field2");           
      //JSON Object A3 (inside A)
      Object_A.createNestedObject("Object_A3_Name"); //Not starter value assignet to this key  
 
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

void createJsonDocByString(JsonDocument& doc, const char* arrayInput) {
  DeserializationError error = deserializeJson(doc, arrayInput);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
}

void stringifyJSON(JsonDocument& doc, char* outputString, size_t outputSize, bool prettify){
  if (prettify) serializeJsonPretty(doc, outputString, outputSize);
  else serializeJson(doc, outputString, outputSize);
}