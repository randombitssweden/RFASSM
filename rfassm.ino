
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
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
#include "config.h"
#include "functions.h"

void setup(void){
  Serial.begin(115200);
  Serial.println("");
  dht.begin();
  sensor_t sensor;
  if (useDHT == 1) {
    dht.temperature().getSensor(&sensor);    
  }
  pinMode(S_PIR, INPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ALARM_TRIGGER, INPUT);
  pinMode(LIGHT_SENSOR, INPUT);
  pinMode(MQ_SENSOR, INPUT);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_Y, HIGH);
  digitalWrite(LED_R, HIGH);
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi network: ");
  //Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    if (ssid == "<SSID>") {
      Serial.println("FATAL ERROR! Please edit config.h before compile");
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
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  Serial.print("Loading: ");
  Serial.print("AES256 key, ");
  server.setRSACert(new BearSSL::X509List(serverCert), new BearSSL::PrivateKey(serverKey));
  Serial.print("RSA Cert, ");
  server.on("/", handleRoot);
  server.on("/status/", handleStatus);
  server.on("/status", handleStatus);
  server.on("/alarm", handleAlarm);
  server.on("/alarm/", handleAlarm);
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  Serial.println("html handlers");
  server.begin();
  Serial.println("HTTPS server started");
  
  
  
  timeClient.begin();
  timeClient.update();
  Serial.println("NTP client started");

  myTime = timeClient.getEpochTime();
  clientTime = myTime;
  readyTime = myTime + 47 + random(10,25);
  dhtTime = myTime +5; 
  digitalWrite(LED_G, LOW);
  Serial.printf("HTTPS Client refresh time set to %s seconds.", clientWait);
  Serial.println("----------------------------");
}

void loop(void){
  if (tamperAlarm == 1) {
    Serial.println("[CRITICAL] Tamper Alarm Triggered!");
    // Send hail mary to master server
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint);
    HTTPClient https;
    serverURL += DSID;
    serverURL += "/CRIT/";
    serverURL += myTime;
    https.begin(*client, serverURL);
    while(0) {
      digitalWrite(LED_R, HIGH);
      digitalWrite(LED_Y, HIGH);
      delay(500);
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_Y, LOW);
      delay(250);
    }
  }
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
    Serial.print("[INFO] NTP update\n[INFO] Time is now: ");
    Serial.println(timeClient.getFormattedTime());
    updateTime = timeClient.getEpochTime() + 378 + random(1,600);
  }
  if (useDHT == 1) {
    if (myTime > dhtTime) {
      sensors_event_t event;
      dht.temperature().getEvent(&event);
      dhtTime = myTime +5;
    }
  }
  if (systemReady == 0) {
    if(myTime > readyTime) {
      systemReady = 1;
      myStatus = 200;
      if (stealthMode == 0) {
        digitalWrite(LED_Y, LOW);
        digitalWrite(LED_G, HIGH);  
      }
      Serial.println("[INFO] Starting Sensory Data Networking\n[INFO] RFASSM READY!");

      if (useMaster == 1) {
        if (myTime > clientTime) {
          std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
      
          client->setFingerprint(fingerprint);

          HTTPClient https;

          Serial.print("[HTTPS} client starting at ");
          Serial.println(timeClient.getFormattedTime());
          digitalWrite(LED_Y, HIGH);
          if (htmlClientFailLog > 0) {
            Serial.printf("[HTTPS] Connection has failed %s times and succeeded %s times.\n", htmlClientFailLog, htmlClientSuccess);
            Serial.printf("[HTTPS] Last successfull connection at UNIX timestamp: %s\n", htmlClientTime);
            digitalWrite(LED_R, HIGH);
            delay(200);
            digitalWrite(LED_R, LOW);
            delay(100);
            digitalWrite(LED_R, HIGH);
            delay(200);
            digitalWrite(LED_R, LOW);
          }
          if (https.begin(*client, serverURL)) {  
            Serial.println("[HTTPS] Sending request");
            int httpCode = https.GET();

            if (httpCode > 0) {
              Serial.printf("[HTTPS] GET: %d\n", httpCode);
              if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                String payload = https.getString();
                Serial.println("[HTTPS] OK");
                //Serial.println(payload);
              }
            } else {
              Serial.printf("[HTTPS] GET Failed, error: %s\n", https.errorToString(httpCode).c_str());
            }
            digitalWrite(LED_Y, LOW);
            https.end();
            clientWait = 1200;
            htmlClientSuccess++;
            htmlClientTime = timeClient.getEpochTime();
          } else {
            Serial.printf("[HTTPS] Unable to connect\n");
            htmlClientFail = 1;
          }
          if (htmlClientFail == 1) {
            clientWait = clientWait + 1200;
            htmlClientFail = 0;
            htmlClientFailLog++;
          }
          clientTime = myTime + clientWait;
        }  
      }
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
  myTime = timeClient.getEpochTime();
 }
