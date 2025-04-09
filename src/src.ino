#include "log.h"
#include <string>
#include <WiFi.h> 
#include "network_integration.h"
#include "SdManager.h"
#include "constants.h"


SdManager sd;
void printConfig(const Config& config) {
  Serial.println(F("------ Configuração Atual ------"));
  Serial.print(F("UID: "));             Serial.println(config.station_uid);
  Serial.print(F("Nome da Estação: ")); Serial.println(config.station_name);
  Serial.print(F("WiFi SSID: "));       Serial.println(config.wifi_ssid);
  Serial.print(F("WiFi Password: "));   Serial.println(config.wifi_password);
  Serial.print(F("MQTT Server: "));     Serial.println(config.mqtt_server);
  Serial.print(F("MQTT Username: "));   Serial.println(config.mqtt_username);
  Serial.print(F("MQTT Password: "));   Serial.println(config.mqtt_password);
  Serial.print(F("MQTT Topic: "));      Serial.println(config.mqtt_topic);
  Serial.print(F("MQTT Port: "));       Serial.println(config.mqtt_port);
  Serial.print(F("Intervalo (ms): "));  Serial.println(config.interval);
  Serial.println(F("--------------------------------"));
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("[MQTT] Mensagem recebida em ");
    Serial.print(topic);
    Serial.print(": ");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  Log("Inicializando");

  while (!sd.begin()) {
    Log("[ERRO] Não foi possível iniciar o cartão SD.");
  }
  std::string configJson;
  sd.loadConfiguration("/config.txt", config, configJson);
  Log(configJson.c_str());
  printConfig(config);

  setupWifi("WiFi", config.wifi_ssid, config.wifi_password);
  delay(1000);
  NTP::setupTime();
  Log(NTP::getFormattedTime());

  time_t timestamp = NTP::getTimestamp();
  String payloadLine = String(timestamp) + "_" + String(123);
  Log(payloadLine);
  sd.storeMeasurement("/data", "log", payloadLine.c_str());
}

void loop() {
  // Main loop code
   if (Serial.available()) {
    char input = Serial.read();
    if (input == 'r' || input == 'R') {
      Serial.println("🔄 Reiniciando dispositivo...");
      delay(1000); // Só pra dar tempo de imprimir a mensagem
      ESP.restart(); // Reinicia o ESP32
    }
  }
  //Log(NTP::getFormattedTime());
  //Log(NTP::getTimestamp());
  delay(800);

}
