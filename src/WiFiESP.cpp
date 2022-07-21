#include "WiFiESP.h"
#include "local_config.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <string>
#include <iostream>

using namespace std;

WiFiESP::WiFiESP() {
    client = PubSubClient(espClient);

    //WiFi.mode(WIFI_STA);

}

void WiFiESP::connectWiFi() {
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);


  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected.");

}

void WiFiESP::setupMQTT() {
  client.setServer(MQTT_SER, 1883);
  client.setClient(espClient);
  // set the callback function
  client.setCallback(callback);
  Serial.println("MQTT setup done");
}

//Function, when message is received
 //callback includes topic and payload ( from which (topic) the payload is comming)
void WiFiESP::callback(char* topic, byte* payload, unsigned int length) {  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


//Function, when connection is lost
void WiFiESP::reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect("esp42JARVIS")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//Connect to mqtt server:
void WiFiESP::connectmqtt() {

  client.connect("esp42JARVIS");  // ESP will connect to mqtt broker with clientID
  Serial.println("connected to MQTT");

    if (!client.connected())
    {
      Serial.println("bla");
      reconnect();
    }
  
}

void WiFiESP::publishPayload(string payload, char* topic) {
  char* buffer = new char[payload.length() + 1];
  strcpy(buffer, payload.c_str());
  client.publish(topic, buffer);
}

void WiFiESP::publishPayload(char* payload, char* topic) {
  client.publish(topic, payload);
}

void WiFiESP::publishPayload(int payload, char* topic) {
  string pl = to_string(payload);
  const char* toSend = pl.c_str();
  client.publish(topic, toSend);  
}

void WiFiESP::loop() {
  client.loop();
}

bool WiFiESP::connected() {
  return client.connected();
}