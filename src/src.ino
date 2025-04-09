#include "log.h"
#include <string>
#include <WiFi.h> 
#include "network_integration.h"
#include "SdManager.h"
#include "constants.h"

SdManager sd;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Log("Inicializando");
  setupWifi("WiFi","Gabriel","2014072276");
  delay(1000);
  NTP::setupTime();
  Log(NTP::getFormattedTime());

  if (!sd.begin()) {
    Log("[ERRO] Não foi possível iniciar o cartão SD.");
  }
  else{
    time_t timestamp = NTP::getTimestamp();
    String payloadLine = String(timestamp) + "_" + String(123);
    Log(payloadLine);
    sd.storeMeasurement("/data", "log", payloadLine.c_str());
  }
}

void loop() {
  // Main loop code
 
  //Log(NTP::getFormattedTime());
  //Log(NTP::getTimestamp());
  delay(800);

}
