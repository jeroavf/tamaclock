# TamaClock

---

## 🇺🇸 English

NTP digital clock with Cozmo-style (Anki) eye animation, running on the **TTGO T-Display** (ESP32).

![TamaClock](exemplos/Captura%20de%20tela%20de%202026-02-24%2021-11-17.png)

### Features

- **NTP Clock** — auto-sync with `pool.ntp.org`, UTC-3 (BRT) timezone, re-sync every 12h
- **Volos-style layout** — large 7-segment HH:MM, amber seconds box, day badge and date
- **Cozmo animation** — animated eyelids and pupils, triggered automatically every 2–5 minutes
- **WiFiManager** — web configuration portal on first boot; credentials saved to flash
- **Auto backlight** — reduced brightness between 10 PM and 7 AM; manual override via button
- **Physical buttons** — control animation, brightness and Wi-Fi reset

### Hardware

| Item | Detail |
|------|--------|
| Board | TTGO T-Display (ESP32) |
| Display | TFT ST7789, 1.14", 240×135 px |
| Orientation | Landscape |
| Connectivity | Built-in Wi-Fi |

### Display Layout

```
┌──────────────────────────────────────┐
│           Tamaclock                  │  ← title (cyan)
│  HH:MM              [SS]             │  ← time (white) + seconds (amber)
│                                      │
├──────────────────────────────────────┤
│  [DAY]   MON, DD        by @jeroavf │  ← day badge + date + signature
└──────────────────────────────────────┘
```

### Buttons

| Button | Action | Threshold |
|--------|--------|-----------|
| GPIO 35 — short press | Trigger Cozmo animation immediately | < 2 s |
| GPIO 35 — long press | Toggle manual brightness DAY ↔ NIGHT | ≥ 2 s |
| GPIO 0 — long press | Erase Wi-Fi credentials and reboot | ≥ 3 s |

### Project Structure

```
tamaclock/
├── tamaclock.ino       # setup(), loop(), handleButtons()
├── clock_face.h/.cpp   # clock screen (Volos style)
├── eyes_anim.h/.cpp    # Cozmo eye animation (FluxGarage RoboEyes)
├── ntp_sync.h/.cpp     # NTP sync
├── wifi_setup.h/.cpp   # WiFiManager + wifiReset()
├── tft_adapter.h       # TFT_eSprite → RoboEyes adapter
└── config.h            # global constants
```

### Dependencies

Install via Arduino Library Manager:

