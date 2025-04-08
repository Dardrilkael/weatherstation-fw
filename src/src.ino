#include "pch.h"
#include <string>
#include "network_integration.h"
#include <WiFi.h> 

void setup() {
  Serial.begin(115200);
  delay(1000);
  Log("Inicializando");
  setupWifi("WiFi","Gabriel","2014072276");
  delay(1000);
  NTP::setupTime();
  Log(NTP::getFormattedTime());
}

void loop() {
  // Main loop code
 
  Log(NTP::getFormattedTime().c_str());
  Log(NTP::getTimestamp());
  delay(800);

}
