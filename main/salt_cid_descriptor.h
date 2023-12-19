
// Data (Object) Dictionary for Modbus parameters:
// The CID field in the table must be unique.
// Modbus Slave Addr field defines slave address of the device with correspond parameter.
// Modbus Reg Type - Type of Modbus register area (Holding register, Input Register and such).
// Reg Start field defines the start Modbus register number and Reg Size defines the number of registers for the characteristic accordingly.
// The Instance Offset defines offset in the appropriate parameter structure that will be used as instance to save parameter value.
// Data Type, Data Size specify type of the characteristic and its data size.
// Parameter Options field specifies the options that can be used to process parameter value (limits or masks).
//      In the current heatpump registers, the options are used as following:
//          opt1: Key ID (MSB 16 bit) | Bit number for the key (LSB 16 bit)
//          opt2: Bit mask for the value (0xFFFF if no specific bitmask)
//          opt3: Pointer to the data
// Access Mode - can be used to implement custom options for processing of characteristic (Read/Write restrictions, factory mode values and etc).

// { CID, Param Name, Units, Modbus Slave Addr, Modbus Reg Type, Reg Start, Reg Size, Instance Offset, Data Type, Data Size, Parameter Options, Access Mode}

{CID_SALT_ELECTROLISE, STR("IsElectrolysisOn"), STR("On/Off"), MB_SALT, MB_PARAM_HOLDING, 769, 1,
 SALT_STATUS_OFFSET(isElectrolysisOn), PARAM_TYPE_U16, 2, OPTS(KEY_ID_SALT_WORK_DATA, SALT_WORK_BIT_NR_IS_ELECTROLISE_ON, (int)&salt_status.isElectrolysisOn), PAR_PERMS_READ},
{CID_SALT_VALUE, STR("Value"), STR("g/L"), MB_SALT, MB_PARAM_HOLDING, 261, 1,
 SALT_STATUS_OFFSET(value), PARAM_TYPE_U16, 2, OPTS(KEY_ID_SALT_WORK_DATA, SALT_WORK_BIT_NR_VALUE, (int)&salt_status.value), PAR_PERMS_READ},
{CID_SALT_ERRORS, STR("Errors"), STR("Enum"), MB_SALT, MB_PARAM_HOLDING, 1025, 1,
 SALT_STATUS_OFFSET(errors), PARAM_TYPE_U16, 2, OPTS(KEY_ID_SALT_WORK_DATA, SALT_WORK_BIT_NR_ERRORS, (int)&salt_status.errors), PAR_PERMS_READ},
{CID_SALT_IVERSION_PERIOD, STR("InversionPeriod"), STR("Hours"), MB_SALT, MB_PARAM_HOLDING, 769, 1,
 SALT_CONTROL_OFFSET(inversionPeriod), PARAM_TYPE_U16, 2, OPTS(KEY_ID_SALT_CONFIGURATION, SALT_WORK_BIT_INVERSION_PERIOD, (int)&salt_control.inversionPeriod), PAR_PERMS_READ_WRITE},
{CID_SALT_PRODUCTION_PERCENT, STR("ProductionPercent"), STR("%"), MB_SALT, MB_PARAM_HOLDING, 770, 1,
 SALT_CONTROL_OFFSET(productionPercent), PARAM_TYPE_U16, 2, OPTS(KEY_ID_SALT_CONFIGURATION, SALT_WORK_BIT_PRODUCTION_PERCENT, (int)&salt_control.productionPercent), PAR_PERMS_READ_WRITE},