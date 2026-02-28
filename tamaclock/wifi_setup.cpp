#include "wifi_setup.h"
#include "config.h"
#include <WiFiManager.h>
#include <Arduino.h>

void wifiSetup() {
    WiFiManager wm;
    wm.setConfigPortalTimeout(WIFI_TIMEOUT_S);

    Serial.println("[WiFi] Iniciando WiFiManager...");

    if (!wm.autoConnect(WIFI_AP_NAME)) {
        Serial.println("[WiFi] Timeout — reiniciando.");
        delay(1000);
        ESP.restart();
    }

    Serial.print("[WiFi] Conectado. IP: ");
    Serial.println(WiFi.localIP());
}

void wifiReset() {
    WiFiManager wm;
    wm.resetSettings();
    Serial.println("[WiFi] Credenciais apagadas — reiniciando.");
    delay(500);
    ESP.restart();
}
