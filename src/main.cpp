#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include "jarvis_pinouts.h"
#include "SoftwareSerial.h"

using namespace std;

SoftwareSerial  deskSerial(17);

String          arr[9];
int             pos= 0;

void moveDesk(char dir);
void printArray();
char *strToChar(String str);
void getHeight();

void setup() {

    
    deskSerial.begin(9600);
    Serial.begin(115200);

    Serial.println("Serial Txd is on pin: "+String(HTX));
    Serial.println("Serial Rxd is on pin: "+String(DTX));

    pinMode(HS0, OUTPUT);
    pinMode(HS1, OUTPUT);
    pinMode(HS2, OUTPUT);
    pinMode(HS3, OUTPUT);

    //pinMode(DTX, INPUT);
    //pinMode(HTX, INPUT);

    digitalWrite(HS0, LOW);
    digitalWrite(HS1, LOW);
    digitalWrite(HS2, LOW);
    digitalWrite(HS3, LOW);

}


void loop() {

    while(deskSerial.available()) {

        int val = deskSerial.read();
        
        String hexVal = String(val, HEX);

        arr[pos] = hexVal;

        pos++;

        if(val == 126) {
            Serial.println();
            pos = 0;
            printArray();
            getHeight();
        }
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

void getHeight() {
    String p1 = arr[4];
    String p2 = arr[5];

    if(p2.length() == 1) {
        p2 = "0" + p2;
    }

    String height = p1 + p2;

    int x;
    char* endptr;

    x = strtol(strToChar(height), &endptr, 16);

    Serial.println(height);

    String strHeight = String(x, DEC);
    int heightInt = strHeight.toInt();

    Serial.println(heightInt);

}

void printArray() {
  for (int i = 0; i < 9; i++) {
    Serial.print(arr[i] + " ");
  }
  Serial.println();
}
