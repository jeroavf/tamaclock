#include "clock_face.h"
#include "config.h"
#include "ntp_sync.h"
#include <Arduino.h>

static TFT_eSprite* spr     = nullptr;
static int          lastSec = -1;

static const char* MONTHS[] = {
    "JAN","FEV","MAR","ABR","MAI","JUN",
    "JUL","AGO","SET","OUT","NOV","DEZ"
};
static const char* DAYS[] = { "DOM","SEG","TER","QUA","QUI","SEX","SAB" };

void clockFaceInit(TFT_eSprite* sprite) {
    spr = sprite;
}

void clockFaceDraw() {
    if (!spr) return;

    struct tm t = ntpGetTime();
    if (t.tm_sec == lastSec) return;
    lastSec = t.tm_sec;

    spr->fillSprite(TFT_BLACK);

    // ── Seção superior (y=0..88) ──────────────────────────────

    // Título — "Tamaclock" centralizado no topo
    spr->setTextColor(TFT_CYAN, TFT_BLACK);
    spr->setTextDatum(TC_DATUM);
    spr->drawString("Tamaclock", 120, 2, 2);

    // HH:MM — Font7 (7-seg), branco
    char horaMin[6];
    snprintf(horaMin, sizeof(horaMin), "%02d:%02d", t.tm_hour, t.tm_min);
    spr->setTextColor(TFT_WHITE, TFT_BLACK);
    spr->setTextDatum(TL_DATUM);
    spr->drawString(horaMin, 5, 20, 7);

    // Caixa de segundos — âmbar, centralizada no eixo do HH:MM (centro y≈44)
    spr->fillRoundRect(165, 18, 72, 52, 6, COLOR_SEC_BG);
    char sec[3];
    snprintf(sec, sizeof(sec), "%02d", t.tm_sec);
    spr->setTextColor(TFT_BLACK, COLOR_SEC_BG);
    spr->setTextDatum(MC_DATUM);
    spr->drawString(sec, 201, 44, 7);

    // ── Seção inferior (y=88..135) ────────────────────────────
    spr->fillRect(0, 88, SCREEN_WIDTH, SCREEN_HEIGHT - 88, COLOR_DATE_BG);
    spr->drawFastHLine(0, 88, SCREEN_WIDTH, TFT_WHITE);

    // Badge do dia da semana
    spr->fillRoundRect(5, 93, 62, 37, 4, COLOR_DAY_BG);
    spr->setTextColor(TFT_BLACK, COLOR_DAY_BG);
    spr->setTextDatum(MC_DATUM);
    spr->drawString(DAYS[t.tm_wday], 36, 111, 4);

    // Caixa da data
    spr->fillRoundRect(72, 93, 164, 37, 4, TFT_WHITE);
    char data[12];
    snprintf(data, sizeof(data), "%s, %02d", MONTHS[t.tm_mon], t.tm_mday);
    spr->setTextColor(COLOR_DATE_BG, TFT_WHITE);
    spr->setTextDatum(ML_DATUM);
    spr->drawString(data, 82, 111, 4);

    // Assinatura — "by @jeroavf" pequeno no canto inferior direito
    spr->setTextColor(TFT_BLACK, TFT_WHITE);
    spr->setTextDatum(BR_DATUM);
    spr->drawString("by @jeroavf", 236, 131, 1);

    spr->pushSprite(0, 0);
}
