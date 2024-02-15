/**
 * @file Json Builder
 *
 * @brief
 *
 * @author Daniela Zaffalon.
 * Contact: dannyzaffalon@hotmail.com
 */

#ifndef jsonBuilder_h
#define jsonBuilder_h
#include <ArduinoJson.h>

/**
 * @brief Create JSON Object Document with wanted Structure.
 * @param doc Json Object Document to store JSON Object. Could be Static or Dynamic.
 * This document could be static, dynamic.
*/
void createJsonStructure(JsonDocument& doc);

/**
 * @brief Create JSON Object Document from String.
 * @param doc Json Object Document to store JSON Object. Could be Static or Dynamic.
 * @param arrayInput String with Json Object nested structure 
*/
void createJsonDocByString(JsonDocument& doc, const char* arrayInput);

/**
 * @brief Fill Json Object with values.
 * @param doc Json Object Document.
 * @param temp Float temperature.
 * @param time time in miliseconds.
 * @param place String with Place.
 * @param i position of array.
*/
void fillJson(JsonDocument& doc, float temp, unsigned long time, char place[], int i);

/**
 * @brief stringifyJSON: Stringify the JSON object Document "doc" into "outputString" 
 * with user defined size "outputsize".
 * @param doc Json Object Document. Could be Static or Dynamic.
 * @param outputString String (char Array) where the JSON going to be saved.
 * @param outputSize size of "outputString".
 * @param prettify boolean: true return JSON payload Pretty,
 *                          false returne JSON payload String.
*/
void stringifyJSON(JsonDocument& doc, char* outputString, size_t outputSize, bool prettify);

#endif