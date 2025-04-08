#include "pch.h"
#include <string>
#include "network_integration.h"

void setup() {
  Serial.begin(115200);
  setupWifi("WIFI - Setup","Gabriel","2014072276");
}

void loop() {
  // Main loop code
  int nivelDbm = (WiFi.RSSI()) * -1;
  Serial.println(nivelDbm);
}
