#pragma once
#include <time.h>

// Inicializa NTP e aguarda primeira sincronização.
void ntpInit();

// Força re-sync NTP imediato.
void ntpSync();

// Chama ntpSync() se já se passaram NTP_SYNC_INTERVAL_MS desde o último sync.
// Retorna true se o sync foi disparado.
bool ntpSyncIfDue();

// Retorna o horário local atual (struct tm preenchida).
// Retorna struct zerada se ainda não sincronizou.
struct tm ntpGetTime();
