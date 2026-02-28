#include "ntp_sync.h"
#include "config.h"
#include <Arduino.h>
#include <time.h>

static unsigned long lastSyncMs = 0;

void ntpInit() {
    // UTC-3, sem horário de verão (offset = 0)
    configTime(TZ_OFFSET * 3600L, 0, NTP_SERVER);

    Serial.print("[NTP] Aguardando sincronização");
    struct tm t;
    int tries = 0;
    while (!getLocalTime(&t) && tries < 30) {
        Serial.print(".");
        delay(500);
        tries++;
    }

    if (tries < 30) {
        lastSyncMs = millis();
        char buf[32];
        strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", &t);
        Serial.printf("\n[NTP] Sincronizado: %s\n", buf);
    } else {
        Serial.println("\n[NTP] AVISO: Falha no sync inicial.");
    }
}

void ntpSync() {
    configTime(TZ_OFFSET * 3600L, 0, NTP_SERVER);
    lastSyncMs = millis();
    Serial.println("[NTP] Re-sync disparado.");
}

bool ntpSyncIfDue() {
    if (millis() - lastSyncMs >= NTP_SYNC_INTERVAL_MS) {
        ntpSync();
        return true;
    }
    return false;
}

struct tm ntpGetTime() {
    struct tm t;
    if (!getLocalTime(&t)) {
        memset(&t, 0, sizeof(t));
    }
    return t;
}
