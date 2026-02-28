#pragma once

// Conecta ao Wi-Fi via WiFiManager.
// Se não houver credenciais salvas, abre AP "TamaClock-Config".
void wifiSetup();

// Apaga credenciais salvas e reinicia o dispositivo.
void wifiReset();
