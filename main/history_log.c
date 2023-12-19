#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_err.h"
#include "stddef.h"
#include "esp_log.h"
#include "esp_system.h"
#include "appconfig.h"
#include "nvs.h"
#include "poly_routing.h"
#include "mqtt_client.h"
#include "mqtt_api.h"
#include "history_log.h"
#include "modbus.h"

#define HISTORY_LOG_NAMESPACE_MFG_DATA   "MfgData"

// Maximum 2 weeks of MfgData, logged every 6 hours
#define HISTORY_LOG_MFG_ENTRIES_MAX_NR  2016

// RAM Log buffer
#define OUTPUT_LOG_BUFFER_SIZE      6*1024

#define OUTPUT_LOG_STRING_BYTES_MAX     256

static const char *TAG = "HL";

static nvs_handle_t HistoryLog_MfgData;

// The first 4bytes are reserved for reporting the outputLogBufPtr offset
__NOINIT_ATTR static char outputLogBuf[OUTPUT_LOG_BUFFER_SIZE];
__NOINIT_ATTR static char* outputLogBufPtr;


// This function will be called by the ESP log library every time ESP_LOG needs to be performed.
//      @important Do NOT use the ESP_LOG* macro's in this function ELSE recursive loop and stack overflow! So use printf() instead for debug messages.
static int _output_log_vprintf(const char *fmt, va_list args) 
{
    int bytesWritten;

    if (outputLogBufPtr+OUTPUT_LOG_STRING_BYTES_MAX < &outputLogBuf[OUTPUT_LOG_BUFFER_SIZE])
    {
        // Mark the end of the buffer
        *outputLogBufPtr=0xFF;
        // Start overwriting buffer from the beginning
        outputLogBufPtr=outputLogBuf+4;
    }

    // Buffer the message in RAM
    bytesWritten = vsprintf(outputLogBufPtr, fmt, args);
    if (bytesWritten >= 0) {
        outputLogBufPtr+=bytesWritten;

        // Just for any case, check if we're still writing in the write buffer
        if (outputLogBufPtr+OUTPUT_LOG_STRING_BYTES_MAX > &outputLogBuf[OUTPUT_LOG_BUFFER_SIZE])
        {
            // Reboot, as we have overwritten data
            esp_restart();
        }
    }


    // ALWAYS Write to stdout!
    return vprintf(fmt, args);
}

int history_log_init(void)
{

    if (esp_reset_reason() == ESP_RST_POWERON)
    {
        // Init the no-init variables
        outputLogBufPtr=outputLogBuf+4;
    }

    history_log_output_enable();
    esp_err_t err = nvs_flash_init_partition(NVS_INGELI_HISTORY_PART_NAME);

    if (err != ESP_OK) {
        ESP_LOGI(TAG, "Error initializing partition with name %s, error: %s HistoryLog NVS!\n", NVS_INGELI_HISTORY_PART_NAME, esp_err_to_name(err));
    }

    err = nvs_open_from_partition(NVS_INGELI_HISTORY_PART_NAME, HISTORY_LOG_NAMESPACE_MFG_DATA, NVS_READWRITE, &HistoryLog_MfgData);

    if (err != ESP_OK) {
        ESP_LOGI(TAG, "Error (%s) opening HistoryLog NVS!\n", esp_err_to_name(err));
    }

    return ESP_OK;
}

int history_log_output_enable(void) {
    ESP_LOGI(TAG, "Redirecting the log output to memory (also keep sending logs to UART0)");
    esp_log_set_vprintf(&_output_log_vprintf);
    return 0;
}

int history_log_output_disable(void) {
    ESP_LOGI(TAG, "Disable log output redirection");
    esp_log_set_vprintf(&vprintf);
    return ESP_OK;
}


