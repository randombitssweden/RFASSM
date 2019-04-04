
#define DSID "NMCU0000Z"
#define DVERSION "0.9.1"
#define DURL "http://www.randombits.se"
#define DURL_ALT "www.randombits.se"
#define DMAKER "Random Bits AB"
// Consider that this is designed on a NodeMCU 1.0 (ESP-12E Module)
#define S_PIR D4
#define LED_Y D5
#define LED_G D1
#define LED_R D2
#define DUMP(str, i, buf, sz) { Serial.println(str); \
                               for(i=0; i<(sz); ++i) { if(buf[i]<0x10) Serial.print('0'); Serial.print(char(buf[i]), HEX); } \
                               Serial.println(); } //Help function for printing the Output


ESP8266WiFiMulti WiFiMulti;

aes256_context ctxt;

int myStatus = 406; // Status OK
int val = 0;
int clientWait = 1200;
int htmlClientFail = 0;
int htmlClientFailLog = 0;
int htmlClientSuccess = 0;

unsigned long readyTime = 0;
unsigned long myTime = 0;
unsigned long clientTime = 0;
unsigned long updateTime = 0;
unsigned long systemReady = 0;
unsigned long dhtTime = 0;
unsigned long htmlClientTime = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);


BearSSL::ESP8266WebServerSecure server(443);
