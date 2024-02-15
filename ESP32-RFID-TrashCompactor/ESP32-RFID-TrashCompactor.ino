
/**
 * @file ESP32-WasteProcessing.ino
 *
 * @brief This sketch 
 *
 * @author Daniela Zaffalon.
 */

#include <SPI.h>
#include <MFRC522.h>
#include "src/wifiConnection/wifiConnection.h"

extern int actualUserID;
extern User users[2]; // A8= tarjeta

MFRC522 mfrc522(CS, MF_0); // Create MFRC522 instance

void obtainUserID(byte* buffer, byte bufferSize, byte* userID);
bool validateID(byte* id);
// Inputs
static const uint32_t startButton = pin25;
static const uint32_t stopButton = pin26;
static const uint32_t doorSensor = pin27;
static const uint32_t fullSensor = pin28;
static const uint32_t loadCell = pin18; // Analog Input
// Outputs
static const uint32_t yellowLed = pin45; //
static const uint32_t greenLed = pin55; 
static const uint32_t redLed = pin56;
static const uint32_t doorLock = pin57;
static const uint32_t motor = pin58;

unsigned long startTime;
bool startButtonState = false;
bool stopButtonState = false;
bool fullButtonPressed = false;
unsigned long delayTime = 0;
int loginTimeout = 50000;
int processTimeout = 10000;
int processSwitchState = 0;

byte userID[] = {0x00,0x00,0x00,0x00};

void startISR() {
   if(processSwitchState == 2 || processSwitchState == 3 || processSwitchState == 4) startButtonState = true;
}

void stopISR() {
   if(4 <= processSwitchState <= 6) {
        stopButtonState = true;
        processSwitchState = 8;
   } 
}

void fullISR() {
    fullButtonPressed = true;
    processSwitchState = 11;
}

void setup(){
    Serial.begin(115200);
    createTask();
    lcdSwitchState = 1;
    processSwitchState = 0;
    delay(5000);
    if(digitalRead(startButton) && digitalRead(stopButton)) {
        DEBUG_PRINT Serial.printf("HotSpot Mode ON\r\n");
        modeAP();
        // while(1){}
    }
    pinMode(yellowLed, OUTPUT);
    pinMode(motor, OUTPUT);
    digitalWrite(greenLed,LOW);
    digitalWrite(redLed,LOW);
    digitalWrite(yellowLed,LOW);
    digitalWrite(doorLock,HIGH); //Door locked
    attachInterrupt(startButton, startISR, RISING);
    attachInterrupt(stopButton, stopISR, RISING);
    attachInterrupt(fullSensor, fullISR, CHANGE);
    SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
}

