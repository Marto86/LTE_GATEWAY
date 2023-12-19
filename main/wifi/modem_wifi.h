#pragma once

#include "esp_err.h"
#include "esp_wifi.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    wifi_mode_t mode;
    const char *ssid;
    const char *password;
    size_t channel;
    size_t max_connection;
} modem_wifi_config_t;

#define MODEM_WIFI_DEFAULT_CONFIG()          \
    {                                           \
        .mode = WIFI_MODE_AP,                 \
        .ssid = "LTEgateway",                  \
        .password = "12345678",                 \
        .channel = 13,                 \
        .max_connection = 10                 \
    }

esp_err_t modem_wifi_ap_init(void);
esp_err_t modem_wifi_set(modem_wifi_config_t *config);
esp_err_t modem_wifi_napt_enable();
esp_err_t modem_wifi_sta_connected(uint32_t wait_ms);
esp_err_t modem_wifi_set_dhcps(esp_netif_t *netif, uint32_t addr);
esp_netif_t *modem_wifi_init(wifi_mode_t mode); //will abandoned

#ifdef __cplusplus
}
#endif