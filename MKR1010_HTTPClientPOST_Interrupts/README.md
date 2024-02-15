# MKR1010 Interrupts HTTP Post
Ethernet Test using MKR1010 Device. This sketch measure the processing, 
sending, waiting server response and end connection times of the HTTP communication 
between client and server. In that way, this sketch is a HTTP Client that integrated 
four digital interrupt inputs connected to a button and a LAN Ethernet network connection 
with a HTTP server PC. In this way, when any of the digital inputs is activated, 
 an HTTP POST is sent to a server on the LAN, with the status of the inputs in a JSON object.

Developed by [Daniela Zaffalon](https://github.com/danielazaffalon)

## Dependencies
   - [Ethernet.h 2.0.1](https://github.com/arduino-libraries/Ethernet)

## Usage
- Load the program to the device.
- Select the interruptions pins and connect to buttons.
- Change the server parameters.
- Press the button to send the JSON Program.
