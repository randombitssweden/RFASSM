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
#define DHTPIN 3
#define DHTTYPE DHT22  
DHT_Unified dht(DHTPIN, DHTTYPE);

void setup(void){
  Serial.begin(115200);
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
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_Y, HIGH);
  digitalWrite(LED_R, HIGH);
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi network: ");
  Serial.println(ssid);
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
  Serial.println("");
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
  oldTime = myTime;
  readyTime = myTime + 47 + random(10,25);
  dhtTime = myTime +5; 
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
      Serial.println("Starting Sensory Data Networking\nRFASSM READY!");
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
