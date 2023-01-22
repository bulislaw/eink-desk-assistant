#pragma once

// Set time zone
#define NTP_TZ (0) // UTC

// Set wifi location
#define AT_HOME

// Include secrets
#include "secrets.h"

#if !defined(WIFI_SSID) || !defined(WIFI_PASS)
#error "WiFI credentials not set."
#endif