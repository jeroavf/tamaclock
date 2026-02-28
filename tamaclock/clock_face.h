#pragma once
#include <TFT_eSPI.h>

// Inicializa com o sprite compartilhado — chamar uma vez em setup().
void clockFaceInit(TFT_eSprite* spr);

// Desenha o relógio (só redesenha quando o segundo muda).
void clockFaceDraw();
