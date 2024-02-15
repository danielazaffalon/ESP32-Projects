/**
 * @file Json Builder
 *
 * @brief 
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
  doc["name"] = "projects/foodchain-9ffe3/databases/(default)/documents/Items/P001";

  JsonObject fields = doc.createNestedObject("fields");

  JsonArray fields_ubi_arrayValue_values = fields["ubi"]["arrayValue"].createNestedArray("values");
  fields_ubi_arrayValue_values.add("Barcelona");
  fields_ubi_arrayValue_values.add("Barcelona");
  fields_ubi_arrayValue_values.add("Barcelona");
  fields_ubi_arrayValue_values.add("Barcelona");
  fields_ubi_arrayValue_values.add("Sant Just Desvern");
  fields["name"]["stringValue"] = "Rap";

  JsonArray fields_date_arrayValue_values = fields["date"]["arrayValue"].createNestedArray("values");
  fields_date_arrayValue_values.add("2019-10-03 - 18:47:48");
  fields_date_arrayValue_values.add("2019-10-03 - 18:48:10");
  fields_date_arrayValue_values.add("2019-10-03");
  fields_date_arrayValue_values.add("2019-10-03 - 18:48:57");
  fields_date_arrayValue_values.add("2019-10-23 - 10:04:43");
  fields["category"]["stringValue"] = "Lactics";

  JsonArray fields_temp_arrayValue_values = fields["temp"]["arrayValue"].createNestedArray("values");
  fields_temp_arrayValue_values.add("17.6 °C");
  fields_temp_arrayValue_values.add("17.8 °C");
  fields_temp_arrayValue_values.add("17.6 °C");
  fields_temp_arrayValue_values.add("16 °C");
  fields_temp_arrayValue_values.add("16 °C");
  fields["cad"]["stringValue"] = "25/04/2019";
  doc["createTime"] = "2019-10-03T18:47:51.891534Z";
  doc["updateTime"] = "2019-10-23T10:04:46.575572Z";
}