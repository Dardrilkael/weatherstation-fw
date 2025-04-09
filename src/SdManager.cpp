#include "SdManager.h"
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include "constants.h"
#include "log.h"



bool SdManager::begin() {
    SPI.begin(SD_CLK_PIN, SD_MISO_PIN, SD_MOSI_PIN);
    int attempts = 0;

    while (!SD.begin(SD_CS_PIN, SPI)) {
        Serial.printf("[SD] Cartão não encontrado. Tentando novamente em %d segundos...\n", RETRY_INTERVAL / 1000);
        delay(RETRY_INTERVAL);
        if (++attempts > 5) {
            Log("[SD] Falha após várias tentativas.");
            return false;
        }
    }

    Log("[SD] Cartão iniciado com sucesso.");
    return true;
}

bool SdManager::createDirectory(const char* directory) {
    Serial.printf("[SD] Verificando/criando diretório: %s\n", directory);
    if (!SD.exists(directory)) {
        if (SD.mkdir(directory)) {
            Log("[SD] Diretório criado com sucesso.");
            return true;
        } else {
            Log("[SD] Falha ao criar diretório.");
            return false;
        }
    }
    Log("[SD] Diretório já existe.");
    return true;
}

bool SdManager::appendToFile(const char* path, const char* message) {
    File file = SD.open(path, FILE_APPEND);
    if (!file) {
        Log("[SD] Falha ao abrir arquivo para escrita.");
        return false;
    }

    if (file.print(message)) {
        Log("[SD] Dados gravados com sucesso.");
    } else {
        Log("[SD] Falha ao gravar dados.");
        file.close();
        return false;
    }

    file.close();
    return true;
}

bool SdManager::storeMeasurement(const char* directory, const char* fileName, const char* payload, const char* ext) {
    if (!createDirectory(directory)) return false;

    String fullPath = String(directory) + "/" + fileName + "." + ext;

    // Check if file exists before writing
    if (!SD.exists(fullPath)) {
        // File doesn't exist, write header first
        const char* header = "timestamp,value\n"; // Customize this based on your data format
        if (!appendToFile(fullPath.c_str(), header)) {
            OnDebug(Log("⚠️ Falha ao escrever o cabeçalho do arquivo."));
            return false;
        }
    }

    return appendToFile(fullPath.c_str(), payload);
}
