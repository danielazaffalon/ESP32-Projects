#ifndef jsonBuilder_h
#define jsonBuilder_h
#include <ArduinoJson.h>

/**
 * Create JSON Object Document with wanted Structure.
 * @param doc Json Object Document to store JSON Object. Could be Static or Dynamic.
 * This document could be static, dynamic.
*/
void createJsonStructure(JsonDocument& doc);

/**
 * Create JSON Object Document from String.
 * @param doc Json Object Document to store JSON Object. Could be Static or Dynamic.
 * @param arrayInput String with Json Object nested structure 
*/
void createJsonDocByString(JsonDocument& doc, const char* arrayInput);

/**
 * stringifyJSON: Stringify the JSON object Document "doc" into "outputString" 
 * with user defined size "outputsize".
 * @param doc Json Object Document. Could be Static or Dynamic.
 * @param outputString String (char Array) where the JSON going to be saved.
 * @param outputSize size of "outputString".
 * @param prettify boolean: true return JSON payload Pretty,
 *                          false returne JSON payload String.
*/
void stringifyJSON(JsonDocument& doc, char* outputString, size_t outputSize, bool prettify);

#endif