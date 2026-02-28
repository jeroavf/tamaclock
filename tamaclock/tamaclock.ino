#include <TFT_eSPI.h>
#include "config.h"
#include "wifi_setup.h"
#include "ntp_sync.h"
#include "clock_face.h"
#include "eyes_anim.h"

TFT_eSPI    tft    = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);   // sprite compartilhado clock + olhos

enum AppState { STATE_CLOCK, STATE_ANIM };
static AppState      appState      = STATE_CLOCK;
static unsigned long animNextMs    = 0;
static unsigned long lastBrightMs  = 0;

// ── Brilho manual ────────────────────────────────────────
static bool manualBright   = false;  // override do brilho automático
static bool manualBrightDay = true;  // estado atual do toggle manual

// ── Estado dos botões ────────────────────────────────────
static bool          btn35Down   = false;
static unsigned long btn35PressMs = 0;
static bool          btn0Down    = false;
static unsigned long btn0PressMs  = 0;

// ── Utilitários ──────────────────────────────────────────

static void splashMsg(const char* msg) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 55);
    tft.print(msg);
}

static void updateBrightness() {
    if (manualBright) return;  // não sobrescreve override manual
    struct tm t = ntpGetTime();
    bool noite = (t.tm_hour >= NIGHT_START || t.tm_hour < NIGHT_END);
    ledcWrite(TFT_BL, noite ? BRIGHT_NIGHT : BRIGHT_DAY);
}

static void handleButtons() {
    // ── Botão 35: curto = animação | longo = toggle brilho ──
    bool b35 = (digitalRead(BTN_35) == LOW);
    if (b35 && !btn35Down) {
        btn35Down    = true;
        btn35PressMs = millis();
    } else if (!b35 && btn35Down) {
        unsigned long held = millis() - btn35PressMs;
        if (held >= BTN_LONG_MS) {
            // pressão longa: toggle brilho manual
            manualBright    = true;
            manualBrightDay = !manualBrightDay;
            ledcWrite(TFT_BL, manualBrightDay ? BRIGHT_DAY : BRIGHT_NIGHT);
            Serial.printf("[Botão] Brilho manual: %s\n", manualBrightDay ? "DIA" : "NOITE");
        } else {
            // pressão curta: dispara animação imediatamente
            if (appState == STATE_CLOCK) {
                animNextMs = millis();
                Serial.println("[Botão] Animacao disparada manualmente.");
            }
        }
        btn35Down = false;
    }

    // ── Botão 0: longo = reset WiFi ─────────────────────────
    bool b0 = (digitalRead(BTN_0) == LOW);
    if (b0 && !btn0Down) {
        btn0Down    = true;
        btn0PressMs = millis();
    } else if (!b0 && btn0Down) {
        unsigned long held = millis() - btn0PressMs;
        if (held >= BTN_WIFI_RESET_MS) {
            Serial.println("[Botão] Reset WiFi solicitado.");
            splashMsg("Reset WiFi...");
            wifiReset();  // apaga credenciais e reinicia
        }
        btn0Down = false;
    }
}

static void scheduleNextAnim() {
    unsigned long interval = random(ANIM_MIN_INTERVAL_MS, ANIM_MAX_INTERVAL_MS);
    animNextMs = millis() + interval;
    Serial.printf("[Timer] Proxima animacao em %lu s\n", interval / 1000);
}

// ── Setup ────────────────────────────────────────────────

void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.println("\n[TamaClock] Iniciando...");

    tft.init();
    tft.setRotation(TFT_ROTATION);
    tft.fillScreen(TFT_BLACK);

    ledcAttach(TFT_BL, 10000, 8);  // pino, 10kHz, 8 bits (API esp32 core 3.x)
    ledcWrite(TFT_BL, BRIGHT_DAY); // brilho inicial

    splashMsg("Conectando WiFi...");
    wifiSetup();

    splashMsg("Sincronizando NTP...");
    ntpInit();

    // Um único sprite de 240x135 (64KB) compartilhado entre clock e olhos
    bool ok = sprite.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
    Serial.printf("[Sprite] Criado: %s | heap livre: %lu bytes\n",
                  ok ? "OK" : "FALHOU", (unsigned long)ESP.getFreeHeap());

    randomSeed(analogRead(0));

    pinMode(BTN_35, INPUT);         // input-only, pullup externo na placa
    pinMode(BTN_0,  INPUT_PULLUP);  // botão BOOT com pullup interno

    clockFaceInit(&sprite);
    eyesAnimInit(&sprite);

    scheduleNextAnim();
    Serial.println("[TamaClock] Setup completo!");
}

// ── Loop ─────────────────────────────────────────────────

void loop() {
    handleButtons();
    ntpSyncIfDue();

    // Atualiza brilho a cada 60s
    if (millis() - lastBrightMs >= 60000UL) {
        updateBrightness();
        lastBrightMs = millis();
    }

    switch (appState) {

    case STATE_CLOCK:
        clockFaceDraw();
        if (millis() >= animNextMs) {
            appState = STATE_ANIM;
        }
        break;

    case STATE_ANIM:
        eyesAnimRun();

        {
            unsigned long postEnd = millis() + ANIM_POST_DELAY_MS;
            while (millis() < postEnd) {
                clockFaceDraw();
                delay(100);
            }
        }

        scheduleNextAnim();
        appState = STATE_CLOCK;
        break;
    }

    delay(50);
}
