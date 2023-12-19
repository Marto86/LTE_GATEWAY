#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_tls.h"
#include "esp_ota_ops.h"
#include <sys/param.h>
#include "heatpump_rak.h"

#include "./appconfig.h"
#include "./gateway_credentials.h"
#include "./polytropic/poly_routing.h"
#include "./polytropic/poly_types.h"
#include "./ota.h"
#include "history_log.h"
#include "mqtt_api.h"
#include "./poly_routing.h"
#include "modbus.h"

extern bool rak_is_active;
extern bool inv_is_active;
extern bool salt_is_active;

extern bool dignostic_mode = false;

static bool is_connected = false;

static const char *TAG = "MQTT";

#if CONFIG_BROKER_CERTIFICATE_OVERRIDDEN == 1
static const uint8_t mqtt_eclipseprojects_io_pem_start[] = "-----BEGIN CERTIFICATE-----\n" CONFIG_BROKER_CERTIFICATE_OVERRIDE "\n-----END CERTIFICATE-----";
#else
extern const uint8_t mqtt_eclipseprojects_io_pem_start[] asm("_binary_ingeliProdCA01_crt_start");
#endif
extern const uint8_t mqtt_eclipseprojects_io_pem_end[] asm("_binary_ingeliProdCA01_crt_end");

// extern const uint8_t client_cert_pem_start[] asm("_binary_ingeliDevCA01_crt_start");
// extern const uint8_t client_cert_pem_end[] asm("_binary_ingeliDevCA01_crt_end");
//
//  Note: this function is for testing purposes only publishing part of the active partition
//        (to be checked against the original binary)
//
// static void send_binary(esp_mqtt_client_handle_t client)
// {
//     spi_flash_mmap_handle_t out_handle;
//     const void *binary_address;
//     const esp_partition_t *partition = esp_ota_get_running_partition();
//     esp_partition_mmap(partition, 0, partition->size, SPI_FLASH_MMAP_DATA, &binary_address, &out_handle);
//     // sending only the configured portion of the partition (if it's less than the partition size)
//     int binary_size = MIN(CONFIG_BROKER_BIN_SIZE_TO_SEND, partition->size);
// int msg_id = esp_mqtt_client_publish(client, "/topic/binary", binary_address, binary_size, 0, 0);
//     ESP_LOGI(TAG, "binary sent with msg_id=%d", msg_id);
// }

static void handle_cloud_to_gw_request(esp_mqtt_client_handle_t client, ContainerTypeEnum topicType, char *topic, int topic_len, char *data, int data_len)
{
    parse_mqtt_request_status status = FAILED;

    switch (topicType)
    {
    case workInv02:
    case manuInv02:
    case configInv02:
        status = mqtt_out_device_topic_data_parse(CID_HP_INV02_OFFSET, data, data_len);
        break;

    case workRak01:
    case manuRak01:
    case configRak01:
        status = mqtt_out_device_topic_data_parse(MODBUS_CID_HP_RAK_OFFSET, data, data_len);
        break;

    case Raw:
        break;

    case ManufacturerHisto:
        break;

    case Diagnostic:
        break;

    case Info:
        break;

    default:
        break;
    }

    if (status != SUCCESS)
        ESP_LOGE(TAG, "Error parsing update request!");
}

static void handle_command_request(esp_mqtt_client_handle_t client, CommandTypeEnum topicType, char *topic, int topic_len, char *data, int data_len)
{
    ESP_LOGI(TAG, "handle_command_request topic: %s, data: %s", topic, data);

    int cmd_topic_type_int = get_cmd_topic_type(topic);
    if (cmd_topic_type_int >= 0)
    {
        CommandTypeEnum cmd_topic_type = (CommandTypeEnum)cmd_topic_type_int;
        if (strcmp(topic, command_topics[CmdOtaLink]) == 0)
        {
            ESP_LOGI(TAG, "Start OTA called!");
            esp_mqtt_client_publish(client, command_response_topics[cmd_topic_type], COMMAND_RECEIVED_MSG, 0, 0, 0);
            set_ota_url(data);
            start_ota(topic, topic_len);
        }
        else if (strcmp(topic, command_topics[CmdInfo]) == 0)
        {
            if (strcmp(data, "get_firmware") == 0)
            {
                ESP_LOGI(TAG, "Get firmware version called!");
                esp_mqtt_client_publish(client, command_response_topics[cmd_topic_type], FIRMWARE_VERSION, 0, 0, 0);
            }
        }
        else if (strcmp(topic, command_topics[CmdHistory]) == 0)
        {
            if (strcmp(data, "manufacturer") == 0)
            {
                ESP_LOGI(TAG, "Get manufacturer history log from NVS!");
                esp_mqtt_client_publish(client, command_response_topics[cmd_topic_type], COMMAND_RECEIVED_MSG, 0, 0, 0);
                history_log_report_saved(client);
            }
            else if (strcmp(data, "logs") == 0)
            {
                ESP_LOGI(TAG, "Get output history logs!");
                esp_mqtt_client_publish(client, command_response_topics[cmd_topic_type], COMMAND_RECEIVED_MSG, 0, 0, 0);
                history_log_report_output_logs(client);
            }
        }
        else if (strcmp(topic, command_topics[CmdGetData]) == 0)
        {
            esp_mqtt_client_publish(client, command_response_topics[cmd_topic_type], COMMAND_RECEIVED_MSG, 0, 0, 0);

            if (strcmp(data, "working") == 0)
            {
                modbus_commands_queue_push(WorkingGetDataCommand);
            }
            else if (strcmp(data, "manufacturer") == 0)
            {
                modbus_commands_queue_push(ManufacturingGetDataCommand);
            }
            else if (strcmp(data, "configuration") == 0)
            {
                modbus_commands_queue_push(ConfigurationGetDataCommand);
            }
        }
        else if (strcmp(topic, command_topics[CmdDiagnostic]) == 0)
        {
            esp_mqtt_client_publish(client, command_response_topics[cmd_topic_type], COMMAND_RECEIVED_MSG, 0, 0, 0);

            if ((strcmp(data, "enable") == 0))
            {
                dignostic_mode = true;
            }
            else if (strcmp(data, "disable") == 0)
            {
                dignostic_mode = false;
            }
        }
    }
}

