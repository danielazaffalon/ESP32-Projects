#include "wifiConnection.h"

// #define MQTT_MAX_PACKET_SIZE 256 //https://docs.losant.com/devices/commands/
const char *WIFI_SSID = SSID;
const char *WIFI_PASS = PASS;
int connectionTimeout = 2000;

//DZ
// Replace with your network credentials
const char* ssid = "HOTSPOT";
const char* password = "";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output26 = pin55;
const int output27 = pin56;

bool wifiConnect(){
  unsigned long startTime = millis();
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("WiFi Status: ");
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  while (WiFi.status() != WL_CONNECTED) {
    printStatus();
    unsigned long actualTime = millis();
    if(actualTime-startTime >= connectionTimeout){
      WiFi.begin(WIFI_SSID, WIFI_PASS);
      startTime = millis();
    }
    delay(100);
  }

  wifiIP = WiFi.localIP();
  lcdSwitchState = 2;
  Serial.print("Wifi Connected with IP: ");
  Serial.println(WiFi.localIP());
  return true;
}

void printStatus(){
  if (WiFi.status() == 255) DEBUG_PRINT Serial.printf("NO_SHIELD\r\n");
  else if (WiFi.status() == 0) DEBUG_PRINT Serial.printf("IDLE_STATUS\r\n");
  else if (WiFi.status() == 1) DEBUG_PRINT Serial.printf("NO_SSID_AVAIL\r\n");
  else if (WiFi.status() == 2) DEBUG_PRINT Serial.printf("SCAN_COMPLETED\r\n");
  else if (WiFi.status() == 3) DEBUG_PRINT Serial.printf("CONNECTED\r\n");
  else if (WiFi.status() == 4) DEBUG_PRINT Serial.printf("CONNECT_FAILED\r\n");
  else if (WiFi.status() == 5) DEBUG_PRINT Serial.printf("CONNECTION_LOST\r\n");
  else if (WiFi.status() == 6) DEBUG_PRINT Serial.printf("DISCONNECTED\r\n");
  // Serial.print('.');
}

void modeAP(){
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  DEBUG_PRINT Serial.printf("AP IP address: \t");
  DEBUG_PRINT Serial.println(IP);
  server.begin();

  while(1){
      WiFiClient client = server.available();   // Listen for incoming clients
      if (client) {                             // If a new client connects,
      DEBUG_PRINT Serial.println("New Client.");          // print a message out in the serial port
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          header += c;
          if (c == '\n') {                    // if the byte is a newline character
            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              
              // turns the GPIOs on and off
              if (header.indexOf("GET /26/on") >= 0) {
                Serial.println("GPIO 26 on");
                output26State = "on";
                digitalWrite(output26, HIGH);
              } else if (header.indexOf("GET /26/off") >= 0) {
                Serial.println("GPIO 26 off");
                output26State = "off";
                digitalWrite(output26, LOW);
              } else if (header.indexOf("GET /27/on") >= 0) {
                Serial.println("GPIO 27 on");
                output27State = "on";
                digitalWrite(output27, HIGH);
              } else if (header.indexOf("GET /27/off") >= 0) {
                Serial.println("GPIO 27 off");
                output27State = "off";
                digitalWrite(output27, LOW);
              }
              
              // Display the HTML web page
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"); //Responsive
              client.println("<link rel=\"icon\" href=\"data:,\">"); //used to prevent requests on the favicon
              // CSS to style the on/off buttons 
              // Feel free to change the background-color and font-size attributes to fit your preferences
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
              client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
              client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
              client.println(".button2 {background-color: #555555;}</style></head>");
              
              // Web Page Heading
              client.println("<body><h1>WEB SERVER</h1>");
              
              // Display current state, and ON/OFF buttons for GPIO 26  
              client.println("<p>GPIO 26 - State " + output26State + "</p>");
              // If the output26State is off, it displays the ON button       
              if (output26State=="off") {
                client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
              } else {
                client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
              } 
                
              // Display current state, and ON/OFF buttons for GPIO 27  
              client.println("<p>GPIO 27 - State " + output27State + "</p>");
              // If the output27State is off, it displays the ON button       
              if (output27State=="off") {
                client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
              } else {
                client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
              }
              client.println("</body></html>");
              
              // The HTTP response ends with another blank line
              client.println();
              // Break out of the while loop
              break;
            } else { // if you got a newline, then clear currentLine
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }
        }
      }
      // Clear the header variable
      header = "";
      // Close the connection
      client.stop();
      DEBUG_PRINT Serial.printf("Client disconnected.\r\n");
    }
  }
}