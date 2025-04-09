#include <Update.h>        // Core ESP32 library for firmware updates
#include <HTTPClient.h>   
#include <WiFi.h> 
#include "Log.h"
bool startUpdate(const String& url) {
    Logln("🔄 Iniciando processo OTA...");

    HTTPClient http;
    if (!http.begin(url)) {
        Logln("🚫 Não foi possível conectar ao servidor OTA.");
        return false;
    }

    int httpCode = http.GET();
    if (httpCode != HTTP_CODE_OK) {
        Logf("⚠️  Servidor retornou código: %d\n", httpCode);
        http.end();
        return false;
    }

    int contentLength = http.getSize();
    if (contentLength <= 0) {
        Logln("🚫 Tamanho inválido para atualização.");
        http.end();
        return false;
    }

    Logf("📥 Tamanho do firmware: %d bytes\n", contentLength);

    if (!Update.begin(contentLength)) {
        Logln("❌ Sem espaço suficiente para atualizar.");
        http.end();
        return false;
    }

    WiFiClient* stream = http.getStreamPtr();
    size_t written = Update.writeStream(*stream);

    if (written != contentLength) {
        Logf("⚠️  Escrita incompleta: %d/%d bytes\n", (int)written, contentLength);
        http.end();
        return false;
    }

    if (!Update.end()) {
        Logf("❌ Erro ao finalizar update: %s\n", Update.errorString());
        http.end();
        return false;
    }

    if (!Update.isFinished()) {
        Logln("❌ Update não finalizado corretamente.");
        http.end();
        return false;
    }

    Logln("✅ Firmware atualizado com sucesso!");
    http.end();
    return true;
}
