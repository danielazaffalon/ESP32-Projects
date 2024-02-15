/**
 * @file ESP32_HTTPClientPOSTTest.ino
 *
 * @brief This sketch measure the processing, sending, waiting server 
 * response and end connection times of the HTTP communication between 
 * client and server. In that way, this sketch is a HTTP Client that 
 * integrated four digital interrupt inputs connected to a button and 
 * a LAN Wi-Fi network connection with a HTTP server PC. 
 * In this way, when any of the digital inputs is activated, 
 * an HTTP POST is sent to a server on the LAN, with the status of the 
 * inputs in a JSON object.
 *
 * @author Daniela Zaffalon.
 * Contact: dannyzaffalon@hotmail.com
 */

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "Secrets.h"
#include "time.h"

WiFiClient wifiClient;
HTTPClient http;
// IPAddress serverAddress(192,168,1,45); //Server IP
IPAddress serverAddress(192,168,1,35);

const char *WIFI_SSID = SSID;
const char *WIFI_PASS = PASS;

// Buttons
const int button1 = 34;
const int button2 = 35;
const int button3 = 32;
const int button4 = 33;

bool buttonPressed = false;
bool printResults = 0;
unsigned long t0 = 0, t1 = 0, t2 = 0, t3 = 0;
unsigned long elapseTime = 0;
int inputsArray[4];

// NTP server to request epoch time
const char* ntpServer = "pool.ntp.org";
unsigned long epochTime; 

// Auxiliar functions declaration
void wifiConnect();
void wiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
void printError();
unsigned long getTime();

// Interrupt functions
void button1ISR(){
  buttonPressed = true;
  inputsArray[0] = 1;
}
void button2ISR(){
  buttonPressed = true;
  inputsArray[1] = 1;
}
void button3ISR(){
  buttonPressed = true;
  inputsArray[2] = 1;
}
void button4ISR(){
  buttonPressed = true;
  inputsArray[3] = 1;
}

void setup() {
  // Configurar interrupciones
  pinMode(button1, INPUT_PULLUP);
  attachInterrupt(button1, button1ISR, FALLING);

  pinMode(button2, INPUT_PULLUP);
  attachInterrupt(button2, button2ISR, FALLING);

  pinMode(button3, INPUT_PULLUP);
  attachInterrupt(button3, button3ISR, FALLING);

  pinMode(button4, INPUT_PULLUP);
  attachInterrupt(button4, button4ISR, FALLING);

  Serial.begin(115200);
  while (!Serial) {};

  wifiConnect();

  configTime(0, 0, ntpServer);
}

void loop(){
  if (buttonPressed) {
    t0 = millis(); //Starting Captured Time

    // Send HTTP POST request
    http.begin(serverAddress.toString(), 3000, "/");
    http.addHeader("Content-Type", "application/json"); // http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    char postData[50];
    epochTime = getTime();
    sprintf(postData, "{\"inputsArray\":[%i,%i,%i,%i],\"Time\":%i}\0", inputsArray[0],inputsArray[1],inputsArray[2],inputsArray[3], epochTime);
    
    t1 = millis(); //Captured time after data creation
    int httpResponseCode = http.POST(postData);
    t2 = millis();  //Captured time after HTTP Post and received Response
    http.end();

    buttonPressed = false;
    printResults = true;
    for(int i=0; i<=3; i++){
      inputsArray[i]=0;
    }
      if(httpResponseCode != 200){
        printError(httpResponseCode);
      }
    t3 = millis();   //Captured time after clear variables and end HTTP connection
  }
  if(printResults){
    elapseTime = (t1-t0);
    Serial.print("Data Processing Time: ");
    Serial.print(elapseTime);
    Serial.println(" ms");

    elapseTime = (t2-t1);
    Serial.print("HTTP Post and wait for response Time ");
    Serial.print(elapseTime);
    Serial.println(" ms");

    elapseTime = (t3-t2);
    Serial.print("Ending HTTP Communication and clean variables: ");
    Serial.print(elapseTime);
    Serial.println(" ms");

    elapseTime = (t3-t0);
    Serial.print("Total Time: ");
    Serial.print(elapseTime);
    Serial.println(" ms");
    Serial.println();
    printResults = false;
  }
}

/**
 * Function to obtain the ESP32 Internal RTC Time.
 */
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

/**
 * Function print HTTP Communication Error.
 * @param httpResponseCode HTTP Error Code
 */
void printError(int httpResponseCode){
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  Serial.println();
  Serial.println();
}

/**
 * Function to connect to WiFi Network.
 */
void wifiConnect(){
  if (WiFi.status() != WL_CONNECTED)
  {

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(500);
    }
    Serial.println("Successfully connected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
}

/**
 * Function to be called once the WiFi event "SYSTEM_EVENT_STA_DISCONNECTED" occurred. That is, when not being
 * able to connect to a WiFi network. It doesn't block the program flow.
 * @param event a variable of type WiFiEventInfo_t
 * @param info a variable of type WiFiEvent_t
 */
void wiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
}
