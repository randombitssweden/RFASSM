This is a base module that was built as a part of a sensor network. Currently 
the modules only does the following: 

Connects to a wifi network
Starts a https server on port 443 to show status and info
Uses a green, yellow and red led to indicate status. 

LED Status:
Constant Green - ALL OK
Constant Yellow and blinking RED - Wifi connection error
Constant Yellow - Waiting for warmup
ALL Lights flashing quickly - Edit rfassm.h before compile.

Functions in progress:

CryptoSigned status for device
Send data to central server
