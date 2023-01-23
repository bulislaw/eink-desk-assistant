#pragma once

// Set how often to update the data and the screen
#define UPDATE_INTERVAL_MS (2 * 60 * 1000) // 2min = 2 * 60 sec * 1000 ms -> Should be good for dev

// Set time zone
#define NTP_TZ (0) // UTC

// Set wifi location
#define AT_HOME

// Include secrets
#include "secrets.h"

#if !defined(WIFI_SSID) || !defined(WIFI_PASS)
#error "WiFI credentials not set."
#endif