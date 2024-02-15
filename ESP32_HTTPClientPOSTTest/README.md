# ESP32_HTTPClientPOSTTest

This sketch measure the processing, sending, waiting server response and end connection times of the HTTP communication between client and server.

Developed by [Daniela Zaffalon](https://github.com/danielazaffalon)

## Dependencies

### Framework
- Developed over Framework version:
  - [esp32:esp32:nodemcu-32s](https://github.com/espressif/arduino-esp32)

### Hardware
- hardware required:

### Software
  - [WiFi](https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/src/WiFi.h)
  - [HTTPClient] (https://github.com/espressif/arduino-esp32/blob/master/libraries/HTTPClient/src/HTTPClient.h)
  - [time] (https://github.com/espressif/arduino-esp32)

  
## Usage

Install buttons on pins 34,35,32 and 33 with pullup resistors.
Make sure you have an active Wi-Fi network and enter the credentials in secrets.h: #define SSID & #define PASS.
Make sure you have a server within the same Wi-Fi network that receives HTTP Post and enter the server IP in the code IPAddress serverAddress(192,168,1,35).
Press any button to send the HTTP Post with the input status to the server.