#ifndef __salt_H__
#define __salt_H__

#include "poly_routing.h"
#include "modbus.h"
#include "mbcontroller.h"

/*=====================================================================================
 * Description:
 *   The Modbus parameter structures used to define Modbus instances that
 *   can be addressed by Modbus protocol. Define these structures for RAK heatupmp
 *====================================================================================*/

// This file defines structure of modbus parameters which reflect correspond modbus address space
// for each modbus register

#pragma pack(push, 1)
typedef struct
{
    uint16_t isElectrolysisOn;
    uint16_t value;
    uint16_t errors;

} salt_status_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
 uint16_t inversionPeriod;
 uint16_t productionPercent;

} salt_control_t;
#pragma pack(pop)
typedef enum
{
    KEY_ID_SALT_REPORT = 0,
    KEY_ID_SALT_WORK_DATA = 0x08,
    KEY_ID_SALT_CONFIGURATION = 0x09,
    KEY_ID_SALT_RAW_DATA_ = 0x83,
    KEY_ID_SALT_PH_RAW_DATA = 0x81,
    KEY_ID_SALT_TEST_COMMUNICATION,
} salt_key_id_t;

enum
{
    SALT_WORK_BIT_NR_IS_ELECTROLISE_ON = 0,
    SALT_WORK_BIT_NR_VALUE,
    SALT_WORK_BIT_NR_ERRORS,
    SALT_MODBUS_COMMUNICATION_ERROR = 15,
   
};

enum
{
    SALT_WORK_BIT_INVERSION_PERIOD,
    SALT_WORK_BIT_PRODUCTION_PERCENT,
};

typedef struct
{
    // This is the LSB
    uint16_t isElectrolysisOn : 1;
    uint16_t value : 1;
    uint16_t errors : 1;
 
} salt_work_data_bits_t;

typedef union
{
    salt_work_data_bits_t bit;
    uint16_t value;
} salt_work_data_t;

typedef struct
{
    uint16_t inversionPeriod : 1;
    uint16_t productionPercent : 1;
} salt_config_bits_t;

// Enumeration of modbus device addresses accessed by master device
enum
{
    MB_SALT = 10 // Only one slave device used for the test (add other slave addresses here)
};

#define CID_SALT_OFFSET CID_SALT_ELECTROLISE
#define CID_SALT_MAX_NUMBER CID_SALT_PRODUCTION_PERCENT

// The macro to get offset for parameter in the appropriate structure
#define SALT_STATUS_OFFSET(field) ((uint16_t)(offsetof(salt_status_t, field) + 1))
#define SALT_STATUS_PTR(param) ((void *)(&salt_control + param->param_offset - 1))

#define SALT_CONTROL_OFFSET(field) ((uint16_t)(offsetof(salt_control_t, field) + 1))
#define SALT_HP_CONTROL_PTR(param) ((void *)(&salt_control + param->param_offset - 1))


extern salt_status_t salt_status;

extern salt_control_t salt_control;

void salt_operate(salt_key_id_t key_id, void *mqtt_client, bool force_resend, ContainerTypeEnum container_type);

void salt_work(time_t *current_time, time_t *previous_force_time, time_t *previous_sent_time, time_t *all_data_previous_sent_time);
#endif // !defined(__salt_H__)