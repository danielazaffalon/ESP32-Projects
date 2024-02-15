/**
  Mutex example

  Example to show how to use Mutex in order to share resources between cores and avoid
  undefined or bad behaviour.
  
  To use it, just upload this code to a ESP32 and open Serial Monitor. Using Mutex, you will see that
  Shared resource is accessed one by one between cores. In Serial Monitor it will be seen how Core 0 and Core 1 alternate
  to increment a shared variable. One at a time in a controlled manner. In some cases, it is possible to see that a Core
  did the task twice before giving control to the other Core. That is not true, the reason a given Core occasionally prints twice is 
  because Serial.print() internally has an other Mutex. Just follow the shared variable value in the Serial Monitor
  to see the real sequence of the events.

  If Serial Monitor shows random characters, check Baudrate or reset microcontroller.
  
  To see the how it behaves without Mutex control, comment all lines that have the following comment:  ###MUTEX###


  This example was developed following Espressif's documentation. It can be found in the following URL: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#semaphore-api
  ESP32 uses a modified version based on Vanilla FreeRTOS v10.4.3 as seen in the previous URL. To see the official FreeRTOS documentation visit this URL: https://www.freertos.org/a00112.html  
*/

TaskHandle_t Task2;               //Variable that handles Core 0 task
SemaphoreHandle_t testMutex;      //Variable that handles Mutex semaphore.

unsigned long sharedResource = 0;   //Resource that is shared by Core 0 and Core 1. In this case a variable.


void Task2code(void *pvParameters){                                     //Define function that will run in Core 0
  while(1){
    if(xSemaphoreTake(testMutex, (TickType_t)50) == pdTRUE){            //Take mutex, if it is already taken, try taking it for 50ms.         ###MUTEX###
      unsigned long takenValue0 = sharedResource;                       //Use shared resource.
      sharedResource++;                                                 //Use shared resource.
      unsigned long givenValue0 = sharedResource;                       //Use shared resource.
      xSemaphoreGive(testMutex);                                        //Give mutex to unlock this resource.                                 ###MUTEX###
      
      Serial.print("Core 0: TAKEN: ");                                  //Print value of sharedResource before and after using it (increment its value).
      Serial.print(takenValue0);
      Serial.print("  GIVEN: ");
      Serial.println(givenValue0);
    }                                                                   //###MUTEX##  
    else{                                                               //If mutex NOT obtained after 50ms it times out and executes this block of code.    //###MUTEX###
      Serial.println("Core 0: Mutex NOT obtained!");                    //###MUTEX###
    }                                                                   //###MUTEX###
  }
}


void setup() {
  Serial.begin(115200);
  delay(3000);
  testMutex = xSemaphoreCreateMutex();                                          //Create a mutex semaphore and assign it to testMutex variable of type (SemaphoreHandle_t).
  xTaskCreatePinnedToCore(Task2code, "Task2", 10000, NULL, 1, &Task2, 0);       //Create task to be executed in core 0.
}


void loop() {
  if(xSemaphoreTake(testMutex, (TickType_t)50) == pdTRUE){      //Take mutex, if it is already taken, try taking it for 50ms.     ###MUTEX###
    unsigned long takenValue1 = sharedResource;                 //Use shared resource.
    sharedResource++;                                           //Use shared resource.
    unsigned long givenValue1 = sharedResource;                 //Use shared resource.
    //delay(100);                                               //Uncomment this line to see that the other core is not able to lock because it times out.
    xSemaphoreGive(testMutex);                                  //Give mutex to unlock this resource.                              ###MUTEX###
  
    Serial.print("Core 1: TAKEN: ");                            //Print value of sharedResource before and after using it (increment its value).
    Serial.print(takenValue1);
    Serial.print("  GIVEN: ");
    Serial.println(givenValue1); 
  }                                                             //###MUTEX###                                                       
  else{                                                         //If mutex NOT obtained after 50ms it times out and executes this block of code.    //###MUTEX###
    Serial.println("Core 1: Mutex NOT obtained!");              //###MUTEX###
  }                                                             //###MUTEX###
}