| Library | Purpose |
|---------|---------|
| [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) | ST7789 display driver |
| [WiFiManager](https://github.com/tzapu/WiFiManager) | Wi-Fi configuration portal |
| [FluxGarage RoboEyes](https://github.com/FluxGarage/RoboEyes) | Eye animation |

> **TFT_eSPI:** configure `User_Setup.h` for TTGO T-Display before compiling.

### Build & Upload

Requires [arduino-cli](https://arduino.github.io/arduino-cli/) with the `esp32:esp32` core installed.

```bash
# Compile
arduino-cli compile --fqbn esp32:esp32:esp32 tamaclock/

# Upload (default port: /dev/ttyACM0)
arduino-cli upload --fqbn esp32:esp32:esp32 --port /dev/ttyACM0 tamaclock/

# Serial monitor
arduino-cli monitor --port /dev/ttyACM0 --config baudrate=115200
```

### Wi-Fi Setup

On first boot (or after a Wi-Fi reset):

1. The device creates a Wi-Fi network called **`TamaClock-Config`**
2. Connect to it from your phone or computer
3. Open `192.168.4.1` and enter your network credentials
4. The device reboots and connects automatically

To reset credentials: hold **GPIO 0 button for 3 seconds**.

### References & Acknowledgements

This project stands on the shoulders of great open-source work:

- **[TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)** by [Bodmer](https://github.com/Bodmer) — the excellent display driver that makes smooth rendering possible on ESP32 with ST7789.

- **[WiFiManager](https://github.com/tzapu/WiFiManager)** by [tzapu](https://github.com/tzapu) — hassle-free Wi-Fi provisioning with a captive portal, no hardcoded credentials.

- **[FluxGarage RoboEyes](https://github.com/FluxGarage/RoboEyes)** by [FluxGarage](https://github.com/FluxGarage) — the delightful animated eye library that brings the Cozmo personality to life.

- **[VolosR](https://github.com/VolosR)** — visual design inspiration for the Volos-style clock layout (large 7-segment digits, color blocks, dark background).

- **[Espressif ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)** — the foundation that makes Arduino development on ESP32 possible.

### License

MIT — see [LICENSE](LICENSE).

---

## 🇧🇷 Português

Relógio digital NTP com animação de olhinhos estilo Cozmo (Anki), rodando no **TTGO T-Display** (ESP32).

### Funcionalidades

- **Relógio NTP** — sincronização automática com `pool.ntp.org`, fuso UTC-3 (BRT), re-sync a cada 12h
- **Layout estilo Volos** — HH:MM em fonte 7-segmentos grande, caixa de segundos âmbar, badge do dia e data
- **Animação Cozmo** — olhinhos com pálpebras e pupilas animadas, disparada automaticamente a cada 2–5 minutos
- **WiFiManager** — portal web de configuração Wi-Fi na 1ª inicialização; credenciais salvas em flash
- **Backlight automático** — brilho reduzido entre 22h e 7h; override manual pelo botão
- **Botões físicos** — controle de animação, brilho e reset de Wi-Fi

### Hardware

| Item | Detalhe |
|------|---------|
| Placa | TTGO T-Display (ESP32) |
| Display | TFT ST7789, 1.14", 240×135 px |
| Orientação | Landscape |
| Conectividade | Wi-Fi integrado |

### Layout do Display

```
┌──────────────────────────────────────┐
│           Tamaclock                  │  ← título (cyan)
│  HH:MM              [SS]             │  ← hora (branco) + segundos (âmbar)
│                                      │
├──────────────────────────────────────┤
│  [DIA]   MÊS, DD        by @jeroavf │  ← badge dia + data + assinatura
└──────────────────────────────────────┘
```

### Botões

| Botão | Ação | Threshold |
|-------|------|-----------|
| GPIO 35 — pressão curta | Dispara animação Cozmo imediatamente | < 2 s |
| GPIO 35 — pressão longa | Alterna brilho manual DIA ↔ NOITE | ≥ 2 s |
| GPIO 0 — pressão longa | Apaga credenciais Wi-Fi e reinicia | ≥ 3 s |

### Estrutura do Projeto

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

### Dependências

Instalar via Arduino Library Manager:

| Biblioteca | Uso |
|------------|-----|
| [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) | Driver do display ST7789 |
| [WiFiManager](https://github.com/tzapu/WiFiManager) | Portal de configuração Wi-Fi |
| [FluxGarage RoboEyes](https://github.com/FluxGarage/RoboEyes) | Animação dos olhinhos |

> **TFT_eSPI:** configure o `User_Setup.h` para TTGO T-Display antes de compilar.

### Compilar e Upload

Requer [arduino-cli](https://arduino.github.io/arduino-cli/) com o core `esp32:esp32` instalado.

```bash
# Compilar
arduino-cli compile --fqbn esp32:esp32:esp32 tamaclock/

# Upload (porta padrão: /dev/ttyACM0)
arduino-cli upload --fqbn esp32:esp32:esp32 --port /dev/ttyACM0 tamaclock/

# Monitor serial
arduino-cli monitor --port /dev/ttyACM0 --config baudrate=115200
```

### Configuração Wi-Fi

Na primeira inicialização (ou após reset de Wi-Fi):

1. O dispositivo cria uma rede Wi-Fi chamada **`TamaClock-Config`**
2. Conecte-se a ela pelo celular ou computador
3. Acesse `192.168.4.1` e insira as credenciais da sua rede
4. O dispositivo reinicia e conecta automaticamente

Para redefinir as credenciais: segure o **botão GPIO 0 por 3 segundos**.

### Referências e Agradecimentos

Este projeto foi construído sobre o trabalho de grandes projetos open-source:

- **[TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)** por [Bodmer](https://github.com/Bodmer) — excelente driver de display que viabiliza renderização suave no ESP32 com ST7789.

- **[WiFiManager](https://github.com/tzapu/WiFiManager)** por [tzapu](https://github.com/tzapu) — configuração de Wi-Fi sem esforço via portal captivo, sem credenciais no código.

- **[FluxGarage RoboEyes](https://github.com/FluxGarage/RoboEyes)** por [FluxGarage](https://github.com/FluxGarage) — a encantadora biblioteca de olhos animados que dá personalidade Cozmo ao projeto.

- **[VolosR](https://github.com/VolosR)** — inspiração visual para o layout de relógio estilo Volos (dígitos 7-segmentos grandes, blocos coloridos, fundo escuro).

- **[Espressif ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)** — a base que torna possível o desenvolvimento Arduino no ESP32.

### Licença

MIT — veja [LICENSE](LICENSE).
