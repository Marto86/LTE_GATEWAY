#pragma once

#include "esp_err.h"
#include "esp_netif.h"

extern bool wifi_active;
extern bool LTE4G_active;


#ifdef __cplusplus
extern "C"
{
#endif

#define LED_RED_SYSTEM_GPIO                 CONFIG_LED_RED_SYSTEM_GPIO
#define LED_BLUE_WIFI_GPIO                  CONFIG_LED_BLUE_WIFI_GPIO
#define LED_GREEN_4GMODEM_GPIO              CONFIG_LED_GREEN_4GMODEM_GPIO
#define MODEM_POWER_GPIO                    CONFIG_MODEM_POWER_GPIO
#define MODEM_RESET_GPIO                    CONFIG_MODEM_RESET_GPIO

#define MODEM_DEFAULT_CONFIG()\
    {                                \
        .rx_buffer_size = 1024*15,   \
        .tx_buffer_size = 1024*15,   \
        .line_buffer_size = 1600,    \
        .event_task_priority = CONFIG_USB_TASK_BASE_PRIORITY + 3,\
        .event_task_stack_size = 3072\
    }


typedef struct {
    int rx_buffer_size;             /*!< USB RX Buffer Size */
    int tx_buffer_size;             /*!< USB TX Buffer Size */
    int line_buffer_size;           /*!< Line buffer size for command mode */
    int event_task_priority;        /*!< USB Event/Data Handler Task Priority*/
    int event_task_stack_size;      /*!< USB Event/Data Handler Task Stack Size*/
} modem_config_t;

esp_netif_t *modem_board_init(modem_config_t *config);
esp_err_t modem_board_force_reset(void);
esp_err_t modem_board_on(void);

#ifdef __cplusplus
}
#endif