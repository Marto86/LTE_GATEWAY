#include "wifi_access_point.h"
#include "esp_netif_types.h"

void start_access_point(esp_netif_t *ppp_netif)
{
    /* Initialize Wi-Fi. Start Wi-Fi AP */
    modem_wifi_config_t modem_wifi_config = MODEM_WIFI_DEFAULT_CONFIG();
    modem_wifi_config.max_connection = CONFIG_EXAMPLE_MAX_STA_CONN;
    modem_wifi_config.channel = CONFIG_EXAMPLE_WIFI_CHANNEL;
    modem_wifi_config.password = CONFIG_EXAMPLE_WIFI_PASSWORD;
    modem_wifi_config.ssid = CONFIG_EXAMPLE_WIFI_SSID;
    esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();
    assert(ap_netif != NULL);
#if CONFIG_EXAMPLE_MANUAL_DNS
    ESP_ERROR_CHECK(modem_wifi_set_dhcps(ap_netif, inet_addr(CONFIG_EXAMPLE_MANUAL_DNS_ADDR)));
    ESP_LOGI(TAG, "ap dns addr(manual): %s", CONFIG_EXAMPLE_MANUAL_DNS_ADDR);
#else /* using dns from ppp */
    esp_netif_dns_info_t dns;
    ESP_ERROR_CHECK(esp_netif_get_dns_info(ppp_netif, ESP_NETIF_DNS_MAIN, &dns));
    ESP_ERROR_CHECK(modem_wifi_set_dhcps(ap_netif, dns.ip.u_addr.ip4.addr));
    ESP_LOGI(TAG, "ap dns addr(auto): " IPSTR, IP2STR(&dns.ip.u_addr.ip4));
#endif
    ESP_ERROR_CHECK(modem_wifi_ap_init());
    ESP_ERROR_CHECK(modem_wifi_set(&modem_wifi_config));
    ESP_ERROR_CHECK(modem_wifi_napt_enable());
    set_led_behaviour(Wifi, On);
}

void stop_access_point()
{
    set_led_behaviour(Wifi, Off);
}