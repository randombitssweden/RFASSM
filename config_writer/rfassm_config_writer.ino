// RFASSM Basic configuration setup.

#include <EEPROM.h>
void setup() {
  Serial.begin(115200);
  uint8_t idTemp[16];
  String makerTemp;
  int addr;
  uint8_t myID[16] = { 0x39, 0x30, 0xe8, 0x93, 0x0b, 0xc4, 0x36, 0x22, 0x5d, 0x18, 0x57, 0xe8, 0x72, 0x8e, 0x27, 0xe6 };
  String maker = "Random Bits AB";
  bool firstRun = 0;
  bool usePIR = 0;
  bool useMQ = 0;
  bool useDHT = 0;
  bool useMaster = 1;
  bool useStealth = 0;
  bool tamperAlarm = 0;
  bool useLockdown = 0;
  bool useLPD = 0;
  bool useGSM = 0;
  bool useBlueTooth = 0;
  bool useDebug = 1;
  char mySSID[] = "my wifi";
  char myWIFIPass[] = "my wifi password";
  char serverURL[] = "master.server.hostname";

  // Plaintext password = "SedceT5Yub9aKf7zCp3bceELYphYVNVM"
  uint8_t aes_pass[32] = {
    0x53, 0x65, 0x64, 0x63, 0x65, 0x54, 0x35, 0x59,
    0x75, 0x62, 0x39, 0x61, 0x4b, 0x66, 0x37, 0x7a,
    0x43, 0x70, 0x33, 0x62, 0x63, 0x65, 0x45, 0x4c,
    0x59, 0x70, 0x68, 0x59, 0x56, 0x4e, 0x56, 0x4d
  };
  uint8_t fingerprint[20] = {0x5A, 0xCF, 0xFE, 0xF0, 0xF1, 0xA6, 0xF4, 0x5F, 0xD2, 0x11, 0x11, 0xC6, 0x1D, 0x2F, 0x0E, 0xBC, 0x39, 0x8D, 0x50, 0xE0};
  char myUser[] = "admin";
  char myPass[] = "password";
  EEPROM.begin(512);
  EEPROM.put(addr, myID);
  addr += sizeof(myID);
  EEPROM.put(addr, maker);
  addr += sizeof(maker);
  EEPROM.put(addr, firstRun);
  addr += sizeof(firstRun);
  EEPROM.put(addr, usePIR);
  addr += sizeof(usePIR);
  EEPROM.put(addr, useMQ);
  addr += sizeof(useMQ);
  EEPROM.put(addr, useDHT);
  addr += sizeof(useDHT);
  EEPROM.put(addr, useMaster);
  addr += sizeof(useMaster);
  EEPROM.put(addr, useStealth);
  addr += sizeof(useStealth);
  EEPROM.put(addr, tamperAlarm);
  addr += sizeof(tamperAlarm);
  EEPROM.put(addr, useLockdown);
  addr += sizeof(useLockdown);
  EEPROM.put(addr, useLPD);
  addr += sizeof(useLPD);
  EEPROM.put(addr, useGSM);
  addr += sizeof(useGSM);
  EEPROM.put(addr, useBlueTooth);
  addr += sizeof(useBlueTooth);
  EEPROM.put(addr, useDebug);
  addr += sizeof(useDebug);
  EEPROM.put(addr, mySSID);
  addr += sizeof(mySSID);
  EEPROM.put(addr, myWIFIPass);
  addr += sizeof(myWIFIPass);
  EEPROM.put(addr, serverURL);
  addr += sizeof(serverURL);
  EEPROM.put(addr, aes_pass);
  addr += sizeof(aes_pass);
  EEPROM.put(addr, fingerprint);
  addr += sizeof(fingerprint);
  EEPROM.end();
}
void loop() {
  delay(1000);
}
