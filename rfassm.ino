#include <Adafruit_Sensor.h>
#include <aes256.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <NTPClient.h>
#include <SoftwareSerial.h>
#include <WiFiClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiUdp.h>

#include "rfassm.h"
#include "config.h"
#include "functions.h"

void setup(){
Serial.begin(115200);

char masterURL[256];
char serverCert[1024];
char serverKey[1024];
char aesKey[128];
char systemAdmin[16];
char systemPass[128];

char wifissid[32];
char wifipassword[64];


int tempVal;
int htmlClientFailLog;


unsigned long htmlClientFailTime;
unsigned long readyTime;
unsigned long myTime;
unsigned long clientTime;
unsigned long updateTime;
unsigned long dhtTime;
unsigned long htmlClientTime;
unsigned long tamperAlarmTime;
}

void loop() {
  // put your main code here, to run repeatedly:

}}
