#ifndef __INGELI_NVS_H__
#define __INGELI_NVS_H__

#include "esp_err.h"

esp_err_t ingeli_nvs_flash_init(char* nvs_name);

esp_err_t ingeli_nvs_flash_erase(char* nvs_name);

void ingeli_initialize_nvs();

#endif // __INGELI_NVS_H__