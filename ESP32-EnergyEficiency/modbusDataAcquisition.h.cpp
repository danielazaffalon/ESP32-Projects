#include "modbusDataAcquisition.h"

int slaveAdress = 0x0001;
int temperatureRegister = 0x0001;
int humidityRegister = 0x0002;
int baudRate = 9600;
uint16_t temperatureCorrectionRegister = 0x0103;
uint16_t temperatureCorrectionValue = 0xFFCC; //0xFFD6;
uint16_t humidityCorrectionRegister = 0x0104;
uint16_t humidityCorrectionValue = 0x0064;

/**
 * TEMPERATURE CORRECTION
 * DHT22 = 20,7º && SHT20 = 21,5º
 * SHT20 > ActualMeasure 5,2º. we should subtract 5,2º sending -52 -> turning it into hexa in 2 bytes = 0xFFCC
 * SHT20 > ActualMeasure 4,2º. we should subtract 4,2º sending -42 -> turning it into hexa 2 bytes = 0xFFD6
 * 
 * HUMIDITY CORRECTION
 * DHT22 = 42.10% && SHT20 = 29.60%
 * SHT20 < ActualMeasure 12,5%. we should subtract 12,5 we also have datasheet maximum limit of 10%.
 * Then, we have to send +100 -> turning it into hexa 2 bytes = 0x0064
 */

void modbusInitialization(){
  RS485.setPins(RS485_TX, RS485_DE, RS485_RE);
  if(!ModbusRTUClient.begin(baudRate, SERIAL_8N1)){   //Modbus client configured as 8 data bits, Even parity and 1 stop bit (SERIAL_8E1).
    Serial.println("Modbus Client not initialized!");    
  }
  else Serial.println("Modbus Client successfully initialized");
  // ModbusRTUClient.holdingRegisterWrite(slaveAdress,temperatureCorrectionRegister,temperatureCorrectionValue);
  // ModbusRTUClient.holdingRegisterWrite(slaveAdress,humidityCorrectionRegister,humidityCorrectionValue);
}
void modbusAcquisition(float (&dataArray) [2]){
  float array[2];
  dataArray[0] = float(ModbusRTUClient.inputRegisterRead(slaveAdress, temperatureRegister) / 10.00F);
  dataArray[1] = float(ModbusRTUClient.inputRegisterRead(slaveAdress, humidityRegister) / 10.00F);
  // Serial.println(ModbusRTUClient.holdingRegisterRead(slaveAdress,temperatureCorrectionRegister));
  // Serial.println(ModbusRTUClient.holdingRegisterRead(slaveAdress,humidityCorrectionRegister));
}