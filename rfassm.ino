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
  uint8_t myID[16];
  bool firstRun;
bool usePIR;
bool useMQ;
bool useDHT;
bool useMaster;
bool useStealth;
bool htmlClientFail = 0;
bool tamperAlarm;
bool useLockdown;
bool useLPD;
bool useGSM;
bool useBlueTooth;
bool useDebug;

  int e_addr = 0;
  char masterURL[256];
  char serverCert[1024];
  char serverKey[1024];
  char aesKey[128];
  char systemAdmin[8];
  char systemPass[16];

  char wifissid[8];
  char wifipass[16];

  String maker;

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
    while (!Serial) {
      // FIXME Error blink
    }
  }

  EEPROM.begin(512);
  EEPROM.get(e_addr, myID);
  e_addr += sizeof(myID);
  EEPROM.get(e_addr, maker);
  e_addr += sizeof(maker);
  EEPROM.get(e_addr, firstRun);
  e_addr += sizeof(firstRun);
  EEPROM.get(e_addr, usePIR);
  e_addr += sizeof(usePIR);
  EEPROM.get(e_addr, useMQ);
  e_addr += sizeof(useMQ);
  EEPROM.get(e_addr, useDHT);
  e_addr += sizeof(useDHT);
  EEPROM.get(e_addr, useMaster);
  e_addr += sizeof(useMaster);
  EEPROM.get(e_addr, useStealth);
  e_addr += sizeof(useStealth);
  EEPROM.get(e_addr, tamperAlarm);
  e_addr += sizeof(tamperAlarm);
  EEPROM.get(e_addr, useLockdown);
  e_addr += sizeof(useLockdown);
  EEPROM.get(e_addr, useLPD);
  e_addr += sizeof(useLPD);
  EEPROM.get(e_addr, useGSM);
  e_addr += sizeof(useGSM);
  EEPROM.get(e_addr, useBlueTooth);
  e_addr += sizeof(useBlueTooth);
  EEPROM.get(e_addr, useDebug);
  e_addr += sizeof(useDebug);
  EEPROM.get(e_addr, wifissid);
  e_addr += sizeof(wifissid);
  EEPROM.get(e_addr, wifipass);
  e_addr += sizeof(wifipass);
  EEPROM.get(e_addr, serverURL);
  e_addr += sizeof(serverURL);
  EEPROM.get(e_addr, aes_pass);
  e_addr += sizeof(aes_pass);
  EEPROM.get(e_addr, fingerprint);
  e_addr += sizeof(fingerprint);
  EEPROM.end();
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
  if (useGSM) {
  }
}

void loop() {


}
