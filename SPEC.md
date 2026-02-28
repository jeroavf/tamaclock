# TamaClock — Especificação do Projeto

**Repositório:** https://github.com/jeroavf/tamaclock

## Visão Geral
Relógio digital com sincronização NTP e animação de olhinhos estilo Cozmo (Anki), exibido no display integrado do TTGO T-Display.

---

## Hardware

| Item | Detalhe |
|------|---------|
| Placa | TTGO T-Display (ESP32) |
| Display | TFT ST7789, 1.14", 240x135 px |
| Orientação | Paisagem (Landscape) — 240 largura x 135 altura |
| Conectividade | Wi-Fi integrado (ESP32) |

---

## Ambiente de Desenvolvimento

| Item | Detalhe |
|------|---------|
| IDE | Arduino IDE |
| Biblioteca display | TFT_eSPI (configurada para TTGO T-Display) |
| CLI | arduino-cli (compilação, upload, monitor serial) |
| Biblioteca Wi-Fi | WiFiManager |
| NTP | Biblioteca NTPClient ou configTime() nativo do ESP32 |

---

## Funcionalidades

### 1. Sincronização NTP
- Servidor: `pool.ntp.org`
- Fuso horário: UTC-3 (BRT — Brasil, sem horário de verão)
- Frequência de sync: **2 vezes por dia** (ao iniciar + a cada 12h)
- Fallback: exibe último horário salvo se Wi-Fi cair

### 2. Tela Principal — Relógio (estilo Volos)
Layout em landscape (240x135):
- **HH:MM**: Font7 (7-segmentos), cor branca, x=5, y=20 — elemento dominante
- **Segundos**: Font7 (7-segmentos), fundo âmbar, texto preto — caixa x=165,y=18,72x52px
- **Badge dia da semana**: Font4, fundo cyan, texto preto — caixa x=5,y=93,62x37px
- **Data**: Font4, fundo branco, texto azul escuro — caixa x=72,y=93,164x37px
- Fundo: **preto**
- Linha divisória branca em y=88
- Referência visual: estilo Volos (VolosR no GitHub)
- **Nota:** Font7 é 7-segmentos (somente dígitos). Font4 para qualquer texto alfanumérico.

### 3. Animação — Olhinhos Estilo Cozmo
**Trigger:** intervalo aleatório entre **2 e 5 minutos**

**Sequência de animação:**
1. **Acordando** — pálpebras abrem lentamente (de fechadas para abertas)
2. **Olha para a esquerda** — pupilas deslizam para esquerda, leve pausa
3. **Olha para a direita** — pupilas deslizam para direita, leve pausa
4. **Volta ao centro** — pupilas centralizam
5. **Dormindo** — pálpebras fecham lentamente

**Duração total da animação:** ~3-4 segundos

**Após a animação:**
- Aguarda **10 segundos** com tela de relógio visível
- Retorna ao ciclo normal (sorteia próximo intervalo aleatório)

**Design dos olhos (Cozmo completo):**
- Dois olhos lado a lado, centralizados no display
- Cada olho: esclera branca arredondada + íris colorida (cyan ou azul) + pupila preta
- Pálpebra superior e inferior animadas independentemente
- Expressão neutra ao acordar → curiosidade ao olhar os lados

---

## Configuração Wi-Fi

- Na primeira inicialização (ou sem credenciais salvas): entra em modo AP
- Usuário conecta no celular à rede `TamaClock-Config`
- Portal web em `192.168.4.1` para inserir SSID e senha
- Credenciais salvas em memória flash (EEPROM/Preferences)
- Nas próximas inicializações: conecta automaticamente

---

## Botões Físicos

| Botão | Pino | Ação | Threshold |
|-------|------|------|-----------|
| Lateral | GPIO 35 | Pressão curta: dispara animação Cozmo | < 2s |
| Lateral | GPIO 35 | Pressão longa: alterna brilho DIA ↔ NOITE | ≥ 2s |
| BOOT | GPIO 0 | Pressão longa: apaga credenciais WiFi e reinicia | ≥ 3s |

- GPIO 35: `INPUT` (pullup externo na placa, input-only pin)
- GPIO 0: `INPUT_PULLUP` (botão BOOT, pullup interno ESP32)
- Detecção no release do botão; ação baseada no tempo de hold
- Brilho manual via botão desativa o controle automático por horário

---

## Estrutura de Arquivos do Projeto

```
tamaclock/
├── tamaclock.ino          # Arquivo principal, setup() e loop()
├── clock_face.h/.cpp      # Desenho da tela de relógio (estilo Volos)
├── eyes_anim.h/.cpp       # Animação dos olhinhos Cozmo
├── ntp_sync.h/.cpp        # Sincronização NTP e gerência de tempo
├── wifi_setup.h/.cpp      # WiFiManager e configuração de rede
└── config.h               # Constantes globais (cores, pinos, intervalos)
```

