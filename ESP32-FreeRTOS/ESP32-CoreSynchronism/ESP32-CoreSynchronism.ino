
/**
 * @file ESP32-CoreSynchronism.ino
 *
 * @brief This sketch uses two cores. Core 1 -> loop is used to blink a LED through
 *  a duty cycle with duty cycle percentage and total cycle time as a variables.
 * The Core 0 -> Task1 is where the user can update the duty cycle and the total cycle 
 * time of the blinked LED. This Core is always communicating through the serial port.
 *
 * @author Daniela Zaffalon. 2022.
 */

TaskHandle_t Task1;
bool executeOnce = true;
bool ledState;
const uint32_t outputPin = pin57;
float dutyCycle = 0.5;
int totalCycle = 1000;
unsigned long pastTime = 0;

void setup()
{
    Serial.begin(115200);
    pinMode(outputPin, OUTPUT);
    delay(5000);
    Serial.printf("Setup core:\t%u\t Setup priority:\t%u\r\n",xPortGetCoreID(),uxTaskPriorityGet(NULL));

    xTaskCreatePinnedToCore(
      Task1code, /* Function to implement the task */
      "Task1", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      2,  /* Priority of the task */
      &Task1,  /* Task handle. */
      0); /* Core where the task should run */ 

      pastTime = millis();
}

void Task1code( void * parameter){
    Serial.printf("Task1 core:\t%u\t Task1 priority:\t%u\r\n\r\n\r\n",xPortGetCoreID(),uxTaskPriorityGet(NULL));
    Serial.print("\r\nThe default cycle is 1000ms with duty cycle of 50 %.\r\n\r\n");
    for(;;){
        
        int dutyCycleTemp;
        int totalCycleTemp;
        String input="";

        Serial.print("Please write the Total Cycle Time and press Enter:\t\t");
        while(!Serial.available()) delay(1);
        while (Serial.available()) {
            input = Serial.readStringUntil('\n');
            // input += (char)Serial.read();
        }
        
        totalCycleTemp = input.toInt();
        // totalCycle = input.toInt(); //Changing in the middle of the process

        input="";
        Serial.printf("%i\r\n\r\n", totalCycleTemp);
        delay(1000);

        Serial.print("Please write the Duty Cycle in percentage and press Enter:\t\t");
        while(!Serial.available()) delay(1); // if there is data comming
        while (Serial.available()) {
            input = Serial.readStringUntil('\n');
            // input += (char)Serial.read();
        }
        dutyCycleTemp = input.toInt();
        Serial.printf("%i %\r\n\r\n", dutyCycleTemp);
        delay(2000);

        totalCycle = totalCycleTemp;
        dutyCycle = ((float)dutyCycleTemp/100);
        Serial.println("Variables have been updated\r\n\r\n\r\n");
        delay(5000);
    }
}

void loop()
{
    if(executeOnce){
        Serial.printf("Loop core:\t%u\t Loop priority:\t%u\r\n",xPortGetCoreID(),uxTaskPriorityGet(NULL));
        executeOnce = false;
    } 
    unsigned long currTime = millis();
    if(currTime-pastTime < (totalCycle*dutyCycle)) digitalWrite(outputPin, HIGH);
    else if (currTime-pastTime < (totalCycle)) digitalWrite(outputPin, LOW);
    else pastTime = currTime;
}