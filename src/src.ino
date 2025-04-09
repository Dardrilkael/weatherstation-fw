#include "log.h"
#include <string>
#include <WiFi.h>
#include "network_integration.h"
#include "SdManager.h"
#include "constants.h"
#include "Mqtt.h"
#define FIRMWARE_VERSION "4.0.0"

MQTT mqtt;
SdManager sd;
void printConfig(const Config& config) {
  Logln(F("   -------- Configuração Atual --------"));
  Log(F("\t|UID:             "));
  Logln(config.station_uid);
  Log(F("\t|Nome da Estação: "));
  Logln(config.station_name);
  Log(F("\t|WiFi SSID:       "));
  Logln(config.wifi_ssid);
  Log(F("\t|WiFi Password:   "));
  Logln(config.wifi_password);
  Log(F("\t|MQTT Server:     "));
  Logln(config.mqtt_server);
  Log(F("\t|MQTT Username:   "));
  Logln(config.mqtt_username);
  Log(F("\t|MQTT Password:   "));
  Logln(config.mqtt_password);
  Log(F("\t|MQTT Topic:      "));
  Logln(config.mqtt_topic);
  Log(F("\t|MQTT Port:       "));
  Logln(config.mqtt_port);
  Log(F("\t|Intervalo (ms):  "));
  Logln(config.interval);
  Logln(F("    ------------------------------------\n"));
}

void callback(char* topic, byte* payload, unsigned int length) {
  Logf("[MQTT] Mensagem recebida em %s: ", topic);

  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Log(message);
}

void setup() {
  Serial.begin(115200);
  delay(2500);
  Logf("🚀 Iniciando dispositivo... 🛠 Firmware v%s\n\n", FIRMWARE_VERSION);
  delay(1000);

  while (!sd.begin()) {
    Log("[ERRO] Não foi possível iniciar o cartão SD.\n");
  }
  std::string configJson;
  sd.loadConfiguration("/config.txt", config, configJson);
  Logln(configJson.c_str());
  printConfig(config);

  setupWifi("WiFi", config.wifi_ssid, config.wifi_password);
  delay(1000);
  NTP::setupTime();
  Log(NTP::getFormattedTime());

  time_t timestamp = NTP::getTimestamp();
  String payloadLine = String(timestamp) + "_" + String(123);
  Logln(payloadLine);
  sd.storeMeasurement("/data", "log", payloadLine.c_str());

  mqtt.setup(config.station_name, config.mqtt_server, config.mqtt_port, config.mqtt_username, config.mqtt_password);
  mqtt.setCallback(callback);
  if (mqtt.connect()) {
    mqtt.subscribe(config.mqtt_topic);
  } else {
    Logln("MQTT connection failed.");
  }
}

void loop() {
  // Main loop code
  if (Serial.available()) {
    char input = Serial.read();
    if (input == 'r' || input == 'R') {
      Logln("🔄 Reiniciando dispositivo...");
      delay(1000);    // Só pra dar tempo de imprimir a mensagem
      ESP.restart();  // Reinicia o ESP32
    }
  }
  mqtt.loop();
  //Log(NTP::getFormattedTime());
  //Log(NTP::getTimestamp());
  delay(800);
}
