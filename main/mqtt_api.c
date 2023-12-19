#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "mqtt_client.h"
#include "esp_log.h"
#include "time.h"
#include "mqtt_api.h"
#include "./polytropic/poly_routing.h"
#include "./board_devices/board_leds.h"
#include "./gateway_credentials.h"
#include "poly_routing.h"
#include "heatpump_rak.h"
#include "inverter_02.h"
#include "modbus.h"
#include "modem_board.h"
#include "history_log.h"
#include "freertos/queue.h"
#include "modbus.h"

#define PUBLISH_MQTT_PACKET_SIZE_MAX (PUBLISH_MQTT_DATA_SIZE_MAX + sizeof(mqtt_payload_header_t))

static const char *TAG = "MQTT_API";

typedef struct
{
  uint32_t timestamp_now;
  uint32_t timestamp_old;
  uint32_t uuid;
} mqtt_payload_header_t;

typedef struct
{
  mqtt_payload_header_t header;
  uint8_t data[PUBLISH_MQTT_DATA_SIZE_MAX];
} mqtt_payload_data_t;

typedef struct
{
  uint8_t *key;

  // Pointer where the bitmask value must be copied
  // NOTE: The bitmask pointer cannot be uint16_t*,
  // because direct modification of 16-bit work will lead to alignment hardfault error
  // The bitmaskVal should be copied with memcpy at the end of the bitmask check creation
  uint8_t *bitmaskPtr;
  uint8_t *dataPtr;
  uint16_t bitmaskVal;
  uint8_t packetSize; // Includes also the timestamps and uuid
} mqtt_payload_info_t;

mqtt_payload_data_t mqtt_payload_data;
mqtt_payload_info_t mqtt_payload_info;

int mqtt_data_add(uint16_t key_id, uint16_t keyBitIx, uint16_t value, uint16_t valueBitMask)
{
  if (!mqtt_payload_info.key || *mqtt_payload_info.key != key_id)
  {

    if (!mqtt_payload_info.key)
    {
      // Reset the mqtt payload info structure
      mqtt_payload_info.key = mqtt_payload_data.data;

      // The initial packet size is header + key id + bitmask value
      mqtt_payload_info.packetSize = sizeof(mqtt_payload_header_t) + sizeof(uint8_t) + sizeof(mqtt_payload_info.bitmaskVal);
    }
    else
    {
      // Copy/Save the current bitmask to the packet
      memcpy(mqtt_payload_info.bitmaskPtr, &mqtt_payload_info.bitmaskVal, sizeof(mqtt_payload_info.bitmaskVal));

      mqtt_payload_info.key = mqtt_payload_info.dataPtr;

      mqtt_payload_info.packetSize += sizeof(uint8_t) + sizeof(mqtt_payload_info.bitmaskVal);
    }

    mqtt_payload_info.bitmaskPtr = mqtt_payload_info.key + 1;
    *mqtt_payload_info.key = key_id;

    // Reserve space for the new bitmask value
    mqtt_payload_info.dataPtr = mqtt_payload_info.bitmaskPtr + sizeof(mqtt_payload_info.bitmaskVal);

    mqtt_payload_info.bitmaskVal = 0;
  }

  mqtt_payload_info.bitmaskVal |= (1 << keyBitIx);

  value &= valueBitMask;

  // Shift the data until the data is right aligned with the mask
  while ((valueBitMask & 0x0001) == 0)
  {
      valueBitMask>>=1;
      value>>=1;
  }

  // Copy data to the packet payload
  // IMPORTANT: It cannot be assigned directly (i.e. *(uint16_t*)mqtt_payload_info.dataPtr=value)
  // Because it will cause memory alignemnt fault
  memcpy(mqtt_payload_info.dataPtr, &value, sizeof(value));
  mqtt_payload_info.dataPtr += sizeof(value);
  mqtt_payload_info.packetSize += sizeof(value);

  if (mqtt_payload_info.packetSize >= PUBLISH_MQTT_PACKET_SIZE_MAX)
    return -1;
  else
    return 0;
}

