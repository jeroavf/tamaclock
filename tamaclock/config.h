#pragma once

// ── Display ──────────────────────────────────────────────
#define SCREEN_WIDTH   240
#define SCREEN_HEIGHT  135
#define TFT_ROTATION   1   // landscape

// ── Cores ─────────────────────────────────────────────────
// RGB(0, 100, 190) = steel blue para bloco de data
#define COLOR_DATE_BG  0x032F
// Âmbar/laranja para caixa de segundos
#define COLOR_SEC_BG   0xFD20
// Badge do dia da semana
#define COLOR_DAY_BG   TFT_CYAN

// ── Brilho do backlight (PWM 0-255) ──────────────────────
#define BRIGHT_DAY     200
#define BRIGHT_NIGHT   60
#define NIGHT_START    22   // hora início brilho reduzido
#define NIGHT_END      7    // hora fim brilho reduzido

// ── NTP ──────────────────────────────────────────────────
#define NTP_SERVER            "pool.ntp.org"
#define TZ_OFFSET             (-3)          // UTC-3 BRT
#define NTP_SYNC_INTERVAL_MS  (12UL * 3600000UL)  // 12h em ms

// ── WiFi ─────────────────────────────────────────────────
#define WIFI_AP_NAME    "TamaClock-Config"
#define WIFI_TIMEOUT_S  180

// ── Animação ─────────────────────────────────────────────
#define ANIM_MIN_INTERVAL_MS  120000UL   // 2 min
#define ANIM_MAX_INTERVAL_MS  300000UL   // 5 min
#define ANIM_POST_DELAY_MS    10000UL    // 10s de relógio pós-animação

// ── Botões ───────────────────────────────────────────────
#define BTN_35             35       // curto: animação | longo: toggle brilho
#define BTN_0              0        // longo: reset WiFi
#define BTN_LONG_MS        2000UL   // 2s = pressão longa
#define BTN_WIFI_RESET_MS  3000UL   // 3s = reset WiFi

// ── Serial ───────────────────────────────────────────────
#define SERIAL_BAUD  115200
