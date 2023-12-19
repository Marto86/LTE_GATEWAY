#include <string.h>
#include "../gateway_credentials.h"
#include "./poly_types.h"
#include "esp_log.h"
#include "./poly_routing.h"

//Topics in/out
char *container_types_strings[] = {
    "workInv02",
    "manuInv02",
    "configInv02",
    "workRak01",
    "manuRak01",
    "configRak01",
    "raw",
    "history",
    "diagnostic",
    "info",
    "Logs"
    };

//command topic type strings
char *command_types_strings[] = {
    "getdata",
    "history",
    "diagnostic",
    "otalink",
    "info"};

char *gw_to_cloud_topics[] = {
    "", // BUILD_ROUTE6(GATEWAY_ROOT, GATEWAY_DIR_IN, GATEWAY_VERSION, GATEWAY_SECTOR, GATEWAY_UUID, CONTAINER_TYPE_INVERTER1),
    "", // BUILD_ROUTE6(GATEWAY_ROOT, GATEWAY_DIR_IN, GATEWAY_VERSION, GATEWAY_SECTOR, GATEWAY_UUID, CONTAINER_TYPE_WORKING),
    "", // BUILD_ROUTE6(GATEWAY_ROOT, GATEWAY_DIR_IN, GATEWAY_VERSION, GATEWAY_SECTOR, GATEWAY_UUID, CONTAINER_TYPE_MANUFACTURER),
    "", // BUILD_ROUTE6(GATEWAY_ROOT, GATEWAY_DIR_IN, GATEWAY_VERSION, GATEWAY_SECTOR, GATEWAY_UUID, CONTAINER_TYPE_CONFIGURATION),
    "", // BUILD_ROUTE6(GATEWAY_ROOT, GATEWAY_DIR_IN, GATEWAY_VERSION, GATEWAY_SECTOR, GATEWAY_UUID, CONTAINER_TYPE_RAW),
    ""
};

char *cloud_to_gw_topics[] = {
    "", // BUILD_ROUTE5(GATEWAY_ROOT, GATEWAY_DIR_OUT, GATEWAY_VERSION, GATEWAY_UUID, CONTAINER_TYPE_INVERTER1),
    "", // BUILD_ROUTE5(GATEWAY_ROOT, GATEWAY_DIR_OUT, GATEWAY_VERSION, GATEWAY_UUID, CONTAINER_TYPE_WORKING),
    "", // BUILD_ROUTE5(GATEWAY_ROOT, GATEWAY_DIR_OUT, GATEWAY_VERSION, GATEWAY_UUID, CONTAINER_TYPE_MANUFACTURER),
    "", // BUILD_ROUTE5(GATEWAY_ROOT, GATEWAY_DIR_OUT, GATEWAY_VERSION, GATEWAY_UUID, CONTAINER_TYPE_CONFIGURATION),
    "" // BUILD_ROUTE5(GATEWAY_ROOT, GATEWAY_DIR_OUT, GATEWAY_VERSION, GATEWAY_UUID, CONTAINER_TYPE_RAW),
};

char *command_topics[] = {
    "",
    "",
    "",
    "",
    "",
};

char *command_response_topics[] = {
    "",
    "",
    "",
    "",
    "",
};

char *ack_topic;

char *concat_strings_as_topic_5(const char *s0, const char *s1, const char *s2, const char *s3, const char *s4)
{
    int size = strlen(s0) + strlen(s1) + strlen(s2) + strlen(s3) + strlen(s4) + 5;
    char *new_string2 = (char *)malloc(size * sizeof(char));
    new_string2[0] = '\0';
    strcat(new_string2, s0);
    strcat(new_string2, "/");
    strcat(new_string2, s1);
    strcat(new_string2, "/");
    strcat(new_string2, s2);
    strcat(new_string2, "/");
    strcat(new_string2, s3);
    strcat(new_string2, "/");
    strcat(new_string2, s4);
    return new_string2;
}

