#include "mqtt.h"

MQTT::MQTT() : client(wifiClient) {}

MQTT::~MQTT() {}

bool MQTT::setup(const char* clientId, const char* server, int port,
                 const char* username, const char* password) {
    mqttClientId = clientId;
    mqttUsername = username;
    mqttPassword = password;
    
    client.setServer(server, port);
    return true;
}

bool MQTT::connect() {
    if (!client.connected()) {
        Serial.print("[MQTT] Conectando como: ");
        Serial.println(mqttClientId);
        
        if (client.connect(mqttClientId, mqttUsername, mqttPassword)) {
            Serial.println("[MQTT] Conectado com sucesso!");
            return true;
        } else {
            Serial.print("[MQTT] Falha: ");
            Serial.println(client.state());
            return false;
        }
    }
    return true;
}

bool MQTT::loop() {
    return client.loop();
}

bool MQTT::publish(const char* topic, const char* payload, bool retained) {
    return client.publish(topic, payload, retained);
}

bool MQTT::subscribe(const char* topic) {
    return client.subscribe(topic);
}

void MQTT::setCallback(MQTT_CALLBACK_SIGNATURE) {
    client.setCallback(callback);
}

void MQTT::setBufferSize(int size) {
    client.setBufferSize(size);
}
