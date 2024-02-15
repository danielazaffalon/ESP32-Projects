/**
 * @file EthernetClientPostTest.ino
 *
 * @brief This sketch measure the processing, sending, waiting server 
 * response and end connection times of the HTTP communication between 
 * client and server. In that way, this sketch is a HTTP Client that 
 * integrated four digital interrupt inputs connected to a button and 
 * a LAN Ethernet network connection with a HTTP server PC. 
 * In this way, when any of the digital inputs is activated, 
 * an HTTP POST is sent to a server on the LAN, with the status of the 
 * inputs in a JSON object.
 *
 * @author Daniela Zaffalon.
 */
#include <Arduino.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};      //User defined MAC

// Varaibles For Static IP
IPAddress ip(192, 168, 1, 10);
IPAddress myDns(8, 8, 8, 8);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

EthernetClient client;

// Buttons for just the 26 pin works as a interrupt
const int button1 = pin25; // No Interruption Pin
const int button2 = pin26; // Interruption Pin
const int button3 = pin27; // No Interruption Pin
const int button4 = pin28; // No Interruption Pin

bool buttonPressed = false;
bool printResults = 0;
unsigned long t0 = 0, t1 = 0, t2 = 0, t3 = 0, t4 = 0;
unsigned long elapseTime = 0;
int inputsArray[4];

// Auxiliar functions declaration
void ethernetStaticIPConnect();
void printError();

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
  while(!Serial){}
  
  // ethernetDHCPConnect();
  ethernetStaticIPConnect();
}

void loop(){
  if (buttonPressed) {
    t0 = millis(); //Starting Captured Time
    char postData[25];
    sprintf(postData, "{\"inputsArray\":[%i,%i,%i,%i]}\0", inputsArray[0],inputsArray[1],inputsArray[2],inputsArray[3]);
    
    t1 = millis(); //Captured time after data creation

     // Send HTTP POST request to LAN server with IP
    if (client.connect("192.168.1.80",3000)){ 
      client.println("POST / HTTP/1.1");
      client.println("Host: 192.168.1.80:3000");
      client.println("Content-Type: application/json");// application/x-www-form-urlencoded
      client.print("Content-Length: ");
      client.println(sizeof(postData));
      client.println();          
      client.println(postData);
      client.println();                                       
    }
    else{
      Serial.println("Cannot connect to Server");
      client.stop();         
    }
    t2 = millis();  //Captured time after HTTP Post and received Response

    char serverResponse = client.read();
    while(serverResponse == 255) {
      serverResponse = client.read();
    }
    Serial.print("HTTP Response code: ");
    while(serverResponse != 255) {
      // Serial.print(serverResponse);
      serverResponse = client.read();
    }
    // Serial.println();

    t3 = millis();   //Captured time after wait for response and read the response

    buttonPressed = false;
    printResults = true;
    for(int i=0; i<=3; i++){
      inputsArray[i]=0;
    }

    t4 = millis();   //Captured time after clean variables
  }
  else if(printResults){
     elapseTime = (t1-t0);
     Serial.print("Data Processing Time: ");
     Serial.print(elapseTime);
     Serial.println(" ms");

     elapseTime = (t2-t1);
     Serial.print("HTTP Post create and Send: ");
     Serial.print(elapseTime);
     Serial.println(" ms");

     elapseTime = (t3-t2);
     Serial.print("Waiting and capture the response: ");
     Serial.print(elapseTime);
     Serial.println(" ms");

     elapseTime = (t4-t3);
     Serial.print("Ending HTTP Communication and clean variables: ");
     Serial.print(elapseTime);
     Serial.println(" ms");

     elapseTime = (t4-t0);
     Serial.print("Total Time: ");
    Serial.print(elapseTime);
     Serial.println(" ms");
     Serial.println();
     printResults = false;
   }
   else{
    // wait for an interruption action
   }
}

/**
 * Function to connect to Ethernet Network with static IP.
 */
void ethernetStaticIPConnect(){
  Ethernet.init(ETHERNET_CS);  // initialize Ethernet with ESP32' Ethernet Chips Select.
  if (Ethernet.begin(mac, ip, myDns, gateway, subnet) == 0) Serial.println("Failed to configure Ethernet using DHCP");
  else Serial.println("DHCP set correctly. ");

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware!");
    while(true);
  }
  else delay(2000);
  
  if (Ethernet.linkStatus() == LinkOFF) Serial.println("Ethernet cable is not connected.");
  
  Serial.print("Assigned IP address: ");
  Serial.println(Ethernet.localIP());
  
  Serial.println();
}