This is a base module that was built to be a part of a sensor network. 
This is the framework of that module. It is built and developed for 
a NodeMCU 1.0 (ESP-12E) on the Arduino IDE.

Version: (See rfassm.h)

*** YOU MUST EDIT config.h BEFORE YOU COMPILE ***

Currently the modules only does the following: 

* Connects to a wifi network
* Starts a https server on port 443 to show status and info
* Can connect to a server and transmit data with HTTP GET
  Server is verified with SSL Certificate fingerprint.
* Uses a green, yellow and red led to indicate status.

LED Status:
Constant Green - ALL OK
Constant Yellow and blinking RED - Wifi connection error
Constant Yellow - Waiting for warmup
Constant Green and Yellow - Sending/Recieving Client SSL connection.
ALL Lights flashing quickly - Edit config.h before compile.


Functions in progress:

CryptoSigned status 
	Not working due to conflicting char types. 
	the RSA encrypt library function wants data
	as a unsigned char. Program created string is 
	signed char. 


(C) 2018 Random Bits AB, Peter Roosdorp <peter.roosdorp@randombits.se>