int mqtt_data_send(void *client, ContainerTypeEnum route_container)
{
  // The 4 LSB bytes from the MAC will be used as UUID
  // uint8_t mac[6];
  time_t current_time;
  int msg_id;

  // esp_read_mac(mac,ESP_MAC_WIFI_STA);
  
  // Save the current bitmask (if any)
  if (mqtt_payload_info.bitmaskPtr)
    memcpy(mqtt_payload_info.bitmaskPtr, &mqtt_payload_info.bitmaskVal, sizeof(mqtt_payload_info.bitmaskVal));

  // Set the header information
  time(&current_time);
  mqtt_payload_data.header.timestamp_old = mqtt_payload_data.header.timestamp_now ? mqtt_payload_data.header.timestamp_now : current_time;
  mqtt_payload_data.header.timestamp_now = current_time;
  uint32_t uuid = atoi(gateway_credentials_get_string(UUID));
  memcpy(&mqtt_payload_data.header.uuid, &uuid, sizeof(mqtt_payload_data.header.uuid));

  // Send the MQTT packet
  // TODO: Set the UUID
  if(wifi_active)
  {
   set_led_behaviour(Wifi, Off);
  }
  if (LTE4G_active)
  {
  set_led_behaviour(LTE4G, Off);
  }
  
  int mqttKey = *mqtt_payload_info.key;
  if (
    LTE4G_active &&
      !dignostic_mode && 
          ((mqttKey == KEY_ID_HP_RAK_MANUFACTURER_DATA) 
        || (mqttKey == KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA) 
        || (mqttKey == KEY_ID_HP_RAK_COMPRESSOR_MNUFACTURER_DATA)
        || (mqttKey == KEY_ID_HP_INV02_MANUFACTURER_DATA) 
        || (mqttKey == KEY_ID_HP_INV02_MANUAL_MANUFACTURER_DATA))
  )
  {
    // ESP_LOGI(TAG, "Save history Topic name %s", gw_to_cloud_topics[route_container]);
    // ESP_LOGI(TAG, "Save history Packet %d, %.*s", mqtt_payload_info.packetSize, mqtt_payload_info.packetSize, (const char *)&mqtt_payload_data);
    msg_id = history_log_save(current_time,(const char *)&mqtt_payload_data, mqtt_payload_info.packetSize);
    // ESP_LOG_BUFFER_HEXDUMP(TAG, (const char *)&mqtt_payload_data, mqtt_payload_info.packetSize, ESP_LOG_INFO);
  }
  else
  {
    ESP_LOGI(TAG, "Send mqtt Topic name %s", gw_to_cloud_topics[route_container]);
    ESP_LOGI(TAG, "Send mqtt Packet %d, %.*s", mqtt_payload_info.packetSize, mqtt_payload_info.packetSize, (const char *)&mqtt_payload_data);
    msg_id = esp_mqtt_client_publish((esp_mqtt_client_handle_t)client, gw_to_cloud_topics[route_container],
                                        (const char *)&mqtt_payload_data, mqtt_payload_info.packetSize, 0, 0);
                                        // ESP_LOG_BUFFER_HEXDUMP(TAG, (const char *)&mqtt_payload_data, mqtt_payload_info.packetSize, ESP_LOG_INFO);
    ESP_LOGI(TAG, "Published MQTT msg_id=%d", msg_id);
  }

  if(wifi_active)
  {
   set_led_behaviour(Wifi,On);
  }
   if (LTE4G_active)
  {
  set_led_behaviour(LTE4G, On);
  }
  // Reset the packet and data info
  mqtt_payload_info.key = NULL;
  mqtt_payload_info.bitmaskPtr = NULL;
  // mqtt_payload_data.header.timestamp_old = mqtt_payload_data.header.timestamp_now;

  return msg_id;
}

void mqtt_live_status_send(void *client)
{
  
  time_t current_time;
 
  // Set the header information
  time(&current_time);
  mqtt_payload_data.header.timestamp_old = mqtt_payload_data.header.timestamp_now ? mqtt_payload_data.header.timestamp_now : current_time;
  mqtt_payload_data.header.timestamp_now = current_time;
  uint32_t uuid = atoi(gateway_credentials_get_string(UUID));
  memcpy(&mqtt_payload_data.header.uuid, &uuid, sizeof(mqtt_payload_data.header.uuid));

  if(rak_is_active)
  {
    esp_mqtt_client_publish((esp_mqtt_client_handle_t)client, gw_to_cloud_topics[workRak01],
                                        (const char *)&mqtt_payload_data, 12, 0, 0);
  }
  if(inv_is_active || salt_is_active)
  {
    esp_mqtt_client_publish(client, gw_to_cloud_topics[workInv02],(const char *)&mqtt_payload_data, 12, 0, 0);
  }                                     
                                        
}

void mqtt_modbus_error_send(void *client)
{
  bool modbus_error = true;
  // time_t current_time;
  

  mqtt_data_add(KEY_ID_HP_INV02_ERRORS, HP_MODBUS_COMMUNICATION_ERROR, modbus_error, 0xFFFF);
  mqtt_data_send(client,workInv02);
  mqtt_data_add(KEY_ID_HP_RAK_ERRORS, HP_MODBUS_COMMUNICATION_ERROR, modbus_error, 0xFFFF);
  mqtt_data_send(client,workRak01);

                                       
}

// int count_set_bits(uint16_t n)
// {
//   int count = 0;
//   while (n)
//   {
//     count += n & 1;
//     n >>= 1;
//   }
//   return count;
// }

