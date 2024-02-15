/**
 * @file ESP32-HeapMemory.ino
 *
 * @brief This sketch measures the Heap and RAM Memory of the ESP32
 * https://thingpulse.com/esp32-how-to-use-psram/
 * https://support.arduino.cc/hc/en-us/articles/360013825179-Reduce-the-size-and-memory-usage-of-your-sketch
 *
 * @author Daniela Zaffalon.
 */

void setup(){
  Serial.begin(115200);
  delay(5000);
  Serial.println();
  Serial.println();
}
void loop() {
  Serial.printf("\tChip Model:\t\t %s\t\t", ESP.getChipModel());
  Serial.println();
  Serial.printf("\tChip Cores:\t\t %u\t\t", ESP.getChipCores());
  Serial.println();
  Serial.printf("\tSketch Size:\t\t %u\t\t", ESP.getSketchSize());
  Serial.println();
  Serial.printf("\tTotal Heap Memory Size:\t\t %u\t\t", ESP.getHeapSize());
  Serial.println();
  Serial.printf("\tHeap Memory Available:\t\t %u\t\t", ESP.getFreeHeap());
  Serial.println();
  Serial.printf("\tTotal PSRAM Memory:\t\t %u\t\t", ESP.getPsramSize());
  Serial.println();
  Serial.printf("\tFree PSRAM Memory:\t\t %u\t\t", ESP.getFreePsram());
  Serial.println();
  Serial.println();
  delay(5000);
}