#include <pgmspace.h>

#define SECRET
#define THINGNAME "esp8266Testing"

const char *WIFI_SSID = "loop";
const char *WIFI_PASSWORD = "cyber@123";
const char *AWS_IOT_ENDPOINT = "a1ws2bpdb04shg-ats.iot.us-east-1.amazonaws.com";
const char *topic_hello = "esp8266/switch";

// Amazon Root CA 1
const char AWS_CERT_CA[] = ("-----BEGIN CERTIFICATE-----\n\
-----END CERTIFICATE-----\n");

// Device Certificate
const char AWS_CERT_CRT[] = ("-----BEGIN CERTIFICATE-----\n\
-----END CERTIFICATE-----\n");

// Device Private Key
const char AWS_CERT_PRIVATE[] = ("-----BEGIN RSA PRIVATE KEY-----\n\
-----END RSA PRIVATE KEY-----\n");