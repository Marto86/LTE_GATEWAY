#include "string.h"
#include "esp_log.h"
#include "mqtt_api.h"
#include "modbus.h"
#include "mbcontroller.h"
#include "sdkconfig.h"
#include "appconfig.h"

#define MB_PORT_NUM (CONFIG_MB_UART_PORT_NUM)   // Number of UART port used for Modbus connection
#define MB_DEV_SPEED (CONFIG_MB_UART_BAUD_RATE) // The communication speed of the UART

// Active Modbus Devise after TEST
bool rak_is_active;
bool inv_is_active;
bool salt_is_active;
// Dignostic mode status 
bool dignostic_mode;
// Note: Some pins on target chip cannot be assigned for UART communication.
// See UART documentation for selected board and target to configure pins using Kconfig.


static const char *TAG = "MODBUS";

const mb_parameter_descriptor_t modbus_param_descriptor[] = {
#include <inverter_02_descriptor.h>
#include <heatpump_rak_descriptor.h>
#include <salt_cid_descriptor.h>

};

// Modbus master initialization
int modbus_init(void)
{
    // Initialize and start Modbus controller
    mb_communication_info_t comm = {
        .port = MB_PORT_NUM,
        .mode = MB_MODE_RTU,
        .baudrate = MB_DEV_SPEED,
        .parity = UART_PARITY_EVEN};
    void *master_handler = NULL;

    esp_err_t err = mbc_master_init(MB_PORT_SERIAL_MASTER, &master_handler);
    MB_RETURN_ON_FALSE((master_handler != NULL), ESP_ERR_INVALID_STATE, TAG,
                       "mb controller initialization fail.");
    MB_RETURN_ON_FALSE((err == ESP_OK), ESP_ERR_INVALID_STATE, TAG,
                       "mb controller initialization fail, returns(0x%x).",
                       (uint32_t)err);
    err = mbc_master_setup((void *)&comm);
    MB_RETURN_ON_FALSE((err == ESP_OK), ESP_ERR_INVALID_STATE, TAG,
                       "mb controller setup fail, returns(0x%x).",
                       (uint32_t)err);

    // Set UART pin numbers
    err = uart_set_pin(MB_PORT_NUM, CONFIG_MB_UART_TXD, CONFIG_MB_UART_RXD,
                       CONFIG_MB_UART_RTS, UART_PIN_NO_CHANGE);

    err = mbc_master_start();
    MB_RETURN_ON_FALSE((err == ESP_OK), ESP_ERR_INVALID_STATE, TAG,
                       "mb controller start fail, returns(0x%x).",
                       (uint32_t)err);

    MB_RETURN_ON_FALSE((err == ESP_OK), ESP_ERR_INVALID_STATE, TAG,
                       "mb serial set pin failure, uart_set_pin() returned (0x%x).", (uint32_t)err);
    // Set driver mode to Half Duplex
    err = uart_set_mode(MB_PORT_NUM, UART_MODE_RS485_HALF_DUPLEX);
    MB_RETURN_ON_FALSE((err == ESP_OK), ESP_ERR_INVALID_STATE, TAG,
                       "mb serial set mode failure, uart_set_mode() returned (0x%x).", (uint32_t)err);

    // This should be called everytime when we need to communicate with the right device
    // uart_flush(MB_PORT_NUM);
    // uart_set_baudrate(MB_PORT_NUM, MB_DEV_SPEED);
    // uart_set_parity(MB_PORT_NUM, UART_PARITY_EVEN);

    vTaskDelay(5);
    ESP_LOGI(TAG, "Total CIDs count: %d", MODBUS_CID_TOTAL_NR);
    err = mbc_master_set_descriptor(modbus_param_descriptor, MODBUS_CID_TOTAL_NR);

    MB_RETURN_ON_FALSE((err == ESP_OK), ESP_ERR_INVALID_STATE, TAG,
                       "mb controller set descriptor fail, returns(0x%x).",
                       (uint32_t)err);
    ESP_LOGI(TAG, "Modbus master stack initialized...");
    return err;
}

/**
 * IMPORTANT: This function should be called ONLY from <>_operate from the specific modbus peripheral.
 * This is because every periphjeral has oits own UART speed/parity and it will not be working if it is envoked for another peripheral
 **/
bool modbus_test(uint16_t cid, uint32_t baud_rate, uart_parity_t parity)
{

    uart_flush(CONFIG_MB_UART_PORT_NUM);
    uart_set_baudrate(CONFIG_MB_UART_PORT_NUM, baud_rate);
    uart_set_parity(CONFIG_MB_UART_PORT_NUM, parity);

    uint16_t retry = 0;
    esp_err_t err = ESP_OK;
    uint16_t value = 0;
    const mb_parameter_descriptor_t *param_descriptor = NULL;

    err = mbc_master_get_cid_info(cid, &param_descriptor);

    if (err != ESP_OK)
        return false;

    uint8_t type = 0;

    err = mbc_master_get_parameter(cid, (char *)param_descriptor->param_key,
                                   (uint8_t *)&value, &type);

    while (err != ESP_OK && retry++ <= MODBUS_READ_RETRY_COUNT)
    {
        vTaskDelay(pdMS_TO_TICKS(MODBUS_READ_RETRY_DELAY));

        err = mbc_master_get_parameter(cid, (char *)param_descriptor->param_key,
                                       (uint8_t *)&value, &type);
    }

    if (err != ESP_OK)
    {
        return false;
    }
    return true;
}

