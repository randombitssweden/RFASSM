
#define DSID "NMCU0000Z"
#define DVERSION "0.9.3"
#define DURL "http://www.randombits.se"
#define DURL_ALT "www.randombits.se"
#define DMAKER "Random Bits AB"
// Consider that this is designed on a NodeMCU 1.0 (ESP-12E Module)
#define S_PIR D4
#define LED_Y D5
#define LED_G D1
#define LED_R D2
#define ALARM_TRIGGER D6
#define LIGHT_SENSOR D7
#define MQ_SENSOR A0

#define DUMP(str, i, buf, sz) { Serial.println(str); \
                               for(i=0; i<(sz); ++i) { if(buf[i]<0x10) Serial.print('0'); Serial.print(char(buf[i]), HEX); } \
                               Serial.println(); } //Help function for printing the Output



aes256_context ctxt;


//NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);


BearSSL::ESP8266WebServerSecure server(443);