void loop(){
    unsigned long actualTime = millis();
    bool doorSensorState = digitalRead(doorSensor);
    int loadCellValue = analogRead(loadCell);
    bool fullButtonState = digitalRead(fullSensor);
    if(WiFi.status() != WL_CONNECTED) wifiConnect();
	
    switch (processSwitchState){
        case 0: // Waiting for RFID CARD
            digitalWrite(redLed,LOW);
            digitalWrite(greenLed,LOW);
            digitalWrite(yellowLed,LOW);
            if(mfrc522.PICC_IsNewCardPresent()) {
                DEBUG_PRINT Serial.printf("Please wait...\r\n");
                lcdSwitchState = 4; // MSG: Please Wait
                processSwitchState = 1;
            }
        break;
        case 1: // RFID Validation
            if (mfrc522.PICC_ReadCardSerial() ) { //&& validateID()
                obtainUserID(mfrc522.uid.uidByte, mfrc522.uid.size, userID);  // Motrar el UID
                delay(2000);
                if(validateID(userID[0])) {
                    DEBUG_PRINT Serial.printf("Login User ID: %02X %02X %02X %02X\r\n",userID[0],userID[1],userID[2],userID[3]);
                    DEBUG_PRINT Serial.printf("Hi %s %s, you already have %i points\r\n",users[actualUserID].getName(),users[actualUserID].getLastName(),users[actualUserID].getPoints());
                    processSwitchState = 3;
                    lcdSwitchState = 5; // MSG: Hi User Name
                    // if(mfrc522.PICC_IsNewCardPresent() || mfrc522.PICC_ReadCardSerial()) lcdSwitchState = 5; // MSG: Hi User Name
                }
                else {
                    DEBUG_PRINT Serial.printf("Invalid User\r\n");
                    lcdSwitchState = 6; // MSG: Invalid User
                    digitalWrite(redLed,HIGH);
                    processSwitchState = 2;
                }
            }
            else {
                processSwitchState = 0;
                DEBUG_PRINT Serial.printf("Welcome to W.P.T.\r\n");
                lcdSwitchState = 3;  // MSG: Wellcome
            }
            break;
        case 2: //Invalid User Delay before return to wait for RFID Card
            if(startButtonState){
                digitalWrite(redLed,LOW);
                DEBUG_PRINT Serial.printf("Welcome to W.P.T.\r\n");
                lcdSwitchState = 3; // MSG: Wellcome
                processSwitchState = 0;
            }
            break;
        case 3: //Valid User. Unlook the door. Start Timeout counter
            delay(5000);
            digitalWrite(doorLock,LOW);
            digitalWrite(greenLed,HIGH);
            DEBUG_PRINT Serial.printf("Door Unlocked, Press Start\r\n");
            lcdSwitchState = 7; // MSG: Door unlocked, please press start button
            processSwitchState = 4;
            startTime = millis();
            break;
        case 4:  // Valid User unlock the door now Waiting for door close && start button
            if(doorSensorState && startButtonState){
                DEBUG_PRINT Serial.printf("WEIGH START\r\n");
                loadCellVoltage = loadCellValue*10.0f/4095;
                DEBUG_PRINT Serial.printf("Weigh: %.2f \r\n",loadCellVoltage);
                DEBUG_PRINT Serial.printf("Points: %i \r\n",(int)loadCellVoltage);
                lcdSwitchState = 8; // MSG: PROCESSING
                processSwitchState = 5;
                digitalWrite(greenLed,LOW);
                delayTime = 3000;
                digitalWrite(doorLock,HIGH);
                delayTime = 3000;
                digitalWrite(yellowLed,HIGH);
                startTime = millis();
                startButtonState = false;
            }
            else if(actualTime-startTime >= loginTimeout){
                digitalWrite(doorLock,HIGH);
                DEBUG_PRINT Serial.printf("Login Timeout\r\n");
                digitalWrite(greenLed,LOW);
                digitalWrite(redLed,HIGH);
                lcdSwitchState = 9; // MSG: Login Timeout
                delay(2000);
                startTime = millis();
                DEBUG_PRINT Serial.printf("Welcome to W.P.T.\r\n");
                digitalWrite(redLed,LOW);
                lcdSwitchState = 3; // Return to start MSG: Wellcome
                processSwitchState = 0;
            }
            else if (!doorSensorState && startButtonState){
                DEBUG_PRINT Serial.printf("First close Door & Press Start\r\n");
                lcdSwitchState = 10; // MSG: Close the door
                startButtonState = false;
            }  
            break;
        case 5: // The door is close and start button pushed. Timeout to weighing
            if(actualTime-startTime >= delayTime){
                processSwitchState = 6;
                digitalWrite(motor, HIGH);
                startTime = millis();
                DEBUG_PRINT Serial.printf("PROCESSING\r\n");
                lcdSwitchState = 11; // MSG: PROCESSING ..Please Wait...
            }
            break;
        case 6: // DOING THE PROCESS
            if(actualTime-startTime >= processTimeout){
                digitalWrite(motor, LOW);
                digitalWrite(yellowLed,LOW);
                processSwitchState = 7;
                users[actualUserID].addPoints((int)loadCellVoltage);
                DEBUG_PRINT Serial.printf("Process END with %i new points, total %i points\r\n", (int)loadCellVoltage, users[actualUserID].getPoints());
                lcdSwitchState = 12; // MSG: END + Peso + Puntos
            }
            break;
        case 7: // Timeout to show the end process summary and start again
            delay(5000);
            // waiting for csv, local and cloud update
            // Save the points in the card and read- from it
            processSwitchState = 0;
            DEBUG_PRINT Serial.printf("Welcome to W.P.T.\r\n");
            lcdSwitchState = 3; // Return to start MSG: Wellcome
            break;
        case 8: // STOP BUTTON PRESSED
            DEBUG_PRINT Serial.printf("EMERGENCY STOP BUTTON PRESSED\r\n");
            digitalWrite(greenLed,LOW);
            digitalWrite(yellowLed,LOW);
            digitalWrite(redLed,HIGH);
            digitalWrite(motor, LOW);
            digitalWrite(doorLock,LOW); // Unlock the door
            stopButtonState = false; 
            lcdSwitchState = 13; // EMERGENCY STOP BUTTON PRESSED
            processSwitchState = 9;
            delayTime = 5000;
            startTime = millis();
            break;
        case 9: // STOP BUTTON PRESSED -> waiting to see the message.
            if(actualTime-startTime >= delayTime){
                lcdSwitchState = 14; // EMERGENCY STOP BUTTON PRESSED
                processSwitchState = 10;
            }            
        break;
        case 10: // STOP BUTTON PRESSED -> TO Unlock has to open the door.
            if(doorSensorState){ //Falta dar un tiempo o una instruccion para reanudadr, si no al estar cerrada la puerta inicia nuevamente
                digitalWrite(doorLock,HIGH); // Unlock the door
                digitalWrite(redLed,LOW);
                processSwitchState = 0;
                lcdSwitchState = 3; // EMERGENCY STOP BUTTON PRESSED
            }
        break;
        case 11: // CONTAINER FULL 
            if(fullButtonState && fullButtonPressed){
                DEBUG_PRINT Serial.printf("fullButtonPressed %s\r\n", fullButtonState ? "true" : "false");
                lcdSwitchState = 15;
                fullButtonPressed = false; 
                digitalWrite(redLed,LOW);
                digitalWrite(greenLed,LOW);
                digitalWrite(yellowLed,HIGH);
                digitalWrite(doorLock,HIGH);
                digitalWrite(motor,LOW);
            }
            else if(!fullButtonState && fullButtonPressed){
                DEBUG_PRINT Serial.printf("fullButtonPressed %s\r\n", fullButtonState ? "true" : "false");
                fullButtonPressed = false;
                processSwitchState = 0;
                lcdSwitchState = 3;
            }
            else {}//Do nothing just wait
        break;
        default:
            break;
    }
}

void obtainUserID(byte* buffer, byte bufferSize, byte* userID) {
  for (byte i = 0; i < bufferSize; i++) {
    userID[i] = buffer[i];
    // Serial.printf("%02x ",userID[i]); //print hexadecimal
  }
}

bool validateID(byte id){
    for (byte i = 0; i < sizeof(users); i++) {
        if(users[i].getID() == id) {
            actualUserID = i;
            return true;
        }
    }
    return false;
}
