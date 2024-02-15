/**
 * @file ESP32-GPS-CAM-M8Q.ino
 *
 * @brief This sketch read GPS CAM M8Q
 *
 * @author Daniela Zaffalon.
 */

//Tomado de:
//https://github.com/sparkfun/SparkFun_u-blox_GNSS_Arduino_Library/issues?q=is%3Aissue+is%3Aclosed
//https://u-blox.force.com/s/question/0D52p0000AT1PZICQ3/cam-m8q-not-finding-any-satellites
//https://learn.watterott.com/sensors/cam-m8q/


#include "wifiConnection.h"
#include "debug.h"
#include <Wire.h> //Needed for I2C to GNSS

#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS
SFE_UBLOX_GNSS myGNSS;

long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.
unsigned long updateTimer = 5000;

//https://forum.arduino.cc/t/connecting-ublox-gps-cam-m8q-to-arduino-uno/522872/3

void setup(){
	Serial.begin(115200);
	delay(5000);
	Wire.begin();
	// myGNSS.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial

	if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
	{
		DEBUG_PRINT Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
		while (1);
	}

	myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
	myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR
}

void loop(){
	if(WiFi.status() != WL_CONNECTED) wifiConnect();

	//Query module only every second. Doing it more often will just cause I2C traffic.
	//The module only responds when a new position is available
	else if(millis() - lastTime > updateTimer){
		lastTime = millis(); //Update the timer
		long latitude = myGNSS.getLatitude()/10000000;
		long longitude = myGNSS.getLongitude()/10000000;
		long altitude = myGNSS.getAltitude()/1000; //[mm] to [m]
		byte SIV = myGNSS.getSIV();
		int32_t heading = myGNSS.getHeading();
		DEBUG_PRINT Serial.printf("Lat: %6ld [º]\t Long: %6ld [º]\t Alt %6ld [m]\t SIV: %02x \t Heading: %i [º]\r\n",myGNSS.getLatitude(),myGNSS.getLongitude(),myGNSS.getAltitude(),myGNSS.getSIV(),myGNSS.getHeading());
		DEBUG_PRINT Serial.printf("Lat: %6ld [º]\t Long: %6ld [º]\t Alt %6ld [m]\t SIV: %02x \t Heading: %i [º]\r\n",latitude,longitude,altitude,SIV,heading);
		
	}
}

