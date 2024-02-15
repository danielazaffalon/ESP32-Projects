
/**
 * @file ESP32-UsedCore.ino
 *
 * @brief This sketch print the ESP32 Core used for runing the setup and loop functions
 *
 * @author Daniela Zaffalon.
 */

void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.print("setup() running on core ");
  Serial.println(xPortGetCoreID());
}

void loop() {
  Serial.print("loop() running on core ");
  Serial.println(xPortGetCoreID());
}
