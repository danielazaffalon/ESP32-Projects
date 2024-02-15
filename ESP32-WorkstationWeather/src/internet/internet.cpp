#include "internet.h"

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};      //User defined MAC

bool internetConnection(){
  Ethernet.init(ETHERNET_CS);  // initialize Ethernet with ESP32' Ethernet Chips Select.
  if (Ethernet.begin(mac) == 0) {
    DEBUG_PRINT Serial.println("Failed to configure Ethernet using DHCP");
    return false;
  }
  else{
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      DEBUG_PRINT Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware!");
      return false;
    }
    else{
      if (Ethernet.linkStatus() == LinkOFF) {
        DEBUG_PRINT Serial.println("Ethernet cable is not connected.");
        return false;
      }
      else{
        DEBUG_PRINT Serial.println("DHCP set correctly.");
        DEBUG_PRINT Serial.print("Assigned IP address: ");
        DEBUG_PRINT Serial.println(Ethernet.localIP());
        IP = Ethernet.localIP();
        return true;     
      }
    }
  }
}