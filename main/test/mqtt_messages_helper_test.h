#ifndef __MQTT_MESSAGES_HELPER_TEST_H__
#define __MQTT_MESSAGES_HELPER_TEST_H__

// #include "unity.h" // TODO
#include "esp_log.h"
#include "mqtt_messages_helper.h"

#define MQTT_MESSAGES_HELPER_TEST "mqtt_messages_helper_test"

// TODO
// TEST_CASE("Test mqtt_messages_helper", "test")
// {

// }

void mqtt_messages_helper_test()
{
    if(message_reading_is_bit_field(10) == true)
    {
        ESP_LOGI(MQTT_MESSAGES_HELPER_TEST, "message_reading_is_bit_field(10) == true");
    }
    else
    {
        ESP_LOGE(MQTT_MESSAGES_HELPER_TEST, "message_reading_is_bit_field(10) == true");
    }

    if(message_reading_is_bit_field(200) == false)
    {
        ESP_LOGI(MQTT_MESSAGES_HELPER_TEST, "message_reading_is_bit_field(200) == false");
    }
    else
    {
        ESP_LOGE(MQTT_MESSAGES_HELPER_TEST, "message_reading_is_bit_field(200) == false");
    }

    const char* invalid_route = "asd";
    TopicType tt = get_topic_type(invalid_route);
    if(tt == UNDEFINED)
    {
        ESP_LOGI(MQTT_MESSAGES_HELPER_TEST, "get_topic_type(invalid_route) == UNDEFINED");
    }
    else
    {
        ESP_LOGE(MQTT_MESSAGES_HELPER_TEST, "invalid_route(invalid_route) == UNDEFINED");
    }
}

#endif //__MQTT_MESSAGES_HELPER_TEST_H__