---

## Fluxo Principal (Loop)

```
BOOT
 └─> WiFiManager (conecta ou abre portal)
      └─> NTP sync inicial
           └─> LOOP:
                ├─> Exibe relógio (atualiza a cada segundo)
                ├─> Timer aleatório (2-5 min) expira?
                │    └─> SIM → Executa animação Cozmo (~4s)
                │              └─> Exibe relógio por 10s
                │                   └─> Sorteia próximo timer
                └─> A cada 12h → NTP sync
```

---

## Constantes Configuráveis (`config.h`)

| Constante | Valor padrão | Descrição |
|-----------|-------------|-----------|
| `TZ_OFFSET` | -3 | Fuso horário em horas |
| `NTP_SYNC_INTERVAL_H` | 12 | Horas entre syncs NTP |
| `ANIM_MIN_INTERVAL_MS` | 120000 | 2 min em ms |
| `ANIM_MAX_INTERVAL_MS` | 300000 | 5 min em ms |
| `ANIM_POST_DELAY_MS` | 10000 | 10s de relógio pós-animação |
| `WIFI_AP_NAME` | "TamaClock-Config" | Nome da rede AP |
| `BTN_35` | 35 | Pino botão lateral |
| `BTN_0` | 0 | Pino botão BOOT |
| `BTN_LONG_MS` | 2000 | Threshold pressão longa btn35 (ms) |
| `BTN_WIFI_RESET_MS` | 3000 | Threshold reset WiFi btn0 (ms) |
| `BRIGHT_DAY` | 200 | Brilho diurno (0-255) |
| `BRIGHT_NIGHT` | 60 | Brilho noturno (0-255) |
| `NIGHT_START` | 22 | Hora início brilho reduzido |
| `NIGHT_END` | 7 | Hora fim brilho reduzido |

---

## Plano de Implementação

### Fase 1 — Infraestrutura ✅
- [x] Configurar projeto arduino-cli (board TTGO T-Display)
- [x] Configurar TFT_eSPI para ST7789 (User_Setup.h)
- [x] Implementar WiFiManager + conexão
- [x] Implementar NTP sync e leitura de hora

### Fase 2 — Tela do Relógio ✅
- [x] Layout Volos: hora grande, segundos, data, badge dia
- [x] Atualização suave a cada segundo (sem flickering via sprite)

### Fase 3 — Animação Cozmo ✅
- [x] Desenhar olhos base (esclera, íris, pupila)
- [x] Animar pálpebras (abrir/fechar)
- [x] Animar pupilas (esquerda/direita)
- [x] Compor sequência completa via FluxGarage RoboEyes

### Fase 4 — Ajustes Visuais ✅
- [x] Timer aleatório no loop principal
- [x] Transições relógio ↔ animação
- [x] PWM backlight automático por horário (DIA/NOITE)
- [x] Autoblink, randomSeed

### Fase 5 — Botões Físicos ✅
- [x] GPIO 35 curto: dispara animação manualmente
- [x] GPIO 35 longo: toggle brilho DIA ↔ NOITE
- [x] GPIO 0 longo: reset WiFi + reinício

### Fase 6 — Design Final ✅
- [x] Título "Tamaclock" centralizado no topo (Font2, cyan, y=2)
- [x] Assinatura "by @jeroavf" canto inferior direito (Font1, preto/branco, y=131)
- [x] Segundos com Font7 (igual HH:MM), caixa ajustada (165,18,72,52)
- [x] Badge dia com Font4 (igual data), caixa ajustada (5,93,62,37)

### Fase 7 — Publicação ✅
- [x] README bilíngue (EN + PT) com layout, botões, dependências, referências
- [x] Licença MIT
- [x] Repositório Git local (branch main)
- [x] Repositório GitHub público: https://github.com/jeroavf/tamaclock
- [x] 12 topics: esp32, arduino, ttgo-t-display, ntp-clock, digital-clock, tft-espi, wifimanager, animation, iot, embedded, st7789, esp32-arduino

---

## Comandos arduino-cli (referência)

```bash
# Compilar
arduino-cli compile --fqbn esp32:esp32:esp32 tamaclock/

# Upload (porta confirmada: ttyACM0)
arduino-cli upload --fqbn esp32:esp32:esp32 --port /dev/ttyACM0 tamaclock/

# Monitor serial
arduino-cli monitor --port /dev/ttyACM0 --config baudrate=115200
```
