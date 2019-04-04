#include <NTPClient.h>
#include <WiFiUdp.h>

#include <aes256.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "rfassm.h"

#define DSID "NMCU0000Z"
#define DVERSION "0.1"
#define DURL "http://www.randombits.se"
#define DURL_ALT "www.randombits.se"
#define DMAKER "Random Bits AB"
#define S_PIR D1
#define DUMP(str, i, buf, sz) { Serial.println(str); \
                               for(i=0; i<(sz); ++i) { if(buf[i]<0x10) Serial.print('0'); Serial.print(char(buf[i]), HEX); } \
                               Serial.println(); } //Help function for printing the Output

ESP8266WiFiMulti WiFiMulti;

aes256_context ctxt;
int myStatus = 406; // Status OK
int pirReady = 0;
unsigned long myTime = 0;
unsigned long oldTime = 0;
unsigned long updateTime = 0;
unsigned long pirTime = 0;
const char* ssid = STASSID;
const char* password = STAPSK;

int val = 0;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

// Server ssl cert fingerprint
const uint8_t fingerprint[20] = {0x5A, 0xCF, 0xFE, 0xF0, 0xF1, 0xA6, 0xF4, 0x5F, 0xD2, 0x11, 0x11, 0xC6, 0x1D, 0x2F, 0x0E, 0xBC, 0x39, 0x8D, 0x50, 0xE0};

BearSSL::ESP8266WebServerSecure server(443);


const int led = 13;
String myFingerprint() {
  String p_data; //= DSID;
  p_data += timeClient.getEpochTime();
  p_data += "00";
  p_data += millis();
  //uint8_t data[p_data.length()] = ;
  //strcpy(data, p_data);
  //data.toCharArray(p_data, p_data.length());
  //  p_data.toCharArray(data, p_data.length());
  // aes256_encrypt_ecb(&ctxt, p_data);
  return p_data;
}
void handleRoot() {
  String htmlReply = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<title>RFASSM:";
  htmlReply +=DSID;
  htmlReply +="</title>\n<meta charset=\"UTF-8\">\n<meta name=\"keywords\" content=\"HTML,JavaScript\">\n<meta name=\"author\" content=\"(C) 2018 Random Bits AB\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n</head>\n<body>RFASSM ID: ";
  htmlReply += DSID;
  htmlReply += "</br>\nVERSION:";
  htmlReply +=DVERSION;
  htmlReply +="</br>\nSTATUS:";
  htmlReply +=myStatus;
  htmlReply +=myFingerprint();
  htmlReply +="</br>\nGMT TIME:";
  htmlReply +=timeClient.getFormattedTime();
  htmlReply +="\n</body></html>";
  server.send(200, "text/html", htmlReply);
}

void handleAlarm() {
  String htmlReply = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<title>RFASSM:";
  htmlReply +=DSID;
  htmlReply +="</title>\n<meta charset=\"UTF-8\">\n<meta name=\"keywords\" content=\"HTML,JavaScript\">\n<meta name=\"author\" content=\"(C) 2018 Random Bits AB\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n</head>\n<body>ALARM:";
  if(myStatus == 200) {
    htmlReply += "OK";
  } else if (myStatus == 423) {
    htmlReply += "LOCKDOWN";
  } else if (myStatus == 409) {
    htmlReply += "CONFLICT";
  } else if (myStatus == 104) {
    htmlReply += "TRIGGERED";
  } else {
    htmlReply += "PROCESSING";
  }
  htmlReply +="<hr>";
  htmlReply +="</br>\n(c) 2018 Random Bits AB - <a href=\"";
  htmlReply +=DURL;
  htmlReply +="\">";
  htmlReply +=DURL_ALT;
  htmlReply +="\n</body></html>";
  server.send(200, "text/html", htmlReply);
}
void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void){
  Serial.begin(115200);
  pinMode(S_PIR, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.setRSACert(new BearSSL::X509List(serverCert), new BearSSL::PrivateKey(serverKey));

  server.on("/", handleRoot);
  server.on("/alarm", handleAlarm);
  server.on("/alarm/", handleAlarm);
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTPS server started");
  uint8_t key[] = { //
    0x60, 0x67, 0x36, 0x30, 0x47, 0x7b, 0x26, 0x6c, 
    0x40, 0x6d, 0x4f, 0x4a, 0x2c, 0x43, 0x28, 0x24, 
    0x2e, 0x55, 0x23, 0x70, 0x26, 0x5e, 0x78, 0x4c, 
    0x5a, 0x3b, 0x42, 0x36, 0x23, 0x49
  };
  Serial.println("AES256 key loaded.");
  timeClient.begin();
  timeClient.update();
  Serial.println("NTP client started");
  //Serial.print("Current time: ");
  //Serial.println(timeClient.getFormattedTime());
  myTime = timeClient.getEpochTime();
  oldTime = myTime;
  pirTime = myTime + 50 + random(10,25);
  Serial.println(myTime);
  Serial.println(pirTime);
}

void loop(void){
  server.handleClient();
  MDNS.update();
  if (myTime > updateTime) {
    timeClient.update();
    Serial.print("NTP update\nTime is now: ");
    Serial.println(timeClient.getFormattedTime());
    updateTime = timeClient.getEpochTime() + 378 + random(1,600);
  }
  Serial.println(pirReady);
  if (pirReady = 0) {
    if(myTime > pirTime) {
      pirReady = 1;
      myStatus = 200;
      Serial.println("PIR sensor ready");
    }
  } else {
    val = digitalRead(S_PIR);
    if(val == LOW) {
      myStatus = 104;
      digitalWrite(LED_BUILTIN, LOW);
    } else if(val == HIGH) {
      myStatus = 200;
      digitalWrite(LED_BUILTIN, HIGH); 
    } else {
      myStatus = 409;
    }  
  }
  if (myStatus == 104) {
    Serial.println("Alarm Triggered");
  }
  /*
  if (myTime > oldTime) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  
    client->setFingerprint(fingerprint);

    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, "https://jigsaw.w3.org/HTTP/connection.html")) {  // HTTPS

      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
    oldTime = myTime + 600;
  }
  */
  myTime = timeClient.getEpochTime();
  Serial.println(myTime);
  //Serial.println(myTime);
  //Serial.println(updateTime);
  //delay(300);
}
