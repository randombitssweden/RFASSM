This is a base module that was built to be a part of a sensor network. 
This is the framework of that module. It is built and developed for 
a NodeMCU 1.0 (ESP-12E) on the Arduino IDE.

See WIRING.txt for pinouts and wiring help.

It is referred to as RFASSM
-Radio Frequency Allround Security Sensor Module
______________________________________________________________________

Version: (See rfassm.h)

*** YOU MUST EDIT config.h BEFORE YOU COMPILE ***

Currently the modules only does the following: 

* Connects to a wifi network
* Starts a https server on port 443 to show status and info
* Can connect to a server and transmit data with HTTP GET
  Server is verified with SSL Certificate fingerprint.
	Multiple RFASSM devices can be used to connect to the 
	server.
* Uses a green, yellow and red led to indicate status.
  The lights can be turned off with stealth mode

LED Status:
Constant Green - ALL OK
Constant Yellow and blinking RED - Wifi connection error
Constant Yellow - Waiting for warmup
Constant Green and Yellow - Sending/Recieving Client SSL connection.
Constant Green and Red - Tamper alarm has been tripped.
ALL Lights flashing quickly - Edit config.h before compile.


Functions in progress:

CryptoSigned status message
	Not working due to conflicting char types. 
	the RSA encrypt library function wants data
	as a unsigned char. Program created string is 
	signed char. 

DHT Sensor
	In progress, perhaps working. Not tested with hardware.

MQ Sensor
	In progress, perhaps working. Not tested with hardware.

Stealth Mode
	In progress, control via https//<device>/stealth/[1|0] 

Ambient Light Sensor
	Detect changes in ambient light and report light levels.

Tamper Sensor
	Detect if device case is tampered with. 
______________________________________________________________________
(C) 2018 Random Bits AB, Peter Roosdorp <peter.roosdorp@randombits.se>
