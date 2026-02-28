#pragma once
#include <TFT_eSPI.h>

// Adapter fino que expõe a API Adafruit GFX (usada pelo RoboEyes)
// via um TFT_eSprite existente.
//
// RoboEyes passa color=0 (BGCOLOR) ou color=1 (MAINCOLOR).
// O adapter mapeia esses valores para cores RGB565 reais.

class TFTSpriteAdapter {
    TFT_eSprite* _spr;
    uint16_t     _bgColor;
    uint16_t     _eyeColor;

public:
    TFTSpriteAdapter(TFT_eSprite* spr, uint16_t bgColor, uint16_t eyeColor)
        : _spr(spr), _bgColor(bgColor), _eyeColor(eyeColor) {}

    // Limpa o buffer com a cor de fundo
    void clearDisplay() {
        _spr->fillSprite(_bgColor);
    }

    // Envia o sprite para o display físico
    void display() {
        _spr->pushSprite(0, 0);
    }

    // RoboEyes chama com color=0 (fundo) ou color=1 (olho)
    void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,
                       int16_t r, uint8_t color) {
        _spr->fillRoundRect(x, y, w, h, r, color ? _eyeColor : _bgColor);
    }

    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                      int16_t x2, int16_t y2, uint8_t color) {
        _spr->fillTriangle(x0, y0, x1, y1, x2, y2, color ? _eyeColor : _bgColor);
    }
};