static QueueHandle_t modbus_commands_queue_handle;

void modbus_commands_queue_init()
{
    modbus_commands_queue_handle = xQueueCreate(20, sizeof(int));
}

void modbus_commands_queue_push(int command_id)
{
    xQueueSend(modbus_commands_queue_handle, &command_id, (TickType_t)0);
}

void modbus_execute_pending_commands(void *client)
{
    int type = -1;
    while(xQueueReceive(modbus_commands_queue_handle, &type, (TickType_t)5))
    {
        if (type == WorkingGetDataCommand)
        {
            if (rak_is_active)
            {
                heatpump_rak_operate(KEY_ID_HP_RAK_WORK_DATA, client, true, workRak01);
                heatpump_rak_operate(KEY_ID_HP_RAK_ERRORS, client, true, workRak01);
            }
            if (salt_is_active)
            {
                salt_operate(KEY_ID_SALT_WORK_DATA, client, true, workInv02);
            }
            if (inv_is_active)
            {
                inverter_02_operate(KEY_ID_HP_INV02_WORK_DATA, client, true, workInv02);
                inverter_02_operate(KEY_ID_HP_INV02_ERRORS, client, true, workInv02);
            }
        }
        else if (type == ManufacturingGetDataCommand)
        {  
            if (rak_is_active)
            {
                heatpump_rak_operate(KEY_ID_HP_RAK_MANUFACTURER_DATA, client, true, manuRak01);
                heatpump_rak_operate(KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA, client, true, manuRak01);   
                heatpump_rak_operate(KEY_ID_HP_RAK_COMPRESSOR_MNUFACTURER_DATA, client, true, manuRak01);
               
            }
            if (inv_is_active)
            {
                inverter_02_operate(KEY_ID_HP_INV02_MANUFACTURER_DATA, client, true, manuInv02);
                inverter_02_operate(KEY_ID_HP_INV02_MANUAL_MANUFACTURER_DATA, client, true, manuInv02);
               
            }
        
        }
        else if (type == ConfigurationGetDataCommand)
        {
            if (rak_is_active)
            {
                heatpump_rak_operate(KEY_ID_HP_RAK_GENERAL_CONFIGURATION, client, true, configRak01); // ok
                heatpump_rak_operate(KEY_ID_HP_RAK_COMPRESSOR_CONFIGURATION_DATA_0, client, true, configRak01);
                heatpump_rak_operate(KEY_ID_HP_RAK_DEFROSTING_CONFIGURATION_DATA, client, true, configRak01);
                heatpump_rak_operate(KEY_ID_HP_RAK_TEMPERATURE_CONFIGURATION_DATA, client, true, configRak01);
            }
            if (salt_is_active)
            {
                salt_operate(KEY_ID_SALT_CONFIGURATION, client, true, configInv02);
            }
            if (inv_is_active)
            {
                inverter_02_operate(KEY_ID_HP_INV02_CONFIGURATION_FAN, client, true, configInv02);
                inverter_02_operate(KEY_ID_HP_INV02_COMPRESSOR_CONFIGURATION_DATA_0, client, true, configInv02);
                inverter_02_operate(KEY_ID_HP_INV02_COMPRESSOR_CONFIGURATION_DATA_1, client, true, configInv02);
                inverter_02_operate(KEY_ID_HP_INV02_GENERAL_CONFIGURATION, client, true, configInv02);
                inverter_02_operate(KEY_ID_HP_INV02_DEFROSTING_CONFIGURATION_DATA, client, true, configInv02);
                inverter_02_operate(KEY_ID_HP_INV02_TEMPERATURE_CONFIGURATION_DATA, client, true, configInv02);
            }
        }
    }
}

void apply_out_topic_messages()
{
    // Check for updates from out topic and apply tyhem
    // QueueHandle_t out_data_queue;
    modbus_update_item_t update_item;
    if (out_data_queue != 0)
    {
        // Receive a message on the created queue.  Block for 10 ticks if a
        // message is not immediately available.

        const mb_parameter_descriptor_t *param_descriptor = NULL;
        esp_err_t err = ESP_OK;
        while (xQueueReceive(out_data_queue, &update_item, (TickType_t)10))
        {
            uint16_t cid = update_item.cid;
            ESP_LOGI(TAG, "GU mbc_master_get_cid_info : %d", cid);
            err = mbc_master_get_cid_info(cid, &param_descriptor);
            if (err == ESP_OK)
            {
                uint8_t type = 0;
                uint16_t value = update_item.value;
                ESP_LOGI(TAG, "Try MB update cid: %d, value: %d, param key: %s", cid, value, (char *)param_descriptor->param_key);
                err = mbc_master_set_parameter(cid, (char *)param_descriptor->param_key, (uint8_t *)&value, &type);
                if (err != ESP_OK)
                {
                    ESP_LOGE(TAG, "Function called mbc_master_set_parameter! Error setting modbust data for cid: %d!", cid);
                }
            }
            else
            {
                ESP_LOGE(TAG, "Function called mbc_master_get_cid_info! Error getting modbust data for cid: %d!", cid);
            }
        }
    }
}
