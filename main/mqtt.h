#ifndef __MQTT_H__
#define __MQTT_H__

bool mqtt_is_connected();
// Returns pointer to MQTT client
void* mqtt_start(void);

#endif //__MQTT_H__
