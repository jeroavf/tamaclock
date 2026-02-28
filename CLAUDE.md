# TamaClock — Contexto para Claude Code

## O que é este projeto
Relógio digital com NTP e animação de olhinhos estilo Cozmo (Anki), rodando em TTGO T-Display (ESP32).

## Status atual
**Todas as fases concluídas (0–5). Código testado, funcionando no hardware e publicado no GitHub.**

Hardware validado em 2026-02-28. Repositório público: https://github.com/jeroavf/tamaclock

## Hardware
- Placa: **TTGO T-Display** (ESP32)
- Display: **TFT ST7789**, 1.14", **240x135px**, orientação **landscape**
- Driver: **TFT_eSPI**

## Stack de desenvolvimento
- **Arduino IDE** + **arduino-cli** (compile, upload, monitor serial)
- **WiFiManager** — portal de configuração Wi-Fi na 1ª inicialização (AP: `TamaClock-Config`)
- **configTime()** nativo ESP32 para NTP
- Fuso: **UTC-3 (BRT)**, sem horário de verão
- NTP server: `pool.ntp.org`, sync a cada **12h**

## Funcionalidades
1. **Relógio NTP** — layout estilo Volos: fundo preto, hora grande em branco, caixa âmbar de segundos centralizada, badge cyan do dia, caixa branca da data
2. **Animação Cozmo** — olhinhos com pálpebras + pupilas animadas, disparada a cada 2-5 min aleatório
   - Sequência: acorda → olha esquerda → olha direita → dorme → 10s de relógio → sorteia próximo timer
3. **WiFiManager** — portal web para configurar SSID/senha, credenciais salvas em flash
4. **Botões físicos:**
   - GPIO 35 (pressão curta): dispara animação Cozmo imediatamente
   - GPIO 35 (pressão longa ≥ 2s): alterna brilho manual DIA ↔ NOITE
   - GPIO 0 (pressão longa ≥ 3s): apaga credenciais WiFi e reinicia

## Referência visual
- Estilo **VolosR** (https://github.com/VolosR)
- Exemplo salvo em: `exemplos/Captura de tela de 2026-02-24 21-11-17.png`

## Estrutura de arquivos
```
tamaclock/               # raiz do repositório git
├── README.md            # documentação bilíngue (EN + PT)
├── LICENSE              # MIT
├── .gitignore
├── CLAUDE.md            # este arquivo
├── SPEC.md              # especificação detalhada
└── tamaclock/           # código-fonte Arduino
    ├── tamaclock.ino    # setup(), loop(), handleButtons()
    ├── clock_face.h/.cpp  # tela do relógio (estilo Volos)
    ├── eyes_anim.h/.cpp   # animação olhos Cozmo
    ├── ntp_sync.h/.cpp    # NTP e gerência de tempo
    ├── wifi_setup.h/.cpp  # WiFiManager + wifiReset()
    ├── tft_adapter.h      # adapter TFT_eSprite → RoboEyes
    └── config.h           # constantes globais
```

## Constantes principais (config.h)
| Constante | Valor | Descrição |
|-----------|-------|-----------|
| `TZ_OFFSET` | -3 | Fuso UTC-3 |
| `NTP_SYNC_INTERVAL_H` | 12 | Horas entre syncs NTP |
| `ANIM_MIN_INTERVAL_MS` | 120000 | 2 min |
| `ANIM_MAX_INTERVAL_MS` | 300000 | 5 min |
| `ANIM_POST_DELAY_MS` | 10000 | 10s pós-animação |
| `WIFI_AP_NAME` | "TamaClock-Config" | Nome do AP |
| `BTN_35` | 35 | Pino botão lateral |
| `BTN_0` | 0 | Pino botão BOOT |
| `BTN_LONG_MS` | 2000 | Threshold pressão longa btn35 |
| `BTN_WIFI_RESET_MS` | 3000 | Threshold reset WiFi btn0 |
| `BRIGHT_DAY` | 200 | Brilho diurno (0-255) |
| `BRIGHT_NIGHT` | 60 | Brilho noturno (0-255) |

## Comandos arduino-cli
```bash
# Compilar
arduino-cli compile --fqbn esp32:esp32:esp32 tamaclock/

# Upload (porta confirmada: ttyACM0)
arduino-cli upload --fqbn esp32:esp32:esp32 --port /dev/ttyACM0 tamaclock/

# Monitor serial
arduino-cli monitor --port /dev/ttyACM0 --config baudrate=115200
```

## Notas técnicas importantes
- **ESP32 core 3.x — API LEDC nova:** usar `ledcAttach(pin, freq, bits)` + `ledcWrite(pin, duty)`.
  `ledcSetup`/`ledcAttachPin` são da API antiga (core 2.x) e não compilam no 3.x.
- **Sprite compartilhado:** NUNCA criar dois sprites simultâneos — estoura heap silenciosamente.
  Um sprite 240×135×16bit = 64.800 bytes; heap livre após WiFi ≈ 126 KB.
- **Clock face — fontes:** Font7 (7-seg) para HH:MM **e** segundos. Font4 para badge do dia e data.
  Font7 suporta apenas dígitos — não usar para texto (dias, meses).
- **Caixa de segundos:** fillRoundRect(165,18,72,52) — centro (201,44) — Font7 TFT_BLACK MC_DATUM.
- **Badge dia:** fillRoundRect(5,93,62,37) — centro (36,111) — Font4 TFT_BLACK MC_DATUM.
- **Título "Tamaclock":** Font2, TFT_CYAN, TC_DATUM, x=120, y=2 (topo, acima do HH:MM).
- **Assinatura "by @jeroavf":** Font1, TFT_BLACK/TFT_WHITE, BR_DATUM, x=236, y=131 (canto inferior direito).
- **Botões:** GPIO 35 usa `INPUT` (pullup externo na placa). GPIO 0 usa `INPUT_PULLUP` (BOOT).
  Detecção no release — compara tempo de hold contra threshold para curto/longo.
- **Brilho manual:** flag `manualBright` desativa o controle automático por horário.

## Plano de implementação
- [x] Fase 0 — Especificação (concluída)
- [x] Fase 1 — Infraestrutura: projeto arduino-cli, TFT_eSPI, WiFiManager, NTP
- [x] Fase 2 — Tela do relógio (layout Volos com TFT_eSprite)
- [x] Fase 3 — Animação Cozmo (RoboEyes via TFTSpriteAdapter)
- [x] Fase 4 — Ajustes visuais: layout Volos, PWM backlight, autoblink, randomSeed
- [x] Fase 5 — Botões físicos: animação manual, toggle brilho, reset WiFi
- [x] Fase 6 — Design final: título "Tamaclock", assinatura "by @jeroavf", fontes revisadas
- [x] Fase 7 — Publicação: README bilíngue, LICENSE MIT, repositório GitHub

## Spec completa
Ver `SPEC.md` na raiz do projeto.
