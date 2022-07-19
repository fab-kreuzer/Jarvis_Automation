#include "WiFiESP.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <string>
using namespace std;

WiFiESP::WiFiESP() {
    password = "635430635470";
    ssid = "Kreuzers-Wlan-2.4";
    mqtt_server = "192.168.2.189";
    topic = "jarvis";

    client = PubSubClient(espClient);

    WiFi.mode(WIFI_STA);
}

void WiFiESP::connectWiFi() {
  Serial.print("Connecting to ");
 
  WiFi.begin(ssid, password);

  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected.");

}

void WiFiESP::setupMQTT() {
  client.setServer(mqtt_server, 1883);
  client.setClient(espClient);
  client.subscribe(topic);
  client.setCallback(callback);
  Serial.println("MQTT setup done");
}

//Connect to mqtt server:
void WiFiESP::connectmqtt() {

  client.connect("ESP32_JARVIS");  // ESP will connect to mqtt broker with clientID
    Serial.println("connected to MQTT");

    if (!client.connected())
    {
      reconnect();
    }
  
}


//Function, when connection is lost
void WiFiESP::reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect("ESP32_JARVIS")) {
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

//Function, when message is received
 //callback includes topic and payload ( from which (topic) the payload is comming)
void WiFiESP::callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");

    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }

    Serial.println();
    Serial.println("-----------------------");

}

void WiFiESP::publishPayload(string payload) {
  char* buffer = new char[payload.length() + 1];
  strcpy(buffer, payload.c_str());
  client.publish(topic, buffer);
}

void WiFiESP::publishPayload(char* payload) {
  client.publish(topic, payload);
}

void WiFiESP::loop() {
  client.loop();
}

bool WiFiESP::connected() {
  return client.connected();
}