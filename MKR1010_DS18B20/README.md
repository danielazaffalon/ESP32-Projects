# Arduino Example for DS18B20 Temperaruter Sensor

## Usage

This example support:


* Two sensors DS18B20
* Parasite connection


You will need a pull-up resistor of about 5 KOhm between the 1-Wire data line
and your 5V power. If you are using the DS18B20, ground pins 1 and 3. The
centre pin is the data line '1-wire'.

In case of temperature conversion problems (result is `-85`), strong pull-up setup may be necessary. See section 
_Powering the DS18B20_ in 
[DS18B20 datasheet](https://datasheets.maximintegrated.com/en/ds/DS18B20.pdf) (page 7)
and use `DallasTemperature(OneWire*, uint8_t)` constructor.

please include OneWire from Paul Stoffregen in the library manager before you begin.
please include DallasTemperature from Miles Burton in the library manager before you begin.

## Credits

The OneWire code has been derived from
http://www.arduino.cc/playground/Learning/OneWire.
The DallasTemperature code hasbeen derived from
https://github.com/milesburton/Arduino-Temperature-Control-Library

## Website

Additional documentation may be found here
https://www.milesburton.com/Dallas_Temperature_Control_Library

# License

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
