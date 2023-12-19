#include "ingeli_nvs.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "appconfig.h"

esp_err_t ingeli_nvs_flash_init(char *nvs_name)
{
    return nvs_flash_init_partition(nvs_name);
}

esp_err_t ingeli_nvs_flash_erase(char *nvs_name)
{
    return nvs_flash_erase_partition(nvs_name);
}

void ingeli_initialize_nvs()
{
    /* Initialize NVS for Wi-Fi storage - default NVS partition*/
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        /* NVS partition was truncated and needs to be erased
         * Retry nvs_flash_init */
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    /* Initialize the NVS partition that is supposed to be used by Ingeli for provisioning */
    ingeli_nvs_flash_init(NVS_INGELI_PROVISIONING_PART_NAME);

    /* Initialize the NVS partition that is supposed to be used for client configurations */
    ingeli_nvs_flash_init(NVS_INGELI_CONFIGS_PART_NAME);
}