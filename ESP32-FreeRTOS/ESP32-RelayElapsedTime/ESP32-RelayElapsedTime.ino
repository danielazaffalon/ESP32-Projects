
/**
 * @file ESP32-RelayElapsedTime.ino
 *
 * @brief This sketch measures the elapsed time between activating a 
 * digital Output and detecting digital input. The input and output 
 * are connected to each other.
 *
 * @author Daniela Zaffalon.
 */

unsigned long t0;
unsigned long t1;
static uint32_t outputPin = pin55; //pin56;
static uint32_t inputPin = pin26;

void setup()
{
    Serial.begin(115200);
    pinMode(inputPin, INPUT_PULLDOWN);
	pinMode(outputPin, OUTPUT);
    delay(5000);
    digitalWrite(outputPin, LOW);
}

void loop()
{
	t0 = micros();
    digitalWrite(outputPin, HIGH);
    while (! digitalRead(inputPin)){}; //wait until the input detects signal
    t1 = micros();
    Serial.printf("Time:\t%u\tmicroseconds",(t1-t0));
    Serial.println();
    digitalWrite(outputPin, LOW);
    delay(5000); // Test speed -> modifiable
}
//ESP relays time:       Time:	791	    microseconds
//ESP + TRZ 24VDC 2CO:   Time:	7290	microseconds Test Speed 100ms & 50ms 