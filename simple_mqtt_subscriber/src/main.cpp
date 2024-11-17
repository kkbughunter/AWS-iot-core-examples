#include "secrets.h"
#include <PubSubClient.h>
#include "ESP8266WiFi.h"
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;  // Define NTP Client to get time
NTPClient timeClient(ntpUDP, "pool.ntp.org", 36000, 60000);

BearSSL::X509List CERT_CA(AWS_CERT_CA);
BearSSL::X509List CERT_CRT(AWS_CERT_CRT);
BearSSL::PrivateKey CERT_PRIVATE(AWS_CERT_PRIVATE);

WiFiClientSecure wiFiClient;  // Define AWS Connection
PubSubClient pubSubClient(AWS_IOT_ENDPOINT, 8883, wiFiClient); // Define MQTT Client

void messageReceived(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);

    // Convert payload to string
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println("Message: " + message);
}

void connectToAWSThing() {
    if (!pubSubClient.connected()) {
        Serial.print("\n\n↺ PubSubClient connecting to: ");
        Serial.println(AWS_IOT_ENDPOINT);
        while (!pubSubClient.connected()) {
            timeClient.update();
            wiFiClient.setX509Time(timeClient.getEpochTime());
          
            Serial.println("↺ Attempting MQTT connection...");
            if(pubSubClient.connect(THINGNAME)){
                Serial.printf("✔ Connected to AWS Thing: %s\n", THINGNAME);

                // Subscribe to the topic after connecting
                pubSubClient.subscribe(topic_hello);
                Serial.printf("✔ Subscribed to topic: %s\n", topic_hello);
            }
            else {
                Serial.print("failed, rc= ");
                Serial.print(pubSubClient.state());
                
                char buf[256];
                wiFiClient.getLastSSLError(buf, 256);
                Serial.println("WiFiClientSecure SSL error: ");
                Serial.println(buf);
            }
        }
    }
}

void setup() {
    Serial.begin(115200);
    Serial.printf("\n\n✔ ESP8266 SDK version: %s\n", ESP.getSdkVersion());
     
    timeClient.begin();
    wiFiClient.setX509Time(timeClient.getEpochTime());
    
    Serial.print("↺ Connecting to WiFi: ");
    Serial.print(WIFI_SSID);
    WiFi.mode(WIFI_STA);
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println();
    WiFi.waitForConnectResult();
    Serial.print("✔ WiFi connected, IP address: ");
    Serial.println(WiFi.localIP());

    // Configure PubSubClient and WiFiClient settings
    pubSubClient.setKeepAlive(90);
    pubSubClient.setBufferSize(6000);
    wiFiClient.setBufferSizes(512, 512);

    // Set AWS Connection Credentials
    wiFiClient.setTrustAnchors(&CERT_CA);
    wiFiClient.setClientRSACert(&CERT_CRT, &CERT_PRIVATE);

    // Set the callback function to handle incoming messages
    pubSubClient.setCallback(messageReceived);
}

void loop() {
    connectToAWSThing();
    pubSubClient.loop();
    delay(1000);  // Wait briefly between checks
}