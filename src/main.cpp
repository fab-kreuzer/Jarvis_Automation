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

SoftwareSerial controllerSerial(DTX);

EspMQTTClient client(
  WIFI_SSID,
  WIFI_PASS,
  MQTT_SER,  // MQTT Broker server ip
  "ESP32_JARVIS"     // Client name that uniquely identify your device
);

String          command[9] = {};
int             pos= 0;
int             actualHeight = 0;

String param1 = "";
String param2 = "";
String calcHeight = "";
TaskHandle_t Core0TaskHandle;
TaskHandle_t Core1TaskHandle;

void printArray();
char *strToChar(String str);
void setHeight();
void moveDeskToHeight(int height);
const char* intToChar(int num);
void CoreTask1(void * parameter);

void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("jarvis/set/height", [](const String & payload) {
    Serial.println(payload);
  });
}

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

void loop() {
    client.loop();
}

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

            //Command finished:
            if(hexVal == "7e" || pos > 8 ) {

                pos = 0;
                setHeight();
                printArray();
                //client.publish("jarvis/get/height", intToChar(actualHeight));
                continue;
            }
        }
            
        delay (500);
    }
    
}

void moveDeskToHeight(int height) {

    if(height > actualHeight) { // goDown
        pinMode(HS1, HIGH);
        while(height > actualHeight) {delay(1);}
        pinMode(HS1, LOW);
            
    }

}

 char *strToChar(String str) {

  char *charArray = new char[str.length() + 1];
  str.toCharArray(charArray, str.length() + 1);

  return charArray;
}

void setHeight() {
    param1 = command[4];
    param2 = command[5];

    calcHeight= param1 + param2;

    int x;
    char* endptr;

    x = strtol(strToChar(calcHeight), &endptr, 16);

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

const char* intToChar(int num) {
    std::string s = std::to_string(num);
    char const *pchar = s.c_str();
    return pchar;
}
