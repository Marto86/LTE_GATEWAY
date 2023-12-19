#ifndef __POLY_ROUTING_H__
#define __POLY_ROUTING_H__

#include "./poly_types.h"

#define GATEWAY_ROOT "poly"
#define GATEWAY_DIR_IN "in"
#define GATEWAY_DIR_OUT "out"
#define GATEWAY_VERSION "1"
#define GATEWAY_COMMAND "cmd"

typedef enum ContainerTypeEnum
{
    workInv02,
    manuInv02,
    configInv02,
    workRak01,
    manuRak01,
    configRak01,
    Raw,
    ManufacturerHisto,
    Diagnostic,
    Info,
    Logs,
    ContainerTypeEnumSize
} ContainerTypeEnum;

char *container_types_strings[ContainerTypeEnumSize];

typedef enum CommandTypeEnum
{
    // WorkingCmd,
    // ManufacturerCmd,
    // ConfigurationCmd,
    // RawCmd,
    // ManufacturerHistoryCmd,
    // DiagnosticCmd,
    // OtaLinkCmd,
    // InfoCmd,
    // CommandTypeEnumSize
    CmdGetData,
    CmdHistory,
    CmdDiagnostic,
    CmdOtaLink,
    CmdInfo,
    CommandTypeEnumSize
} CommandTypeEnum;

char *gw_to_cloud_topics[ContainerTypeEnumSize];

char *cloud_to_gw_topics[ContainerTypeEnumSize];

char *command_topics[CommandTypeEnumSize];

char *command_response_topics[CommandTypeEnumSize];

char *command_types_strings[CommandTypeEnumSize];

void poly_init_routes();

/**
 * @param route Input parameter, Pointer to the received topic string
 * @param topic_type Output parameter, which returns the index of ContainerTypeEnum or CommandTypeEnum
 * @return Returns the Topic type
 */
TopicType poly_get_topic_type(char *route, uint8_t* topic_type);

int get_cmd_topic_type(char *route);

#endif //__POLY_ROUTING_H__