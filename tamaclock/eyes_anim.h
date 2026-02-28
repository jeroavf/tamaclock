#pragma once
#include <TFT_eSPI.h>

// Inicializa com o sprite compartilhado — chamar uma vez em setup().
void eyesAnimInit(TFT_eSprite* spr);

// Executa a animação Cozmo completa (~4s, bloqueante).
void eyesAnimRun();
