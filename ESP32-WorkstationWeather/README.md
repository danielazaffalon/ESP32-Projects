# ESP32-Almacen
This sketch connect to Wi-Fi, read from NTC the unix date 
and update the RTC clock each reboot. In the cycle, the sketch read from modbus rtu the temperature and humidity from XY-MD02 Sensor print in LCD Screen and save it in csv Logfile in uSD.
The sketch detect the uSD and when is not, print error message in the screen.

Developed by [Daniela Zaffalon](https://github.com/danielazaffalon)

### Framework
- Developed over Framework version:

### Hardware
- hardware required:
  - [Modbus RTU RS485 SHT20](XY-MD02)
  - [DFR0464](https://wiki.dfrobot.com/Gravity__I2C_16x2_Arduino_LCD_with_RGB_Backlight_Display_SKU__DFR0464)

## Dependencies
  - [RTClib 2.1.1](https://github.com/adafruit/RTClib)
  - [TimeLib 1.6.1](http://playground.arduino.cc/Code/Time/)
  - [NtpClientLib 3.0.2-beta](https://github.com/gmag11/NtpClient)
  - [DFRobot_RGBLCD1602 2.0.0](https://github.com/DFRobot/DFRobot_RGBLCD1602)
  - [Ethernet 3.0.6](https://www.arduino.cc/reference/en/libraries/ethernet/)
  - [ArduinoRS485 1.0.2](http://www.arduino.cc/en/Reference/ArduinoRS485)
  - [ModbusRTUClient 1.0.6](https://www.arduino.cc/en/ArduinoModbus/ArduinoModbus)
  - [FS 2.0.0](https://github.com/espressif/arduino-esp32/tree/master/libraries/FS)
  - [SD 1.2.4](http://www.arduino.cc/en/Reference/SD)

## Usage
- Connect the ethernet cable between the ESP32 and a DHCP network
- Connect the Modbus Temperature/Humidity Sensor to Modbus RTU ESP32 Port and to the power supply
- Connecto LCD 2x16 Screen to I2C ESP32 Port
- Connect the ESP32 to Power Supply
- Introduce a uSD Card into the ESP32 slot.
- Turn on the devices.

