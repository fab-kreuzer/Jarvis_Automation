#include <WiFi.h>
#include <PubSubClient.h>
#include <string>

using namespace std;

class WiFiESP {
    private:
    char* password;
    char* ssid;
    char* mqtt_server;

    public:

        WiFiClient espClient;
        PubSubClient client;
        char* topic;
        static char  direction;

        WiFiESP();
        void connectWiFi();
        void connectmqtt();
        void loop();
        static void callback(char* topic, byte* payload, unsigned int length);
        void reconnect();
        void publishPayload(string payload);
        void publishPayload(char* payload);
        void setupMQTT();
        bool connected();
        void subscribe(char* topic);
};