char *concat_strings_as_topic_6(const char *s0, const char *s1, const char *s2, const char *s3, const char *s4, const char *s5)
{
    int size = strlen(s0) + strlen(s1) + strlen(s2) + strlen(s3) + strlen(s4) + strlen(s5) + 6;
    char *new_string2 = (char *)malloc(size * sizeof(char));
    new_string2[0] = '\0';
    strcat(new_string2, s0);
    strcat(new_string2, "/");
    strcat(new_string2, s1);
    strcat(new_string2, "/");
    strcat(new_string2, s2);
    strcat(new_string2, "/");
    strcat(new_string2, s3);
    strcat(new_string2, "/");
    strcat(new_string2, s4);
    strcat(new_string2, "/");
    strcat(new_string2, s5);
    return new_string2;
}

void poly_init_routes()
{
    for (size_t i = 0; i < ContainerTypeEnumSize; i++)
    {
        char *new_item = concat_strings_as_topic_6(
            GATEWAY_ROOT,
            GATEWAY_DIR_IN,
            GATEWAY_VERSION,
            (const char *)gateway_credentials_get_string(SECTOR),
            (const char *)gateway_credentials_get_string(UUID),
            (const char *)container_types_strings[i]);

        gw_to_cloud_topics[i] = new_item;
        ESP_LOGI("POLY_ROUTING", "Gateway to cloud Topic: %s", gw_to_cloud_topics[i]);
    }

    for (size_t i = 0; i < ContainerTypeEnumSize; i++)
    {
        char *new_item = concat_strings_as_topic_5(
            GATEWAY_ROOT,
            GATEWAY_DIR_OUT,
            GATEWAY_VERSION,
            (const char *)gateway_credentials_get_string(UUID),
            (const char *)container_types_strings[i]);
        cloud_to_gw_topics[i] = new_item;
        ESP_LOGI("POLY_ROUTING", "Cloud to gateway Topic: %s", cloud_to_gw_topics[i]);
    }

    for (size_t i = 0; i < CommandTypeEnumSize; i++)
    {
        char *new_item = concat_strings_as_topic_5(
            GATEWAY_ROOT,
            GATEWAY_COMMAND,
            GATEWAY_VERSION,
            (const char *)gateway_credentials_get_string(UUID),
            (const char *)command_types_strings[i]);
        command_topics[i] = new_item;
        ESP_LOGI("POLY_ROUTING", "Command Topic: %s", command_topics[i]);

        char *new_item_resp = concat_strings_as_topic_6(
            GATEWAY_ROOT,
            GATEWAY_COMMAND,
            GATEWAY_VERSION,
            "ack",
            (const char *)gateway_credentials_get_string(UUID),
            (const char *)command_types_strings[i]);
        command_response_topics[i] = new_item_resp;
    }
}

TopicType poly_get_topic_type(char *route, uint8_t* topic_type)
{
    ESP_LOGI("POLY_ROUTING", "poly_get_topic_type route: %s", route);
    for (size_t i = 0; gw_to_cloud_topics[i]; i++)
    {
        if (strcmp(route, gw_to_cloud_topics[i]) == 0)
        {
            return GWTC;
        }
    }

    for (size_t i = 0; cloud_to_gw_topics[i]; i++)
    {
        if (strcmp(route, cloud_to_gw_topics[i]) == 0)
        {
            *topic_type = i;
            return CTGW;
        }
    }

    for (size_t i = 0; command_topics[i]; i++)
    {
        if (strcmp(route, command_topics[i]) == 0)
        {
            *topic_type = i;
            return CMD;
        }
    }

    return UNDEFINED;
}

int get_cmd_topic_type(char *route)
{
    for (size_t i = 0; command_topics[i]; i++)
    {
        if (strcmp(route, command_topics[i]) == 0)
        {
            return i;
        }
    }

    return -1;
}