#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_spi_flash.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_netif.h"
#include "gateway_credentials.h"

#include "modem_board.h"
#include "./wifi/modem_wifi.h"
#include <esp_wifi.h>
#include <esp_event.h>

#include "heatpump_rak.h"
#include "inverter_02.h"

#ifdef CONFIG_CDC_USE_TRACE_FACILITY
#include "esp_usbh_cdc.h"
#endif

#include "modbus.h"
#include "mqtt.h"
#include "appconfig.h"
#include "esp_http_server.h"
#include "./board_devices/board_button.h"
#include "./board_devices/board_leds.h"
#include "./polytropic/poly_routing.h"
#include "esp_usbh_cdc.h"

#include "./wifi/wifi_access_point.h"
#include "./wifi/wifi_station.h"
#include "./wifi/http_web_server.h"

#include "./ingeli_nvs.h"
#include "sntp.h"
#include "mqtt_api.h"
#include "ble_provisioning.h"
#include "wps.h"
#include "history_log.h"

static const char *TAG = "main";
static void *mqtt_client;
static esp_netif_t *ppp_netif;
static bool had_error_previous_time = false;
bool modbus_error = false;
#ifdef RUN_TESTS
#include "./test/run_tests.h"
void app_main(void)
{
    run_tests();
}
#else
void app_main(void)
{
    history_log_init();
    wifi_active = false;
    LTE4G_active = false;
    initialize_board_button();
    initialize_board_leds();
    set_led_behaviour(Power, On);
    set_led_behaviour(LTE4G, On);
    set_led_behaviour(Wifi, On);
    ingeli_initialize_nvs();
    poly_init_routes();
    set_led_behaviour(LTE4G, Off);
    set_led_behaviour(Wifi, Off);
    set_led_behaviour(Modbus, Off);
    set_led_behaviour(Modbus, FastBlinking);

    modem_board_on();

    /* Initialize default TCP/IP stack */
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialization of device peripheral and objects

    modbus_init();
    vTaskDelay(pdMS_TO_TICKS(1500));

    // set_led_behaviour(Modbus, StopFastBlinking);
    const char *device_serial_number = gateway_credentials_get_string(SERIAL_NUMBER);
    const char *device_uuid_number = gateway_credentials_get_string(UUID);
    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "    Ingeli 4G LTE-Modbus Gateway    ");
    ESP_LOGI(TAG, "    Firmware version %s    ", FIRMWARE_VERSION);
    ESP_LOGI(TAG, "====================================");

    ESP_LOGI(TAG, "GWID_SN:%s", device_serial_number);
    ESP_LOGI(TAG, "GWID_UUID:%s", device_uuid_number);
    ESP_LOGI(TAG, "GWID_Firmware:%s", FIRMWARE_VERSION);

    modem_config_t modem_config = MODEM_DEFAULT_CONFIG();
    vTaskDelay(pdMS_TO_TICKS(5000));
    ppp_netif = modem_board_init(&modem_config);

    // check if 4G stick is pluged and is connected to network
    // otherwise use WIFI
    if (ppp_netif == NULL)
    {
        // ESP_LOGW(TAG, "Wfi is : %d", wifi_active);
        set_led_behaviour(LTE4G, Off);
        esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
        assert(sta_netif);

        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));

        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_start());
        set_led_behaviour(Wifi, On);
        if (esp_wifi_connect() != ESP_OK)
        {
            set_led_behaviour(LTE4G, Off);
            set_led_behaviour(Wifi, Off);
            // if device cannot connnect to network enter in provisioning mode
            app_provisoning();
        }
        else
        {
            wifi_active = true;
            LTE4G_active = false;
        }
    }
    else if (ppp_netif != NULL)
    {
        assert(ppp_netif != NULL);
        set_led_behaviour(LTE4G, SlowBlinking);
        wifi_active = false;
        LTE4G_active = true;
        // ESP_LOGW(TAG, "Wfi is : %d", wifi_active);
        set_led_behaviour(Wifi, Off);
    }

    // Set actual time from the network
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();

    set_led_behaviour(Power, On);
    vTaskDelay(pdMS_TO_TICKS(1500));
    mqtt_client = mqtt_start();
    vTaskDelay(pdMS_TO_TICKS(1500));

    time_t current_time;

    time_t previous_force_time = -181;
    time_t previous_sent_time = -301;
    time_t all_data_previous_sent_time = -601;

    if (modbus_test(CID_SALT_ELECTROLISE, HP_INV_02_BAUD_RATE, UART_PARITY_DISABLE))
    {
        salt_is_active = true;
        ESP_LOGW(TAG, "SALT device connected on modbus network");
        set_led_behaviour(Modbus, StopFastBlinking);
        set_led_behaviour(Modbus, On);
    }
    if (modbus_test(CID_HP_RAK_WATER_IN_TEMPERATURE, HP_RAK_01_BAUD_RATE, UART_PARITY_EVEN))
    {
        rak_is_active = true;
        ESP_LOGW(TAG, "RAK device connected on modbus network");
        set_led_behaviour(Modbus, StopFastBlinking);
        set_led_behaviour(Modbus, On);
    }
    if (modbus_test(CID_INV02_WATER_IN_TEMPERATURE, HP_INV_02_BAUD_RATE, UART_PARITY_DISABLE))
    {
        inv_is_active = true;
        ESP_LOGW(TAG, "INVERTER device connected on modbus network");
        set_led_behaviour(Modbus, StopFastBlinking);
        set_led_behaviour(Modbus, On);
    }
    if ((salt_is_active == 0) && (inv_is_active == 0) && (rak_is_active == 0))
    {
        set_led_behaviour(Modbus, FastBlinking);
        ESP_LOGW(TAG, "No device connected on modbus network");
         if ((salt_is_active == 0) && (inv_is_active == 0) && (rak_is_active == 0))
            {   
                modbus_error = true;
                
                if (modbus_error)
                {
                    if (!had_error_previous_time)
                    {
                        mqtt_modbus_error_send(mqtt_client);
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
            }
    }

    modbus_commands_queue_init();

    while (1)
    {
        bool sendAllData = false;

        apply_out_topic_messages();

        // Extract Errors from active Modbus devices
        if (rak_is_active)
            heatpump_rak_operate(KEY_ID_HP_RAK_ERRORS, mqtt_client, false, workRak01);

        if (inv_is_active)
            inverter_02_operate(KEY_ID_HP_INV02_ERRORS, mqtt_client, false, workInv02);

        if (salt_is_active)
            salt_operate(KEY_ID_SALT_WORK_DATA, mqtt_client, false, workInv02);

        time(&current_time);

        modbus_execute_pending_commands(mqtt_client);
        // Execute periodic or forced events
        if (board_btn_info.was_btn_clicked_flag && (current_time - previous_force_time) > BTN_FORCE_RESEND_DELAY_SECONDS)
        {
            sendAllData = true;

            time(&previous_force_time);
            previous_sent_time = previous_force_time;
        }
        else
        {
            if (current_time - all_data_previous_sent_time > SEND_DATA_ALL_INTERVAL_SECONDS)
            {
                sendAllData = true;

                time(&all_data_previous_sent_time);
                previous_sent_time = all_data_previous_sent_time;
            }
            else if (current_time - previous_sent_time > SEND_DATA_DIFF_INTERVAL_SECONDS)
            {
                ESP_LOGI(TAG, "Monitoring data");

                if (rak_is_active)
                {
                    heatpump_rak_operate(KEY_ID_HP_RAK_WORK_DATA, mqtt_client, false, workRak01);
                    heatpump_rak_operate(KEY_ID_HP_RAK_MANUFACTURER_DATA, mqtt_client, false, manuRak01);
                    heatpump_rak_operate(KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA, mqtt_client, false, manuRak01);
                    heatpump_rak_operate(KEY_ID_HP_RAK_COMPRESSOR_MNUFACTURER_DATA, mqtt_client, false, manuRak01);
                }
                if (inv_is_active)
                {      mqtt_live_status_send(mqtt_client);
                    // inverter_02_operate(KEY_ID_HP_INV02_WORK_DATA, mqtt_client, false, workInv02);
                    // inverter_02_operate(KEY_ID_HP_INV02_MANUFACTURER_DATA, mqtt_client, false, manuInv02);
                    // inverter_02_operate(KEY_ID_HP_INV02_MANUAL_MANUFACTURER_DATA, mqtt_client, false, manuInv02);
                }
                if (salt_is_active)
                    salt_operate(KEY_ID_SALT_WORK_DATA, mqtt_client, false, workInv02);

                time(&previous_sent_time);
            }
        }
        if ((salt_is_active == 0) && (inv_is_active == 0) && (rak_is_active == 0))
        {
            ESP_LOGW(TAG, "No device connected on modbus network please check connection");
            if (modbus_test(CID_SALT_ELECTROLISE, HP_INV_02_BAUD_RATE, UART_PARITY_DISABLE))
            {
                salt_is_active = true;
                modbus_error = false;
                ESP_LOGW(TAG, "SALT device connected on modbus network");
                set_led_behaviour(Modbus, StopFastBlinking);
                set_led_behaviour(Modbus, On);
            }
            if (modbus_test(CID_HP_RAK_WATER_IN_TEMPERATURE, HP_RAK_01_BAUD_RATE, UART_PARITY_EVEN))
            {
                rak_is_active = true;
                modbus_error = false;
                ESP_LOGW(TAG, "RAK device connected on modbus network");
                set_led_behaviour(Modbus, StopFastBlinking);
                set_led_behaviour(Modbus, On);
            }
            if (modbus_test(CID_INV02_WATER_IN_TEMPERATURE, HP_INV_02_BAUD_RATE, UART_PARITY_DISABLE))
            {
                inv_is_active = true;
                modbus_error = false;
                ESP_LOGW(TAG, "INVERTER device connected on modbus network");
                set_led_behaviour(Modbus, StopFastBlinking);
                set_led_behaviour(Modbus, On);
            }
            if ((salt_is_active == 0) && (inv_is_active == 0) && (rak_is_active == 0))
            {   
                modbus_error = true;
                
                if (modbus_error)
                {
                    if (!had_error_previous_time)
                    {
                        mqtt_modbus_error_send(mqtt_client);
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
            }
        }
        if (sendAllData)
        {
            ESP_LOGI(TAG, "Forced request all data");

            if (rak_is_active)
            {
                heatpump_rak_operate(KEY_ID_HP_RAK_WORK_DATA, mqtt_client, true, workRak01);
                heatpump_rak_operate(KEY_ID_HP_RAK_ERRORS, mqtt_client, true, workRak01);
                heatpump_rak_operate(KEY_ID_HP_RAK_MANUFACTURER_DATA, mqtt_client, true, manuRak01);
                heatpump_rak_operate(KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA, mqtt_client, true, manuRak01);
                heatpump_rak_operate(KEY_ID_HP_RAK_COMPRESSOR_MNUFACTURER_DATA, mqtt_client, true, manuRak01);
                heatpump_rak_operate(KEY_ID_HP_RAK_GENERAL_CONFIGURATION, mqtt_client, true, configRak01); // ok
                heatpump_rak_operate(KEY_ID_HP_RAK_COMPRESSOR_CONFIGURATION_DATA_0, mqtt_client, true, configRak01);
                heatpump_rak_operate(KEY_ID_HP_RAK_DEFROSTING_CONFIGURATION_DATA, mqtt_client, true, configRak01);
                heatpump_rak_operate(KEY_ID_HP_RAK_TEMPERATURE_CONFIGURATION_DATA, mqtt_client, true, configRak01);
            }

            if (inv_is_active)
            {
                inverter_02_operate(KEY_ID_HP_INV02_WORK_DATA, mqtt_client, true, workInv02);
                inverter_02_operate(KEY_ID_HP_INV02_ERRORS, mqtt_client, true, workInv02);
                inverter_02_operate(KEY_ID_HP_INV02_MANUFACTURER_DATA, mqtt_client, true, manuInv02);
                inverter_02_operate(KEY_ID_HP_INV02_MANUAL_MANUFACTURER_DATA, mqtt_client, true, manuInv02);
                inverter_02_operate(KEY_ID_HP_INV02_CONFIGURATION_FAN, mqtt_client, true, configInv02);
                inverter_02_operate(KEY_ID_HP_INV02_COMPRESSOR_CONFIGURATION_DATA_0, mqtt_client, true, configInv02);
                inverter_02_operate(KEY_ID_HP_INV02_COMPRESSOR_CONFIGURATION_DATA_1, mqtt_client, true, configInv02);
                inverter_02_operate(KEY_ID_HP_INV02_GENERAL_CONFIGURATION, mqtt_client, true, configInv02);
                inverter_02_operate(KEY_ID_HP_INV02_DEFROSTING_CONFIGURATION_DATA, mqtt_client, true, configInv02);
                inverter_02_operate(KEY_ID_HP_INV02_TEMPERATURE_CONFIGURATION_DATA, mqtt_client, true, configInv02);
            }

            if (salt_is_active)
            {
                salt_operate(KEY_ID_SALT_WORK_DATA, mqtt_client, true, workInv02);
                salt_operate(KEY_ID_SALT_CONFIGURATION, mqtt_client, true, configInv02);
            }
        }

        if (board_btn_info.is_pressed && board_btn_info.press_time_us && current_time - board_btn_info.press_time_us / 1000000 > 5 && LTE4G_active == false)
        {
            // Long press
            board_btn_info.was_btn_hold = true;
            ESP_LOGW(TAG, "Device enter in BLE provisioning mode!!!!!!!!!!!");
            if (board_btn_info.was_btn_hold)
            {

                app_provisoning();
            }
            // Reset the press time so it does not enter again here
            board_btn_info.press_time_us = 0;
        }
        if (!mqtt_is_connected())
        {
            ESP_LOGW(TAG, "MQTT DICCONNECTED !!!!!!!!!!!");
            if (ppp_netif == NULL)
            {
                esp_wifi_connect();
                vTaskDelay(pdMS_TO_TICKS(5000));
            }
        }

        board_btn_info.was_btn_clicked_flag = false;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

#ifdef CONFIG_DUMP_SYSTEM_STATUS
    _system_dump();
#endif
    vTaskDelay(pdMS_TO_TICKS(1000));
}

#endif