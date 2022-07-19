#include <Arduino.h>
#include "jarvis_pinouts.h"
#include <string.h>

#include <HardwareSerial.h>

void moveDesk(char dir);

HardwareSerial hsSerial(2);

void setup() {

    
    hsSerial.begin(9600, SERIAL_8N1, HTX, DTX);
    Serial.begin(9600);

    Serial.println("Serial Txd is on pin: "+String(HTX));
    Serial.println("Serial Rxd is on pin: "+String(DTX));

    pinMode(HS0, OUTPUT);
    pinMode(HS1, OUTPUT);
    pinMode(HS2, OUTPUT);
    pinMode(HS3, OUTPUT);

    pinMode(DTX, INPUT);
    pinMode(HTX, INPUT);

    digitalWrite(HS0, LOW);
    digitalWrite(HS1, LOW);
    digitalWrite(HS2, LOW);
    digitalWrite(HS3, LOW);

}


void loop() {

    Serial.println(hsSerial.available());

    if(hsSerial.available() > 0)
        Serial.print(hsSerial.readString());

    delay(1000);
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

