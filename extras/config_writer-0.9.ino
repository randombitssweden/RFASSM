#include <EEPROM.h>
/* Write basic information according to Version number 0.90
 * Version numbers may be incompatable. So don't use this
 * unless you are a developer on/for rfassm or developing
 * your own rfassm devices.
 */
void setup() {
Serial.begin(74880);
EEPROM.begin(4194304);
delay(100);
Serial.println("Starting");
float firmwareVersion = 0.9;
bool firstRun = 0;
char sysUser[] = "admin";
char sysUserN[12];
strcpy(sysUserN,sysUser);
char sysPass[] = "welcome";
char sysPassN[256];
strcpy(sysPassN, sysPass);
char wifiSSID[] = "myWiFiNetworkName"; // Must Change this.
char wifiSSIDN[64];
strcpy(wifiSSIDN, wifiSSID);
char wifiPass[] = "mySecretPassword"; // Must hange this.
char wifiPassN[64];
strcpy(wifiPassN, wifiPass); 

char masterURL[] = "172.16.100.12"; // Should change this.
char masterURLN[128];
strcpy(masterURLN, masterURL);
char aesPass[] = "MySecretPass!"; // Really should change this.
char aesPassN[128];
strcpy(aesPassN, aesPass);
char serverCertN[4096];
char serverCertKeyN[8192];
// Really should change this.
static const char serverCert[] PROGMEM = R"EOF(
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
// Really should change this.
static const char serverCertKey[] PROGMEM =  R"EOF(
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
strcpy(serverCertN, serverCert);
strcpy(serverCertKeyN, serverCertKey);
bool useDDNS = 0;
char ddnsUser[] = "user";
char ddnsUserN[32];
strcpy(ddnsUserN, ddnsUser);
char ddnsPass[] = "password";
char ddnsPassN[32];
strcpy(ddnsPassN, ddnsPass);
char ddnsHost[] = "hostname.com";
char ddnsHostN[128];
strcpy(ddnsHostN, ddnsHost);
int addr = 0;
EEPROM.put(addr, firmwareVersion);
addr += sizeof(firmwareVersion);
EEPROM.put(addr, firstRun);
addr += sizeof(firstRun);
EEPROM.put(addr, sysUserN);
addr += sizeof(sysUserN);
EEPROM.put(addr, sysPassN);
addr += sizeof(sysPassN);
EEPROM.put(addr, wifiSSIDN);
addr += sizeof(wifiSSIDN);
EEPROM.put(addr, wifiPassN);
addr += sizeof(wifiPassN);
EEPROM.put(addr, useDDNS);
addr += sizeof(useDDNS)
EEPROM.put(addr, aesPassN); 
addr += sizeof(aesPassN);
EEPROM.put(addr, serverCertN);
addr += sizeof(serverCertN);
EEPROM.put(addr, serverCertKeyN);
addr += sizeof(serverCertKeyN);
EEPROM.put(addr, useDDNS);
addr += sizeof(useDDNS);
EEPROM.put(addr, ddnsUserN);
addr += sizeof(ddnsUserN);
EEPROM.put(addr, ddnsPassN);
addr += sizeof(ddnsPassN);
EEPROM.put(addr, ddnsHostN);
EEPROM.commit();
EEPROM.end();
Serial.println("Complete");
}

void loop() {

}
