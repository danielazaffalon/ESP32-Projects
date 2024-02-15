
/**
 * @file ESP32-LowPassFilter.ino
 *
 * @brief This sketch apply a low pass filter to analog read value.
 *
 * @author Daniela Zaffalon.
 */

int lowPassFilterEMA();
int previousTemperature = 0;
float alpha = 0.15; //0.01;
uint32_t analogInput = pin16;
uint32_t analogOutput = pin45;

void setup(){
    Serial.begin(115200);
    analogWrite(analogOutput,1023);
    delay(10);
    previousTemperature = analogRead(analogInput);
}

void loop(){
	int measure = analogRead(analogInput);
    int filtered = lowPassFilterEMA(measure);
    Serial.println((float(filtered)/4096.0)*100);
}

int lowPassFilterEMA(int value){
   previousTemperature = alpha * value + (1 - alpha) * previousTemperature;
   return previousTemperature;
}