static void handle_mqtt_event_data(esp_mqtt_client_handle_t client, const char *topic, int topic_len, const char *data, int data_len)
{
    char topic_nt[topic_len + 1];
    strncpy(topic_nt, topic, topic_len);
    topic_nt[topic_len] = '\0';
    uint8_t topic_type = 0xFF;

    switch (poly_get_topic_type(topic_nt, &topic_type))
    {
    case CTGW:
        handle_cloud_to_gw_request(client, topic_type, topic_nt, topic_len, data, data_len);
        break;
    case CMD:
    {
        char data_nt[data_len + 1];
        strncpy(data_nt, data, data_len);
        data_nt[data_len] = '\0';
        handle_command_request(client, topic_type, topic_nt, topic_len, data_nt, data_len);
        break;
    }
    default:
        break;
    }
}

// static void send_gw_to_cloud_message(char *topic, int topic_len, char *data, int data_len)
// {
//     // esp_mqtt_client_publish(client, topic, data, data_len, 2, 0);
// }

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        for (size_t i = 0; cloud_to_gw_topics[i]; i++)
        {
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, cloud_to_gw_topics[i], 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        }

        for (size_t i = 0; command_topics[i]; i++)
        {
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, command_topics[i], 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        }

        is_connected = true;
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        is_connected = false;
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        // msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 2, 0);
        // ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        // if (strncmp(event->data, "send binary please", event->data_len) == 0)
        // {
        //     ESP_LOGI(TAG, "Sending the binary");
        //     send_binary(client);
        // }
        // for (size_t i = 0; i < event->data_len; i++)
        // {
        //     ESP_LOGI(TAG, "DI: %x, index: %d", (uint8_t)event->data[i], i);
        // }
        handle_mqtt_event_data(event->client, event->topic, event->topic_len, event->data, event->data_len);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
        {
            ESP_LOGI(TAG, "Last error code reported from esp-tls: 0x%x", event->error_handle->esp_tls_last_esp_err);
            ESP_LOGI(TAG, "Last tls stack error number: 0x%x", event->error_handle->esp_tls_stack_err);
            ESP_LOGI(TAG, "Last captured errno : %d (%s)", event->error_handle->esp_transport_sock_errno,
                     strerror(event->error_handle->esp_transport_sock_errno));
        }
        else if (event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED)
        {
            ESP_LOGI(TAG, "Connection refused error: 0x%x", event->error_handle->connect_return_code);
        }
        else
        {
            ESP_LOGW(TAG, "Unknown error type: 0x%x", event->error_handle->error_type);
        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

// Returns pointer to MQTT client
void *mqtt_start(void)
{
    ESP_LOGI(TAG, "cl:%s,us:%s,ps:%s", gateway_credentials_get_string(MQTT_CLIENT_ID), gateway_credentials_get_string(MQTT_CLIENT_USER), gateway_credentials_get_string(MQTT_CLIENT_PASSWORD));
    const esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_BROKER_URI,
        //.cert_pem = (const char *)mqtt_eclipseprojects_io_pem_start,
        //.client_cert_pem = (const char *)client_cert_pem_start,
        .cert_pem = (const char *)mqtt_eclipseprojects_io_pem_start,
        .client_id = gateway_credentials_get_string(MQTT_CLIENT_ID),      // GATEWAY_MQTT_CLIENT_ID, // const char *client_id;                  /*!< default client id is ``ESP32_%CHIPID%`` where %CHIPID% are last 3 bytes of MAC address in hex format */
        .username = gateway_credentials_get_string(MQTT_CLIENT_USER),     // GATEWAY_MQTT_CLIENT_ID,  // const char *username;                   /*!< MQTT username */
        .password = gateway_credentials_get_string(MQTT_CLIENT_PASSWORD), // GATEWAY_MQTT_PASSWORD,   // const char *password;
        .skip_cert_common_name_check = true,
        .network_timeout_ms = 30000,
        .keepalive = 400};

    // const esp_mqtt_client_config_t mqtt_cfg = {
    //     .uri = "mqtts://repo.pts.rnd.bg:8883",
    //     .cert_pem = (const char *)mqtt_pravets_pem_start,
    //     //.client_id = GATEWAY_MQTT_CLIENT_ID,
    //     .username = "ingeli",
    //     .password = "Test4321",
    //     .skip_cert_common_name_check = true,
    //     .network_timeout_ms = 30000};

    // ESP_LOGI(TAG, "GU len=%d: %.*s", 20, 20, client_cert_pem_start);

    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);

    return client;
}
bool mqtt_is_connected()
{
    return is_connected;
}
