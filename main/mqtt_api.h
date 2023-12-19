#ifndef __MQTT_API_H__
#define __MQTT_API_H__

#include "poly_routing.h"
#include "heatpump_rak.h"
#include "inverter_02.h"

#define PUBLISH_MQTT_DATA_SIZE_MAX 128

typedef enum
{
  SUCCESS,
  FAILED
} parse_mqtt_request_status;

// Add data for sending
int mqtt_data_add(uint16_t key_id, uint16_t keyBitIx, uint16_t value, uint16_t valueBitMask);

// Send the added data
int mqtt_data_send(void *client, ContainerTypeEnum route_container);

void mqtt_modbus_error_send(void *client);

void mqtt_live_status_send(void *client);
// Parse modbus update data for specific device (defined by CID offset)
parse_mqtt_request_status mqtt_out_device_topic_data_parse(uint16_t device_cid_offset, const char *data, int data_len);


QueueHandle_t out_data_queue;

#endif //__MQTT_API_H__
