#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include <PubSubClient.h>
#include "jarvis_pinouts.h"
#include "SoftwareSerial.h"
#include "WiFiESP.h"

SoftwareSerial controllerSerial(DTX);
WiFiESP wifi;
PubSubClient client;

String          command[9] = {};
int             pos= 0;
int             actualHeight = 0;

void moveDesk(char dir);
void printArray();
char *strToChar(String str);
void setHeight();
void moveDeskToHeight(int height);

void setup() {

    
    controllerSerial.begin(9600);
    Serial.begin(115200);

    Serial.println("Serial Txd is on pin: "+String(HTX));
    Serial.println("Serial Rxd is on pin: "+String(DTX));

    pinMode(HS0, OUTPUT);
    pinMode(HS1, OUTPUT);
    pinMode(HS2, OUTPUT);
    pinMode(HS3, OUTPUT);

    digitalWrite(HS0, LOW);
    digitalWrite(HS1, LOW);
    digitalWrite(HS2, LOW);
    digitalWrite(HS3, LOW);

    wifi.connectWiFi();
    wifi.setupMQTT();
    wifi.connectmqtt();

    client = wifi.client;

    Serial.println("setup finished");

}


void loop() {
    if (!client.connected())
    {
        wifi.reconnect();
    }

    
    while(controllerSerial.available()) {

        int val = controllerSerial.read();
        
        String hexVal = String(val, HEX);

        if(hexVal.length() == 1)
            hexVal = "0" + hexVal;

        command[pos] = hexVal;

        pos++;

        //Command finished:
        if(hexVal == "7e" || pos > 8 ) {

            pos = 0;
            setHeight();
            printArray();
            continue;
        }
    }

    client.loop();

}

void moveDeskToHeight(int height) {

    if(height > actualHeight) { // goDown
        pinMode(HS1, HIGH);
        while(height > actualHeight) {delay(1);}
        pinMode(HS1, LOW);
            
    }

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

 char *strToChar(String str) {

  char *charArray = new char[str.length() + 1];
  str.toCharArray(charArray, str.length() + 1);

  return charArray;
}

void setHeight() {
    String p1 = command[4];
    String p2 = command[5];

    String height = p1 + p2;

    int x;
    char* endptr;

    x = strtol(strToChar(height), &endptr, 16);

    String strHeight = String(x, DEC);
    actualHeight = strHeight.toInt() / 10;
}

void printArray() {
  for (int i = 0; i < 9; i++) {

    Serial.print(command[i] + " ");

    if(command[i] == "7e")
        break;
  }
  
  Serial.println();
}
