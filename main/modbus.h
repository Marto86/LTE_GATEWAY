#ifndef __MODBUS_H__
#define __MODBUS_H__
#include "mbcontroller.h"
#include <stdbool.h>
#include "salt.h"

/*=====================================================================================
 * Description:
 *   The Modbus parameter structures used to define Modbus instances that
 *   can be addressed by Modbus protocol. 
 *====================================================================================*/

#define HP_INV_02_BAUD_RATE (9600)
#define HP_RAK_01_BAUD_RATE (19200)
#define SALT_BAUD_RATE  (19200)

bool rak_is_active;
bool inv_is_active;
bool salt_is_active;

bool dignostic_mode;

#define STR(fieldname) ((const char *)(fieldname))
// The Modbus options with defalut value bitmask 0xFFFF (i.e. no specific bitmask for the data)
#define OPTS(key_id, bitIx, valueAddr)                   \
    {                                                      \
        .opt1 = (key_id<<16) | bitIx, .opt2 = 0xFFFF, .opt3 = valueAddr \
    }

// The Modbus options with specific value bitmask defined as the last parameter
#define OPTS_VALUE_BITMASK(key_id, bitIx, valueAddr, valueBitMask)            \
    {                                                                         \
        .opt1 = (key_id<<16) | bitIx, .opt2 = valueBitMask, .opt3 = valueAddr \
    }

#define OPT_KEY_ID(paramOpts)   (paramOpts.opt1>>16)
#define OPT_BIT_IX(paramOpts)   (paramOpts.opt1&0xFFFF)

typedef struct
{
  int cid;
  uint16_t value;
} modbus_update_item_t;

typedef struct
{
  uint32_t uuid;
  modbus_update_item_t *update_items;
  int update_items_count;
} mudbus_update_request_data_t;

enum
{
#include <inverter_02_cid.h>
#include <heatpump_rak_cid.h>
#include <salt_equipment_cid.h>
  MODBUS_CID_TOTAL_NR
};

extern const mb_parameter_descriptor_t modbus_param_descriptor[];

// Modbus master initialization
int modbus_init(void);

bool modbus_test(uint16_t cid, uint32_t baud_rate, uart_parity_t parity);


void apply_out_topic_messages();

typedef enum {
  WorkingGetDataCommand = 1,
  ManufacturingGetDataCommand,
  ConfigurationGetDataCommand
} GetDataCommandType;

void modbus_commands_queue_init();

void modbus_commands_queue_push(int command_id);

void modbus_execute_pending_commands(void *client);

#endif // !defined(__MODBUS_H__)
