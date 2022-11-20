#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include <PubSubClient.h>
#include "jarvis_pinouts.h"
#include "SoftwareSerial.h"
#include "WiFiESP.h"
#include "local_config.h"
#include <string> 

using namespace std;

/************************************************/

EspMQTTClient client( WIFI_SSID, WIFI_PASS, MQTT_SER, "ESP32_JARVIS" );

/************************************************/

String          command[9]      = {};
int             pos             = 0;
int             actualHeight    = 0;

String          param1          = "";
String          param2          = "";
String          calcHeight      = "";
String          strHeight       = "";
String          lastHeight      = "";
String          toHeight        = "";
TaskHandle_t    Core0TaskHandle;
TaskHandle_t    Core1TaskHandle;
SoftwareSerial  controllerSerial(DTX);

/************************************************/
void        printArray();
char*       strToChar(String str);
void        setHeight();
void        moveDeskToHeight(void * parameter);
const char* intToChar(int num);
void        CoreTask1(void * parameter);

/************************************************/

void onConnectionEstablished()
{
  client.subscribe("jarvis/set/height", [](const String & payload) {
    toHeight = payload;
    Serial.println("created");
    xTaskCreatePinnedToCore(moveDeskToHeight, "CPU_0", 4096, NULL, 1, &Core0TaskHandle, 0);
  });
}

/************************************************/

void setup() {

    
    client.enableDebuggingMessages(); 
    client.enableHTTPWebUpdater(); 
    client.enableOTA(); 
    client.enableLastWillMessage("Jarvis/lastwill", "I am going offline");
    
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


    xTaskCreatePinnedToCore(CoreTask1, "CPU_1", 4096, NULL, 1, &Core1TaskHandle, 1);

    Serial.println("setup finished");

}

/************************************************/

void loop() {
    client.loop();
}

/************************************************/

void CoreTask1(void * parameter) {
    String hexVal = "";
    for(;;) {

        while(controllerSerial.available()) {

            int val = controllerSerial.read();
            
             hexVal = String(val, HEX);

            if(hexVal.length() == 1)
                hexVal = "0" + hexVal;

            command[pos] = hexVal;

            pos++;

            if(hexVal == "7e" || pos > 8 ) {

                pos = 0;
                setHeight();

                if (lastHeight != strHeight) {
                    printArray();
                    client.publish("jarvis/get/height", strHeight);
                    lastHeight = strHeight;
                }
                continue;
            }
        }
            
        delay (100);
    }

}

/************************************************/

void moveDeskToHeight(void * parameter) {

    int h = atoi(toHeight.c_str());

    Serial.print(h);

    if (h <= 10) {

        switch (h) {
            case 1:
                digitalWrite(HS0, HIGH);
                digitalWrite(HS1, HIGH);
                delay(100);
                digitalWrite(HS1, LOW);
                digitalWrite(HS0, LOW);
            break;
            case 2:
                digitalWrite(HS2, HIGH);
                delay(100);
                digitalWrite(HS2, LOW);
            break;
            case 3:
                digitalWrite(HS0, HIGH);
                digitalWrite(HS2, HIGH);
                delay(100);
                digitalWrite(HS2, LOW);
                digitalWrite(HS0, LOW);
            break;
            case 4:
                digitalWrite(HS2, HIGH);
                digitalWrite(HS1, HIGH);
                delay(100);
                digitalWrite(HS1, LOW);
                digitalWrite(HS2, LOW);
            break;
            case 5:

            break;
        }

        vTaskDelete(Core0TaskHandle);
    }

    // Move desk to specific Height:
    if(h < actualHeight) { // goDown

        digitalWrite(HS0, HIGH);

        while(h < actualHeight) {
            delay(10);
        }

        digitalWrite(HS0, LOW);   

    }else if (h > actualHeight) { //goUp
        digitalWrite(HS1, HIGH);

        while(h > actualHeight) {
            delay(100);
        }

        digitalWrite(HS1, LOW);   
    } else {

        Serial.println("no move");
    }

    vTaskDelete(Core0TaskHandle);
}

/************************************************/

char *strToChar(String str) {

  char *charArray = new char[str.length() + 1];
  str.toCharArray(charArray, str.length() + 1);

  return charArray;
}

/************************************************/

void setHeight() {
    param1 = command[4];
    param2 = command[5];

    calcHeight= param1 + param2;

    int x;
    char* endptr;

    x = strtol(strToChar(calcHeight), &endptr, 16);

    actualHeight = x / 10;
    strHeight = String(actualHeight, DEC);
}

/************************************************/

void printArray() {
  for (int i = 0; i < 9; i++) {

    Serial.print(command[i] + " ");

    if(command[i] == "7e")
        break;
  }
  
  Serial.println();
}

/************************************************/

const char* intToChar(int num) {
    std::string s = std::to_string(num);
    char const *pchar = s.c_str();
    return pchar;
}