static uint16_t get_cid(uint16_t cid_offset, uint8_t reading_key, uint16_t bit_index)
{
  ESP_LOGI(TAG, "get_cid reading_key: %d, bit_index: %d, from offset:%d", reading_key, bit_index,cid_offset);
  for (uint16_t cid = cid_offset; cid <= MODBUS_CID_TOTAL_NR; cid++)
  {
    if (OPT_KEY_ID(modbus_param_descriptor[cid].param_opts) == reading_key &&
        OPT_BIT_IX(modbus_param_descriptor[cid].param_opts) == bit_index)
    {
      // ESP_LOGI(TAG, "Get CID: %d", cid);
      return cid;
    }
  }

  return 0;
}

static uint16_t get_cid_by_address(uint16_t cid_offset, uint16_t address)
{
  for (uint16_t cid = cid_offset; cid <= MODBUS_CID_TOTAL_NR; cid++)
  {
    // ESP_LOGI(TAG, "get_cid_by_address mb_reg_start: %d, address: %d", rak_heatpump_param_descriptor[cid].mb_reg_start, address);
    if (modbus_param_descriptor[cid].mb_reg_start == address)
    {
      return cid;
    }
  }

  return 0;
}

QueueHandle_t out_data_queue;
static bool is_queue_initialized = false;

parse_mqtt_request_status mqtt_out_device_topic_data_parse(uint16_t device_cid_offset, const char *data, int data_len)
{
  // ESP_LOGI(TAG, "Out topic data length: %d, data: %.*s", data_len, data_len, data);
  // ESP_LOGI(TAG, "Data item: %x, %x, %x, %x", (uint8_t)data[0], (uint8_t)data[1], (uint8_t)data[2], (uint8_t)data[3]);
  // out->uuid = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
  // ESP_LOGI(TAG, "UUID: %d", out->uuid);
  // int index = 4;
  // for (size_t i = 0; i < data_len; i++)
  // {
  //   ESP_LOGI(TAG, "Data item: %x, index: %d", (uint8_t)data[i], i);
  // }
  if (!is_queue_initialized)
  {
    out_data_queue = xQueueCreate(50, sizeof(modbus_update_item_t));
    is_queue_initialized = true;
  }

  // out->update_items_count = 0;
  int index = 0;
  while (index < data_len)
  {
    ESP_LOGI(TAG, "Data item: %x, index: %d", (uint8_t)data[index], index);
    // Get reading key
    if (index + 1 >= data_len)
    {
      return FAILED;
    }
    uint8_t rk = data[index];
    ESP_LOGI(TAG, "Reading key: %x", rk);
    index++;

    // Generate update items
    if (rk < 128) // Bit field logic
    {
      if (index + 1 >= data_len)
      {
        return FAILED;
      }
      uint16_t bf = data[index] | (data[index + 1] << 8);
      ESP_LOGI(TAG, "Bit field: : %x!", bf);
      index += 2;

      // Generate values
      uint16_t bit_field_mod = bf;
      int bit_index = 0;
      while (bit_field_mod)
      {
        if (bit_field_mod & 1)
        {
          if (index + 1 >= data_len)
          {
            return FAILED;
          }
          modbus_update_item_t item = {get_cid(device_cid_offset, rk, bit_index), (data[index] | (data[index + 1] << 8))};
          uint16_t *data = (uint16_t *)modbus_param_descriptor[item.cid].param_opts.opt3;

          // If we have defined mask for that CID shift and mask the data 
          if (modbus_param_descriptor[item.cid].param_opts.opt2 != 0xFFFF)
          {
            uint16_t mask = modbus_param_descriptor[item.cid].param_opts.opt2;

            // Shift the data to the right bit offset
            while ((mask & 0x0001) == 0)
            {
              mask >>=1;
              item.value <<=1;
            }

            // Mask the data
            item.value &= modbus_param_descriptor[item.cid].param_opts.opt2;

            // Add the data for the other fields in that modbus register
            item.value |= (*data & ~modbus_param_descriptor[item.cid].param_opts.opt2);
          }

          ESP_LOGI(TAG, "MB update item cid: %d, old_val:%d, new_value: %d, bit_index: %d", item.cid, *data, item.value, bit_index);

          // Update the current value of the modbus register
          *data = item.value;

          // modbus_update_item_t *p_item = &item;
          xQueueSend(out_data_queue, &item, (TickType_t)0);
          // out->update_items[out->update_items_count] = item;
          // out->update_items_count++;
          index += 2;
        }
        bit_field_mod >>= 1;
        bit_index++;
      }
    }
    else // Modbus address logic for writing RAW data
    {
      if (data_len != 5)
      {
        return FAILED;
      }
      uint16_t address = (data[index] | (data[index + 1] << 8));
      uint16_t cid = get_cid_by_address(device_cid_offset, address);
      uint16_t value = (data[index + 2] | (data[index + 3] << 8));
      modbus_update_item_t item = {cid, value};

      modbus_update_item_t *p_item = &item;
      xQueueSend(out_data_queue, &item, (TickType_t)0);
      // out->update_items[out->update_items_count] = item;
      // out->update_items_count++;
      return SUCCESS;
    }
  }

  return SUCCESS;
}