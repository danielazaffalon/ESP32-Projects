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
 * @brief Class for handling Json Object definition and serialization
 * */
// template <size_t desiredCapacity>
class JsonBuilder {
    private:
        void _createJsonStructure();

    public:
        /* Public Variables */
        StaticJsonDocument <1024> doc;

        /*Public Methods*/
        
        /**
        * @brief Class Constructor (without params)
        */
        JsonBuilder();
    
        /**
        * @brief stringifyJSONPretty: JSON Serialization into pretty format. 
        * @param output Reference to char array where stringified Json Object is stored.
        * @param outputSize Size of "output" array.
        **/
        void stringifyJSONPretty(char* output, size_t outputSize);

        /**
        * @brief stringifyJSON: JSON Serialization. 
        * @param output Reference to char array where stringified Json Object is stored.
        * @param outputSize Size of "output" array.
        **/
        void stringifyJSON(char* output, size_t outputSize);
};

#endif

    /**
    * @brief Class Constructor with params
    * @param documentType boolean: false create a Static document and true create a Dynamic one.
    * @param documentSize unsigned int give the size of document
    */
    // JsonBuilder(bool documentType);

    /**
    *  @brief setJsonValue: Set method to update value of specific key.
    *  @param key key of the value to update.
    *  @param value new value to update.
    **/
    // void setJsonValue(char* key, int value);