#include <EEPROM.h>

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
//#include "functions.h"


void setup() {
  int e_addr = 0;
  char masterURL[256];
  char serverCert[1024];
  char serverKey[1024];
  char aesKey[128];
  char systemAdmin[8];
  char systemPass[16];

  char wifissid[8];
  char wifipass[16];
  
  int tempVal = 0;
  int htmlClientFailLog = 0;
  int myStatus = 0;

  unsigned long htmlClientFailTime;
  unsigned long readyTime;
  unsigned long myTime;
  unsigned long clientTime;
  unsigned long updateTime;
  unsigned long dhtTime;
  unsigned long htmlClientTime;
  unsigned long tamperAlarmTime;

  if (!useLockdown) {
    Serial.begin(115200);
      while(!Serial) {
      // FIXME Error blink
    }
  }
  
  EEPROM.begin(512);
  firstRun = EEPROM.read(0);
  usePIR = EEPROM.read(1);
  useMQ = EEPROM.read(2);
  useDHT = EEPROM.read(3);
  useMaster = EEPROM.read(4);
  useStealth = EEPROM.read(5);
  htmlClientFail = EEPROM.read(6);
  tamperAlarm = EEPROM.read(7);
  useLockdown = EEPROM.read(8);
  useLPD = EEPROM.read(9);
  useGSM = EEPROM.read(10);
  useBlueTooth = EEPROM.read(11);
  for (int i = 13 ; i < 21 ; i++) {
    if (EEPROM.read(i) == NULL) {
      break;
    }
    systemAdmin[i-13] = EEPROM.read(i);
  }
  for (int i = 21 ; i < 37 ; i++) {
    if (EEPROM.read(i) == NULL) {
      break;
    }
    systemPass[i-21] = EEPROM.read(i);
  }
  for (int i = 37 ; i < 45 ; i++) {
    if (EEPROM.read(i) == NULL) {
      break;
    }
    wifissid[i-37] = EEPROM.read(i);
  }
  for (int i = 45; i < 57 ; i++) {
    if (EEPROM.read(i) == NULL) {
      break;
    }
    wifipass[i -45] = EEPROM.read(i);
  }
Serial.printf("[DEBUG] firstRun = %s", firstRun);
Serial.printf("[DEBUG] usePIR = %s", usePIR);
Serial.printf("[DEBUG] useMQ = %s", useMQ);
Serial.printf("[DEBUG] useDHT = %s", useDHT);
Serial.printf("[DEBUG] useMaster = %s", useMaster);
Serial.printf("[DEBUG] useStealth = %s", useStealth);
Serial.printf("[DEBUG] htmlClientFail = %s", htmlClientFail);
Serial.printf("[DEBUG] tamperAlarm = %s", tamperAlarm);
Serial.printf("[DEBUG] useLockdown = %s", useLockdown);
Serial.printf("[DEBUG] useLPD = %s", useLPD);
Serial.printf("[DEBUG] useGSM = %s", useGSM);
Serial.printf("[DEBUG] useBlueTooth = %s", useBlueTooth);
Serial.printf("[DEBUG] useDebug = %s", useDebug);

  /*
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifissid, wifipass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());*/
if (useBlueTooth == 1) {
  // FIXME Bluetooth module
}
if (useMaster) {
}
if (usePIR) {
}
if (useDHT) {
}
if (useStealth) {
}
if (useLPD) {
}
if (useGSM){
}
}

void loop() {

  
}
