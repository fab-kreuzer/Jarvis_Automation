#include <WiFi.h>
#include <PubSubClient.h>
#include <string>

using namespace std;

class WiFiESP {
    public:

        WiFiClient espClient;
        PubSubClient client;

        WiFiESP();
        void connectWiFi();
        void connectmqtt();
        void loop();
        static void callback(char* topic, byte* payload, unsigned int length);
        void reconnect();
        void publishPayload(string payload, char* topic);
        void publishPayload(char* payload, char* topic);
        void publishPayload(int payload, char* topic);
        void setupMQTT();
        bool connected();
};
