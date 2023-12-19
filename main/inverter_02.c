#include "string.h"
#include "mqtt_api.h"
#include "inverter_02.h"
#include "mbcontroller.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "./board_devices/board_leds.h"
#include "board_button.h"
#include "mqtt_api.h"
#include "poly_routing.h"

// Note: Some pins on target chip cannot be assigned for UART communication.
// See UART documentation for selected board and target to configure pins using Kconfig.

// Timeout to update cid over Modbus
#define UPDATE_CIDS_TIMEOUT_MS (500)
#define UPDATE_CIDS_TIMEOUT_TICS (UPDATE_CIDS_TIMEOUT_MS / portTICK_RATE_MS)

// Timeout between polls
#define POLL_TIMEOUT_MS (1)
#define POLL_TIMEOUT_TICS (POLL_TIMEOUT_MS / portTICK_RATE_MS)

static const char *TAG = "INVERTER_02";

// Here are the user defined instances for device parameters packed by 1 byte
// These keep the values that can be accessed from Modbus master
inverter_02_status_t inverter_02_status = {0};

inverter_02_errors_t inverter_02_errors = {0};

inverter_02_control_t inverter_02_control = {0};


static bool had_error_previous_time = false;
// User operation function to read slave values and check alarm
void inverter_02_operate(inverter_02_key_id_t key_id, void *mqtt_client, bool force_resend, ContainerTypeEnum container_type)
{
    uart_flush(CONFIG_MB_UART_PORT_NUM);
    uart_set_baudrate(CONFIG_MB_UART_PORT_NUM, HP_INV_02_BAUD_RATE);
    uart_set_parity(CONFIG_MB_UART_PORT_NUM, UART_PARITY_DISABLE);

    esp_err_t err = ESP_OK;
    bool modbus_error = false;
    uint16_t value = 0;
    bool value_changed = false;
    const mb_parameter_descriptor_t *param_descriptor = NULL;

    ESP_LOGI(TAG, "Data key : %d ", key_id);

    // Read all found characteristics from slave(s)
    for (uint16_t cid = CID_HP_INV02_OFFSET; (err != ESP_ERR_NOT_FOUND) && cid <= CID_HP_INV02_MAX_NUMBER; cid++)
    {

        // Get data from parameters description table
        // and use this information to fill the characteristics description table
        // and having all required fields in just one table
        err = mbc_master_get_cid_info(cid, &param_descriptor);

        if ((err != ESP_ERR_NOT_FOUND) && (param_descriptor != NULL) && (key_id == OPT_KEY_ID(param_descriptor->param_opts)))
        {
            uint8_t type = 0;
            uint16_t retry = 0;

            err = mbc_master_get_parameter(cid, (char *)param_descriptor->param_key,
                                           (uint8_t *)&value, &type);

            while (err != ESP_OK && retry++ <= MODBUS_READ_RETRY_COUNT)
            {
                vTaskDelay(pdMS_TO_TICKS(MODBUS_READ_RETRY_DELAY));

                err = mbc_master_get_parameter(cid, (char *)param_descriptor->param_key,
                                               (uint8_t *)&value, &type);
            }

            if (err == ESP_OK)
            {
                
                uint16_t *data = (uint16_t *)param_descriptor->param_opts.opt3;
                assert(data);

                // If the data is requested, then add it for sending over MQTT
                if (*data != value || force_resend)
                {
                    set_led_behaviour(Modbus, Off);
                    set_led_behaviour(LTE4G, Off);
                    set_led_behaviour(Wifi, Off);
                    // ESP_LOGI(TAG, "Reported  key_id: %d, bit:%04X, mask:%04X, data: %d ",
                    //          OPT_KEY_ID(param_descriptor->param_opts), OPT_BIT_IX(param_descriptor->param_opts), param_descriptor->param_opts.opt2, *data);
                    mqtt_data_add(OPT_KEY_ID(param_descriptor->param_opts), OPT_BIT_IX(param_descriptor->param_opts), value, param_descriptor->param_opts.opt2);
                    value_changed = true;
                }

                ESP_LOGI(TAG, "%s = %d ",
                         (char *)param_descriptor->param_key,
                         value);

                *data = value;
            }
            else
            {
                modbus_error = true;
                ESP_LOGE(TAG, "Characteristic #%d (%s) read fail, err = 0x%x (%s).",
                         param_descriptor->cid,
                         (char *)param_descriptor->param_key,
                         (int)err,
                         (char *)esp_err_to_name(err));
            }

            vTaskDelay(POLL_TIMEOUT_TICS); // timeout between polls
        }
    }

    // Send communication error message over MQTT
    if (modbus_error)
    {
        if (!had_error_previous_time)
        {
            mqtt_data_add(KEY_ID_HP_INV02_ERRORS, HP_MODBUS_COMMUNICATION_ERROR, modbus_error, 0xFFFF);
            mqtt_data_send(mqtt_client, KEY_ID_HP_INV02_WORK_DATA);
            set_led_behaviour(Modbus, FastBlinking);
        }
        had_error_previous_time = true;
    }
    else
    {
        if (had_error_previous_time)
        {
            set_led_behaviour(Modbus, StopFastBlinking);
        }
        had_error_previous_time = false;
    }

    set_led_behaviour(Modbus, On);
    if (value_changed || force_resend)
    {
        mqtt_data_send(mqtt_client, container_type);
    }
}
