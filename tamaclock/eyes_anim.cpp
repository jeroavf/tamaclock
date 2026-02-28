#include "eyes_anim.h"
#include "config.h"
#include "tft_adapter.h"
#include <FluxGarage_RoboEyes.h>
#include <Arduino.h>

static TFT_eSprite* eyesSpr = nullptr;

void eyesAnimInit(TFT_eSprite* spr) {
    eyesSpr = spr;
}

static void runFor(RoboEyes<TFTSpriteAdapter>& eyes, unsigned long ms) {
    unsigned long end = millis() + ms;
    while (millis() < end) {
        eyes.update();
    }
}

void eyesAnimRun() {
    if (!eyesSpr) return;

    TFTSpriteAdapter display(eyesSpr, TFT_BLACK, TFT_CYAN);
    RoboEyes<TFTSpriteAdapter> eyes(display);

    eyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 60);
    eyes.setWidth(65, 65);
    eyes.setHeight(55, 55);
    eyes.setBorderradius(14, 14);
    eyes.setSpacebetween(20);
    eyes.setCuriosity(ON);

    // ── Sequência Cozmo ───────────────────────────────────────

    // Posiciona ao centro antes de abrir
    eyes.setPosition(DEFAULT);

    // 1. Acorda
    eyes.open();
    runFor(eyes, 700);
    eyes.setAutoblinker(true, 2, 1);  // pisca a cada 2±1 segundos

    // 2. Olha esquerda
    eyes.setPosition(W);
    runFor(eyes, 900);

    // 3. Olha direita
    eyes.setPosition(E);
    runFor(eyes, 900);

    // 4. Centro
    eyes.setPosition(DEFAULT);
    runFor(eyes, 400);

    // 5. Dorme
    eyes.setAutoblinker(false);
    eyes.close();
    runFor(eyes, 700);

    Serial.println("[Eyes] Animacao concluida.");
}
