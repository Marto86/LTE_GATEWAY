#include "./gateway_credentials.h"
#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_err.h"
#include "stddef.h"
#include "esp_log.h"

char res[10];
// size_t *length = 0;
char *out_value = "sdfs";

char *gateway_credentials_get_string(CredentialSetType type)
{
    nvs_handle_t my_handle;
    ESP_LOGI("GC", "before");
    esp_err_t err = nvs_open_from_partition("provisioning", "provisioning", NVS_READONLY, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGI("GWC", "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    ESP_LOGI("GC", "after");
    
    int32_t uuid;
    size_t len;
    esp_err_t err_read;

    switch (type)
    {
    case UUID:
        err_read = nvs_get_i32(my_handle, "uuid", &uuid);
        if (err_read != ESP_OK) {
            ESP_LOGI("GWC", "Error (%s) UUID", esp_err_to_name(err_read));
        }
        ESP_LOGI("GC", "uuid %d", uuid);
        itoa(uuid, res, 10);
        return res;
        break;
    case SERIAL_NUMBER:
        err_read = nvs_get_str(my_handle, "serial", NULL, &len);
        if (err_read == ESP_OK)
        {
            char *str = (char *)malloc(len);
            if ((err = nvs_get_str(my_handle, "serial", str, &len)) == ESP_OK)
            {
                ESP_LOGI("GWC", "serial %s", str);
            }

            return str;
        }
        else
        {
            ESP_LOGI("GWC", "Error (%s) serial", esp_err_to_name(err_read));
        }
        break;
    case MQTT_CLIENT_ID:
        err_read = nvs_get_str(my_handle, "mqtt_cl_id", NULL, &len);
        if (err_read == ESP_OK)
        {
            char *str = (char *)malloc(len);
            if ((err = nvs_get_str(my_handle, "mqtt_cl_id", str, &len)) == ESP_OK)
            {
                ESP_LOGI("GC", "mqtt_cl_id %s", str);
            }

            return str;
        }
        else
        {
            ESP_LOGI("GWC", "Error (%s) mqtt_cl_id", esp_err_to_name(err_read));
        }
        break;
    case MQTT_CLIENT_USER:
        err_read = nvs_get_str(my_handle, "mqtt_cl_user", NULL, &len);
        if (err_read == ESP_OK)
        {
            char *str = (char *)malloc(len);
            if ((err = nvs_get_str(my_handle, "mqtt_cl_user", str, &len)) == ESP_OK)
            {
                ESP_LOGI("GC", "mqtt_cl_user %s", str);
            }

            return str;
        }
        else
        {
            ESP_LOGI("GWC", "Error (%s) mqtt_cl_user", esp_err_to_name(err_read));
        }
        break;
    case MQTT_CLIENT_PASSWORD:
        err_read = nvs_get_str(my_handle, "mqtt_pass", NULL, &len);
        if (err_read == ESP_OK)
        {
            char *str = (char *)malloc(len);
            if ((err = nvs_get_str(my_handle, "mqtt_pass", str, &len)) == ESP_OK)
            {
                ESP_LOGI("GC", "mqtt_pass %s", str);
            }

            return str;
        }
        else
        {
            ESP_LOGI("GWC", "Error (%s) mqtt_pass", esp_err_to_name(err_read));
        }
        break;
    case SECTOR:
        err_read = nvs_get_str(my_handle, "sector", NULL, &len);
        if (err_read == ESP_OK)
        {
            char *str = (char *)malloc(len);
            if ((err = nvs_get_str(my_handle, "sector", str, &len)) == ESP_OK)
            {
                ESP_LOGI("GC", "sector %s", str);
            }
            // free(str);
            return str;
        }
        else
        {
            ESP_LOGI("GWC", "Error (%s) sector", esp_err_to_name(err_read));
        }
        break;
    default:
        return "";
        break;
    }

    return "";
}

int gateway_credentials_set_string(CredentialSetType type, char *value)
{
    // TODO

    nvs_handle_t my_handle;
    ESP_LOGI("GC", "before");
    esp_err_t err = nvs_open_from_partition("configs", "configs", NVS_READWRITE, &my_handle);
    return 0;
}