int history_log_save(uint32_t timestamp, void* data, size_t size)
{
   char keyName[NVS_KEY_NAME_MAX_SIZE];
    nvs_stats_t nvs_stats;
    esp_err_t err;
    static uint32_t oldTimestamp=0;
    static int duplicatedTimestamps=0;

    if (timestamp == oldTimestamp)
    {
        duplicatedTimestamps++;
    }
    else
    {
        oldTimestamp = timestamp;
        duplicatedTimestamps=0;
    }

    sprintf(keyName,"%d%d",timestamp,duplicatedTimestamps);

    err = nvs_get_stats(NVS_INGELI_HISTORY_PART_NAME,&nvs_stats);

    if (nvs_stats.used_entries > HISTORY_LOG_MFG_ENTRIES_MAX_NR)
    {
        // Free the 2 oldest entries
        nvs_entry_info_t info;
        nvs_iterator_t it = nvs_entry_find(NVS_INGELI_HISTORY_PART_NAME, HISTORY_LOG_NAMESPACE_MFG_DATA, NVS_TYPE_ANY);

        ESP_LOGI(TAG, "Maximum NVS quota reached. Used:%d, Available:%d",nvs_stats.used_entries,nvs_stats.free_entries);

        nvs_entry_info(it, &info);
        // Protect from overflow of NVS memory
        nvs_erase_key(HistoryLog_MfgData, info.key);
        ESP_LOGI(TAG, "Del key %s",info.key);
        it = nvs_entry_next(it);
        nvs_erase_key(HistoryLog_MfgData, info.key);
        ESP_LOGI(TAG, "Del key %s",info.key);

        nvs_release_iterator(it);

        // Update the NVS
        nvs_commit(HistoryLog_MfgData);
    }

    err = nvs_set_blob(HistoryLog_MfgData, keyName, data, size);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error %d. Cannot add to NVS key %s, size %d",err, keyName, size);
        return err;
    }

    err = nvs_commit(HistoryLog_MfgData);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error %d. Cannot commit to NVS key %s, size %d",err, keyName, size);
        return err;
    }

    ESP_LOGI(TAG, "Saved key %s=%08X[%d]",keyName,*(uint32_t*)data,size);

    return err;
}

int history_log_report_saved(void *mqtt_client)
{
    nvs_iterator_t it;
    esp_err_t err;
    
    it = nvs_entry_find(NVS_INGELI_HISTORY_PART_NAME, HISTORY_LOG_NAMESPACE_MFG_DATA, NVS_TYPE_ANY);

    while (it != NULL)
    {
        uint8_t mqttData[PUBLISH_MQTT_DATA_SIZE_MAX];
        size_t dataSize;
        nvs_entry_info_t info;
        vTaskDelay(pdMS_TO_TICKS(300));
        nvs_entry_info(it, &info);
        dataSize=PUBLISH_MQTT_DATA_SIZE_MAX;
        err = nvs_get_blob(HistoryLog_MfgData, info.key, mqttData, &dataSize);       
        // ESP_LOG_BUFFER_HEXDUMP(TAG, mqttData, &dataSize, ESP_LOG_INFO);
        if (err != ESP_OK)
            ESP_LOGE(TAG, "Error %d. Cannot read from NVS key=%s",err,info.key);
        if (inv_is_active)
        {
            int msg_id = esp_mqtt_client_publish((esp_mqtt_client_handle_t)mqtt_client, gw_to_cloud_topics[manuInv02],
                                            (const char *)mqttData, dataSize, 0, 0);
            

        ESP_LOGI(TAG, "PUB[%d] %s:%08X,%d", dataSize, gw_to_cloud_topics[manuInv02],*(uint32_t*)mqttData,msg_id);
           // If the publish was successful, then remove the key from NVS
        if (msg_id >= 0)
            nvs_erase_key(HistoryLog_MfgData, info.key);

        it = nvs_entry_next(it);
        }
          if (rak_is_active)
        {
            int msg_id = esp_mqtt_client_publish((esp_mqtt_client_handle_t)mqtt_client, gw_to_cloud_topics[manuRak01],
                                            (const char *)mqttData, dataSize, 0, 0);
            vTaskDelay(pdMS_TO_TICKS(500));

        ESP_LOGI(TAG, "PUB[%d] %s:%08X,%d", dataSize, gw_to_cloud_topics[manuInv02],*(uint32_t*)mqttData,msg_id);
           // If the publish was successful, then remove the key from NVS
        if (msg_id >= 0)
            nvs_erase_key(HistoryLog_MfgData, info.key);

        it = nvs_entry_next(it);
        }

        // If the publish was successful, then remove the key from NVS
    }

    nvs_release_iterator(it);

    err = nvs_commit(HistoryLog_MfgData);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error %d. Cannot update NVS with the deleted keys",err);
    }

    return err;
}


int history_log_report_output_logs(void *mqtt_client)
{
    uint32_t* logBufOffset = (uint32_t*)outputLogBuf;

    *logBufOffset=outputLogBufPtr-outputLogBuf;

    int msg_id = esp_mqtt_client_publish((esp_mqtt_client_handle_t)mqtt_client, gw_to_cloud_topics[Logs],
                                        (const char *)outputLogBuf, OUTPUT_LOG_BUFFER_SIZE, 0, 0);

    ESP_LOGI(TAG, "PUB[%d] %s:%08X,%d", OUTPUT_LOG_BUFFER_SIZE, gw_to_cloud_topics[Logs],*(uint32_t*)outputLogBuf,msg_id);

    return msg_id;
}