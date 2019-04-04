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
  pinMode(LED_G, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_Y, HIGH);
  digitalWrite(LED_R, HIGH);
  Serial.print("SSID: ");
/*  if(ssid == "<SSID>") {
    Serial.println("FATAL ERROR: WiFi SSID not changed from default.");
    while(0) {
      delay(5000);
    }
  }*/
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi network: ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    if (ssid == "<SSID>") {
      Serial.println("FATAL ERROR! Please edit rfassm.h before compile");
      digitalWrite(LED_G, HIGH);
      digitalWrite(LED_Y, HIGH);
      digitalWrite(LED_R, HIGH);
      delay(400);
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_Y, LOW);
      digitalWrite(LED_R, LOW);
      delay(400);
    } else {
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_R, LOW);
      //digitalWrite(LED_Y, LOW);
      delay(400);
      digitalWrite(LED_Y, HIGH);
      digitalWrite(LED_R, HIGH);
      delay(600);
    }
  }
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  digitalWrite(LED_R, LOW);
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
  
  Serial.println("AES256 key loaded.");
  
  timeClient.begin();
  timeClient.update();
  Serial.println("NTP client started");

  myTime = timeClient.getEpochTime();
  oldTime = myTime;
  pirTime = myTime + 47 + random(10,25);
  
  digitalWrite(LED_G, LOW);
}

void loop(void){
  if (stealthMode == 1) {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_Y, LOW);
    digitalWrite(LED_R, LOW);
  }
  server.handleClient();
  MDNS.update();
  if (myTime > updateTime) {
    timeClient.update();
    Serial.print("NTP update\nTime is now: ");
    Serial.println(timeClient.getFormattedTime());
    updateTime = timeClient.getEpochTime() + 378 + random(1,600);
  }
  if (pirReady == 0) {
    if(myTime > pirTime) {
      pirReady = 1;
      myStatus = 200;
      if (stealthMode == 0) {
        digitalWrite(LED_Y, LOW);
        digitalWrite(LED_G, HIGH);  
      }
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
 }
