#include <Arduino.h>
#include "WiFiESP.h"
#include "jarvis_pinouts.h"

WiFiESP  wifi;
PubSubClient client;

void moveDesk(char dir);

void setup() {

    Serial.begin(9600);

    pinMode(HS0, OUTPUT);
    pinMode(HS1, OUTPUT);
    pinMode(HS2, OUTPUT);
    pinMode(HS3, OUTPUT);
    pinMode(DTX, OUTPUT);
    pinMode(HTX, OUTPUT);


    wifi.connectWiFi();
    wifi.setupMQTT();
    wifi.connectmqtt();

    client = wifi.client;
    
}


void loop() {
    if (!wifi.client.connected())
        wifi.reconnect();
    
    wifi.client.loop();
}

void moveDesk(char dir) {
    Serial.print("Move desk: ");
    // Move desk

    switch(dir) {
        case 'u':
            Serial.println("Up");

            digitalWrite(HS1, HIGH);
            delay(2000);
            digitalWrite(HS1, LOW);

            break;
        case 'd':
            Serial.println("Down");
            
            digitalWrite(HS0, HIGH);
            delay(2000);
            digitalWrite(HS0, LOW);

            break;
        case '1':
            Serial.println("1");
            digitalWrite(HS0, HIGH);
            digitalWrite(HS1, HIGH);
            delay(500);
            digitalWrite(HS0, LOW);
            digitalWrite(HS1, LOW);
            break;
        case '2':
            Serial.println("2");
            break;
        case '3':
            Serial.println("3");
            break;
        case '4':
            Serial.println("4");
            break;
        case 'm':
            Serial.println("memory");
            break;
        default:
            Serial.println("Unknown");
            break;
    }

    delay(1000);
}

