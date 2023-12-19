#ifndef __GATEWAY_CREDENTIALS_H__
#define __GATEWAY_CREDENTIALS_H__

// #define GATEWAY_UUID_INT 43981
// #define GATEWAY_SERIAL_NUMBER "PC-B-22-05-00043981"
// #define GATEWAY_MQTT_CLIENT_ID "wildcard9"
// #define GATEWAY_MQTT_CLIENT_USER "wildcard9"
// #define GATEWAY_MQTT_PASSWORD "7e0bb4ccf1a68c3"
// #define GATEWAY_SECTOR "303"

typedef enum CredentialSetType
{
    UUID,
    SERIAL_NUMBER,
    MQTT_CLIENT_ID,
    MQTT_CLIENT_USER,
    MQTT_CLIENT_PASSWORD,
    SECTOR
} CredentialSetType;

char *gateway_credentials_get_string(CredentialSetType type);

int gateway_credentials_set_string(CredentialSetType type, char *value);

#endif //__GATEWAY_CREDENTIALS_H__