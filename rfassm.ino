#include <ESP_EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266mDNS.h>
#include <EasyDDNS.h>
#include <WiFiUdp.h>
#include <TinyUPnP.h>

BearSSL::ESP8266WebServerSecure server(443);
TinyUPnP tinyUPnP(20000);

void handleRootFirstUse() {
  server.send(200, "text/plain", "setup index.html");
}

void handleRoot() {
  server.send(200, "text/plain", "static index.html");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
void setup() {
  Serial.begin(74880);
  int addr = 0;

  float firmwareVersion = EEPROM.get(addr, firmwareVersion);
  addr += sizeof(firmwareVersion);  
  bool firstRun = EEPROM.get(addr, firstRun);
  addr += sizeof(firstRun);
  char sysUser[8];
  EEPROM.get(addr, sysUser);
  addr += sizeof(sysUser);
  char sysPass[256];
  char w_ssid[64];
  char w_passwd[256];
  bool useMaster;
  char masterURL[128];
  char aesPass[256];
  char serverCert;
  char serverCertKey;
  Serial.println("BOOTING...\nReading Memory");
  String w_softAP = "RFASSM-";
  w_softAP += ESP.getFlashChipId();
  String myName = "RFASSM";
  myName += ESP.getFlashChipId();
  EEPROM.get(addr, sysPass);
  addr += sizeof(sysPass);
  EEPROM.get(addr, w_ssid);
  addr += sizeof(addr, w_passwd);
  EEPROM.get(addr, w_passwd);
  addr += sizeof(w_passwd);
  EEPROM.get(addr, aesPass);
  addr += sizeof(aesPass);
  EEPROM.get(addr, serverCert);
  addr += sizeof(serverCert);
  EEPROM.get(addr, serverCertKey);
  bool useDDNS;
  addr += sizeof(serverCertKey);
  EEPROM.get(addr, useDDNS);
  addr += sizeof(useDDNS);
  char ddnsUser[32];
  EEPROM.get(addr, ddnsUser);
  addr += sizeof(ddnsUser);
  char ddnsPass[32];
  EEPROM.get(addr, ddnsPass);
  addr += sizeof(ddnsPass);
  char ddnsHost[128];
  EEPROM.get(addr, ddnsHost);
  addr += sizeof(ddnsHost);
  
  if (!firstRun) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(w_ssid, w_passwd);
    Serial.print("Connecting to wifi network ");
    Serial.print(w_ssid);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  } else {
    WiFi.softAP(w_softAP);
    // Serial.printf("First time boot. Settings not found.\nStarting WiFi AP\nPlease connect to %s and visit https://%i/setup.html\n", w_softAP, WiFi.localIP());
  }
  static const char genCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDhDCCAmygAwIBAgIQC3hHjTFCetg8KQyOb8Dy9zANBgkqhkiG9w0BAQsFADAm
MSQwIgYDVQQDDBtSYW5kb20gQml0cyBBQiBSb290IFRlc3QgQ0EwHhcNMTkwNDA5
MTgzNjQ3WhcNMjIwMzI0MTgzNjQ3WjAVMRMwEQYDVQQDDApyZmFzc20tdHN0MIIB
IjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAvmfbK4eXhV1Wle9k6eE73+TT
dxrULBNQXjMUrZ9g/SOn3nqHPqyuEdKiTZ7pIIc/POuIHYE5m+0vPmUlaZQnrWOo
YP3HbKxoBmCQM1CoUYULx5zZLcCEFns36LQHKNId+M7p72VmyXk1typ6D76EVc7T
CBF8PtnUFmIzYlvpyTVqOlannucwXl3wtaZFxU4hJNfZ+RyT7AvobSyoD2aWTHYd
cKDEiaDyAxmFH+bBssYqqq2AGeyM1kUYyYJc1iGN4HThbZIp8XCkmhnz1krlIvsS
5v+nhjs3dEaagIVXsZkjcL16H6thwBWKi/HxIpn9RehPJiNTpO2XmCALjYf91wID
AQABo4G+MIG7MAkGA1UdEwQCMAAwHQYDVR0OBBYEFB0JyX1nOTWa2wfmJK/gLMw+
wXohMFYGA1UdIwRPME2AFDPoILEthPe03xCWK9HepeXc+248oSqkKDAmMSQwIgYD
VQQDDBtSYW5kb20gQml0cyBBQiBSb290IFRlc3QgQ0GCCQCZg9eqmiNxZTATBgNV
HSUEDDAKBggrBgEFBQcDATALBgNVHQ8EBAMCBaAwFQYDVR0RBA4wDIIKcmZhc3Nt
LXRzdDANBgkqhkiG9w0BAQsFAAOCAQEAr4RqKlL0Kd4micd0bBkgxIWmwsjKSBve
mLqc6UrFTkULD0bDJHmqTZrP1LiIwN5qEO59bvEREWwbhjd/8eD8sob/oOacbyMu
s43XvMzyiMULmpBKfZGk4PWjdTlyF+KoJQc4acvpltqHW5LCQNX9d3Oi0B31t0yk
2UbSxVk7miFsJ2xyzw+q3Zu2AbmoyuKOCDL9TGAMarMHyPsaBAYsknbvLtqZhE97
cwsXrKGX9/Uc2V2io60+hOv+AxXrbULmlpTYNAQlsUJ1dLJEKgpK2JCjiQCT11CE
DNcqTsFScb+x71jvYtYx5EqLK5Rju9NDYynMLRSsQslRHvwvw5RYwg==
-----END CERTIFICATE-----
)EOF";
static const char genCertKey[] PROGMEM =  R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEAvmfbK4eXhV1Wle9k6eE73+TTdxrULBNQXjMUrZ9g/SOn3nqH
PqyuEdKiTZ7pIIc/POuIHYE5m+0vPmUlaZQnrWOoYP3HbKxoBmCQM1CoUYULx5zZ
LcCEFns36LQHKNId+M7p72VmyXk1typ6D76EVc7TCBF8PtnUFmIzYlvpyTVqOlan
nucwXl3wtaZFxU4hJNfZ+RyT7AvobSyoD2aWTHYdcKDEiaDyAxmFH+bBssYqqq2A
GeyM1kUYyYJc1iGN4HThbZIp8XCkmhnz1krlIvsS5v+nhjs3dEaagIVXsZkjcL16
H6thwBWKi/HxIpn9RehPJiNTpO2XmCALjYf91wIDAQABAoIBAQCSaRXU4QyYFm+w
F0cHuJPr0+7vVQf1ZFyWMr0hzL4tqK86mglJHkszlT4MZbdptdXgqFQgK+/6gGWl
mu0rxCrGXy1od3xTijbrQHFhpBHprV6jfW5aigCHsPQK0fP3Zap6WdD0ropdPgEZ
+WWmmZ70xHbta1SYl7NICvbsGTUp671pcsnuwGUzV81UqItyD7/9wIKmaWXFWGOT
/8pv17TRdXWyC8XfItksJDZoAwcOCY8jsofDHiIHg91fIUqlRN0WHfTmqDs3BR9B
UveitDPeVJzNtP+QjsebQVRE6eeRJvpIV8EqqBxLIRt+1vm8ikOYHnwlXGo1DCAL
ziNE9uMxAoGBAPW3Hrdx91D1b7+4BoFx4u/KU723JgscBRYFiCjXreT3RUbe7f3k
v+6HuM9yAhIVodYuJ9azsIbho9psyGrcbPdqiRfrBG2KINygLBkFP5nPK0x1lByc
bBz67dHmq9oTiQG4s0U1zadlZRThP5iGCmIE0qV+yqjB0adUt5cZqFwjAoGBAMZg
FZxji4XxWl745n1dojAyp/PeAmFGyQ4P4lvNTjaTEDqjC0eUSLwfqC6cpQS1RMZf
ef2d6+/RoSCZRByDIZoiMAvuux79QBUouYQaaOzjVrGDDwpCoBzkU4NUSzLDh7T4
ab47sqzqoNIAJkc9k2AHrjkXrpUJLT7Zzf9+96i9AoGBAK+wu9nXN0s/8yNWZfBw
ih+Cv9XK+vy+kDRqyCzL6p7o3zQJeoor4cCgctiBYFGl/g2y+vFdkj8fX5Vyautd
qNxP5K6hcI0SUuTt8hoB0rQbmY4WFE1Q20ztbtbC5OYNSc1dT3GRQvcKXPhNBrf8
P7hOUn/a5PU9gLbwsETuJ5+ZAoGAGdQm3C7l2rG7F4SG1inb1TSzQ4BThQDnaqpM
ybEojtiFE21pKY+4DK5+glso7T0A0GdcBdaLArWjqg/qm1/Df0MWSGdz+6vzrQ2Z
YgbiRH2xKvmqeqaEHF7qNL5D8D8fK0Pw2LEDKZp/B/i3Ay2UQ7ucRpi7xXHlbzqC
pNKOQm0CgYEAnAlxigMPnefYjEuYyA/+CHUAr8goaukUiMfOgtXY4rfIo7daNe2/
Ap6bI9UVXV95Dsf1zK3F4A87xMNzTaxXmSp3jS4gpNB2qPVg9oDQdm28X0JO6zV8
z2wuIbxQ6qc6cFInjvr+4arrUPJ+6xoUFXE34W8UxLPTh/b+qpGYKMM=
-----END RSA PRIVATE KEY-----
)EOF";
// FIXME Change to variable cert store
server.setRSACert(new BearSSL::X509List(genCert), new BearSSL::PrivateKey(genCertKey));
  
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

if(useDDNS) {
  EasyDDNS.service("dyndns");    // Enter your DDNS Service Name - "duckdns" / "noip" / "dyndns" / "dynu"
EasyDDNS.client("hostname","username","client-key");    // Enter ddns Hostname - Username - Client-key
}
/*
boolean portMappingAdded = false;
  tinyUPnP.addPortMappingConfig(WiFi.localIP(), 443, RULE_PROTOCOL_TCP, 36000, myName);
  while (!portMappingAdded) {
    portMappingAdded = tinyUPnP.commitPortMappings();
    Serial.println("");
  
    if (!portMappingAdded) {
      // for debugging, you can see this in your router too under forwarding or UPnP
      tinyUPnP.printAllPortMappings();
      Serial.println(F("This was printed because adding the required port mapping failed"));
      delay(30000);  // 30 seconds before trying again
    }
  }
*/
  
  if (firstRun) {
    server.on("/", handleRootFirstUse);
  } else {
    server.on("/", handleRoot);
  }
  server.onNotFound(handleNotFound);
  server.begin();
  MDNS.begin(myName);
}

void loop() {

}
