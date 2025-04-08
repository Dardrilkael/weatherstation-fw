// Título: Integração WIFI
// Data: 08/04/2025
#pragma once
#include "pch.h"
#include <WiFi.h>              // Para conexão WiFi

// ----------- VARIÁVEIS GLOBAIS -----------
WiFiClient wifiClient;
// WiFiClientSecure secureWifiClient; // Ative se precisar de HTTPS com certificados

// ----------- FUNÇÕES -----------

/**
 * Inicializa a conexão WiFi em modo estação (STA).
 * 
 * @param contextName Nome do contexto para debug
 * @param ssid SSID da rede WiFi
 * @param password Senha da rede WiFi
 * @return int 0 se conectado com sucesso
 */
int setupWifi(const char* contextName, const char* ssid, const char* password)
{
  OnDebug(Serial.printf("%s: Estabelecendo conexão WiFi...\n", contextName);)

  WiFi.mode(WIFI_STA);               // Modo estação (cliente)
  WiFi.begin(ssid, password);        // Conecta à rede
  WiFi.setAutoReconnect(true);       // Reconnect automático
  WiFi.persistent(true);             // Salva rede nos registros

  // secureWifiClient.setCACert(root_ca); // Descomente se for usar HTTPS com certificado

  // Aguarda conexão
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    OnDebug(Serial.print(".");)
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    OnDebug(Serial.printf("\n%s: Conectado com sucesso. IP: %s\n", contextName, WiFi.localIP().toString().c_str());)
    return 0;
  } else {
    OnDebug(Serial.printf("\n%s: Falha na conexão WiFi.\n", contextName);)
    return -1;
  }
}
