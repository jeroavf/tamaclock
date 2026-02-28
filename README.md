# TamaClock

Relógio digital NTP com animação de olhinhos estilo Cozmo (Anki), rodando no **TTGO T-Display** (ESP32).

![TamaClock](exemplos/Captura%20de%20tela%20de%202026-02-24%2021-11-17.png)

---

## Funcionalidades

- **Relógio NTP** — sincronização automática com `pool.ntp.org`, fuso UTC-3 (BRT), re-sync a cada 12h
- **Layout estilo Volos** — HH:MM em fonte 7-segmentos grande, caixa de segundos âmbar, badge do dia e data
- **Animação Cozmo** — olhinhos com pálpebras e pupilas animadas, disparada automaticamente a cada 2–5 minutos
- **WiFiManager** — portal web de configuração Wi-Fi na 1ª inicialização; credenciais salvas em flash
- **Backlight automático** — brilho reduzido entre 22h e 7h; override manual pelo botão
- **Botões físicos** — controle de animação, brilho e reset de Wi-Fi

---

## Hardware

| Item | Detalhe |
|------|---------|
| Placa | TTGO T-Display (ESP32) |
| Display | TFT ST7789, 1.14", 240×135 px |
| Orientação | Landscape |
| Conectividade | Wi-Fi integrado |

---

## Layout do Display

```
┌──────────────────────────────────────┐
│           Tamaclock                  │  ← título (cyan)
│  HH:MM              [SS]             │  ← hora (branco) + segundos (âmbar)
│                                      │
├──────────────────────────────────────┤
│  [DIA]   MÊS, DD        by @jeroavf │  ← badge dia + data + assinatura
└──────────────────────────────────────┘
```

---

## Botões

| Botão | Ação | Threshold |
|-------|------|-----------|
| GPIO 35 — pressão curta | Dispara animação Cozmo imediatamente | < 2 s |
| GPIO 35 — pressão longa | Alterna brilho manual DIA ↔ NOITE | ≥ 2 s |
| GPIO 0 — pressão longa | Apaga credenciais Wi-Fi e reinicia | ≥ 3 s |

---

## Estrutura do Projeto

```
tamaclock/
├── tamaclock.ino       # setup(), loop(), handleButtons()
├── clock_face.h/.cpp   # tela do relógio (estilo Volos)
├── eyes_anim.h/.cpp    # animação olhinhos Cozmo (FluxGarage RoboEyes)
├── ntp_sync.h/.cpp     # sincronização NTP
├── wifi_setup.h/.cpp   # WiFiManager + wifiReset()
├── tft_adapter.h       # adapter TFT_eSprite → RoboEyes
└── config.h            # constantes globais
```

---

## Dependências

Instalar via Arduino Library Manager:

| Biblioteca | Uso |
|------------|-----|
| [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) | Driver do display ST7789 |
| [WiFiManager](https://github.com/tzapu/WiFiManager) | Portal de configuração Wi-Fi |
| [FluxGarage RoboEyes](https://github.com/FluxGarage/RoboEyes) | Animação dos olhinhos |

> **TFT_eSPI:** configure o `User_Setup.h` para TTGO T-Display antes de compilar.

---

## Compilar e Upload

Requer [arduino-cli](https://arduino.github.io/arduino-cli/) com o core `esp32:esp32` instalado.

```bash
# Compilar
arduino-cli compile --fqbn esp32:esp32:esp32 tamaclock/

# Upload (porta padrão: /dev/ttyACM0)
arduino-cli upload --fqbn esp32:esp32:esp32 --port /dev/ttyACM0 tamaclock/

# Monitor serial
arduino-cli monitor --port /dev/ttyACM0 --config baudrate=115200
```

---

## Configuração Wi-Fi

Na primeira inicialização (ou após reset de Wi-Fi):

1. O dispositivo cria uma rede Wi-Fi chamada **`TamaClock-Config`**
2. Conecte-se a ela pelo celular ou computador
3. Acesse `192.168.4.1` e insira as credenciais da sua rede
4. O dispositivo reinicia e conecta automaticamente

Para redefinir as credenciais: segure o **botão GPIO 0 por 3 segundos**.

---

## Licença

MIT — veja [LICENSE](LICENSE).
