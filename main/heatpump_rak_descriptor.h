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
// Working data
// STATUS Parameters
{CID_HP_RAK_WATER_IN_TEMPERATURE, STR("rak_WaterInTemperature"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2111, 1,
 RAK_HP_STATUS_OFFSET(waterInTemp), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_WORK_DATA, HP_RAK_WORK_BIT_NR_WATER_IN_TEMPERATURE, (int)&rak_heatpump_status.waterInTemp), PAR_PERMS_READ},
    {CID_HP_RAK_AMBIENT_IN_TEMPERATURE, STR("rak_AmbientInTemperature"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2100, 1,
     RAK_HP_STATUS_OFFSET(ambientInTemp), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_WORK_DATA, HP_RAK_WORK_BIT_NR_AMBIENT_IN_TEMPERATURE, (int)&rak_heatpump_status.ambientInTemp), PAR_PERMS_READ},
    {CID_HP_RAK_MODE, STR("rak_Mode"), STR("__"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 1999, 1,
     RAK_HP_STATUS_OFFSET(mode), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_WORK_DATA, HP_RAK_WORK_BIT_NR_MODE, (int)&rak_heatpump_status.mode), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_IS_DEFROSTING, STR("rak_IsDefrosting"), STR("on/off"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2110, 1,
     RAK_HP_STATUS_OFFSET(isDefrosting), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_WORK_DATA, HP_RAK_WORK_BIT_NR_IS_DEFROSTING, (int)&rak_heatpump_status.isDefrosting), PAR_PERMS_READ},
    {CID_HP_RAK_IS_FLOWING, STR("rak_IsFlowing"), STR("On/Off"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2102, 1,
     RAK_HP_STATUS_OFFSET(isFlowing), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_WORK_DATA, HP_RAK_WORK_BIT_NR_IS_FLOWING, (int)&rak_heatpump_status.isFlowing), PAR_PERMS_READ},
    {CID_HP_RAK_IS_WATER_PUMP_ENABLE, STR("rak_IsWaterPumpEnabled"), STR("Enable/Disable"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2103, 1,
     RAK_HP_STATUS_OFFSET(isWaterPumpEnabled), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_WORK_DATA, HP_RAK_WORK_BIT_NR_IS_WATER_PUMP_ENABLED, (int)&rak_heatpump_status.isWaterPumpEnabled), PAR_PERMS_READ},
    {CID_HP_RAK_HEATING_TEMPERATURE_ORDER, STR("rak_HeatingTemperatureOrder"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2000, 1,
     RAK_HP_STATUS_OFFSET(heatingTempOrder), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_WORK_DATA, HP_RAK_WORK_BIT_NR_HEATING_TEMPERATURE_ORDER, (int)&rak_heatpump_status.heatingTempOrder), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_COOLING_TEMPERATURE_ORDER, STR("rak_CoolingTemperatureOrder"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2001, 1,
     RAK_HP_STATUS_OFFSET(coolingTempOrder), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_WORK_DATA, HP_RAK_WORK_BIT_NR_COOLING_TEMPERATURE_ORDER, (int)&rak_heatpump_status.coolingTempOrder), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_CIRCULATION_RUNNING_MODE, STR("rak_CirculationRunningMode"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3040, 1,
     RAK_HP_STATUS_OFFSET(circulationRunningMode), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_WORK_DATA, HP_RAK_WORK_BIT_NR_RAK_CIRCULATION_RUNNING_MODE, (int)&rak_heatpump_status.circulationRunningMode), PAR_PERMS_READ},  
    // ERRORS
    {CID_HP_RAK_ERRORS_PROTECTION, STR("rak_Errors_2104"), STR("__"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2104, 1,
     RAK_HP_ERROR_OFFSET(errors2104), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_ERRORS, ERRORS_BIT_NR_HP_RAK_ERROR2104, (int)&rak_heatpump_errors.errors2104), PAR_PERMS_READ},
    {CID_HP_RAK_ERRORS_2105, STR("rak_Errors_2105"), STR("__"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2105, 1,
     RAK_HP_ERROR_OFFSET(errors2105), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_ERRORS, ERRORS_BIT_NR_HP_RAK_ERROR2105, (int)&rak_heatpump_errors.errors2105), PAR_PERMS_READ},
    {CID_HP_RAK_ERRORS_2136, STR("rak_Errors_2136"), STR("__"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2136, 1,
     RAK_HP_ERROR_OFFSET(errors2136), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_ERRORS, ERRORS_BIT_NR_HP_RAK_ERROR2136, (int)&rak_heatpump_errors.errors2136), PAR_PERMS_READ},
    {CID_HP_RAK_ERRORS_2137, STR("rak_Errors_2137"), STR("__"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2137, 1,
     RAK_HP_ERROR_OFFSET(errors2137), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_ERRORS, ERRORS_BIT_NR_HP_RAK_ERROR2137, (int)&rak_heatpump_errors.errors2137), PAR_PERMS_READ},
    {CID_HP_RAK_ERRORS_2138, STR("rak_Errors_2138"), STR("__"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2138, 1,
     RAK_HP_ERROR_OFFSET(errors2138), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_ERRORS, ERRORS_BIT_NR_HP_RAK_ERROR2138, (int)&rak_heatpump_errors.errors2138), PAR_PERMS_READ},
    {CID_HP_RAK_ERRORS_2184, STR("rak_Errors_2184"), STR("__"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2184, 1,
     RAK_HP_ERROR_OFFSET(errors2184), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_ERRORS, ERRORS_BIT_NR_HP_RAK_ERROR2184, (int)&rak_heatpump_errors.errors2184), PAR_PERMS_READ},
    {CID_HP_RAK_ERRORS_2185, STR("rak_Errors_2185"), STR("__"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2185, 1,
     RAK_HP_ERROR_OFFSET(errors2185), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_ERRORS, ERRORS_BIT_NR_HP_RAK_ERROR2185, (int)&rak_heatpump_errors.errors2185), PAR_PERMS_READ},
    {CID_HP_RAK_ERRORS_2186, STR("rak_Errors_2186"), STR("__"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2186, 1,
     RAK_HP_ERROR_OFFSET(errors2186), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_ERRORS, ERRORS_BIT_NR_HP_RAK_ERROR2186, (int)&rak_heatpump_errors.errors2186), PAR_PERMS_READ},

    // Control parameters
    // Fan Configuration Data
    {CID_HP_RAK_FAN_SPEED_LVL_1, STR("rak_FanSpeedLevel_1"), STR("RPM"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3008, 1,
     RAK_HP_CONTROL_OFFSET(fanSpeedLevel1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_CONFIGURATION_FAN, HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_1, (int)&rak_heatpump_control.fanSpeedLevel1), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_FAN_SPEED_LVL_2, STR("rak_FanSpeedLevel_2"), STR("RPM"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3009, 1,
     RAK_HP_CONTROL_OFFSET(fanSpeedLevel2), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_CONFIGURATION_FAN, HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_2, (int)&rak_heatpump_control.fanSpeedLevel2), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_FAN_SPEED_LVL_3, STR("rak_FanSpeedLevel_3"), STR("RPM"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3010, 1,
     RAK_HP_CONTROL_OFFSET(fanSpeedLevel3), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_CONFIGURATION_FAN, HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_3, (int)&rak_heatpump_control.fanSpeedLevel3), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_FAN_SPEED_LVL_4, STR("rak_FanSpeedLevel_4"), STR("RPM"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3011, 1,
     RAK_HP_CONTROL_OFFSET(fanSpeedLevel4), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_CONFIGURATION_FAN, HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_4, (int)&rak_heatpump_control.fanSpeedLevel4), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_FAN_SPEED_LVL_5, STR("rak_FanSpeedLevel_5"), STR("RPM"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3012, 1,
     RAK_HP_CONTROL_OFFSET(fanSpeedLevel5), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_CONFIGURATION_FAN, HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_5, (int)&rak_heatpump_control.fanSpeedLevel5), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_FAN_SPEED_LVL_6, STR("rak_FanSpeedLevel_6"), STR("RPM"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3013, 1,
     RAK_HP_CONTROL_OFFSET(fanSpeedLevel6), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_CONFIGURATION_FAN, HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_6, (int)&rak_heatpump_control.fanSpeedLevel6), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_FAN_SPEED_LVL_7, STR("rak_FanSpeedLevel_7"), STR("RPM"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3014, 1,
     RAK_HP_CONTROL_OFFSET(fanSpeedLevel7), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_CONFIGURATION_FAN, HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_7, (int)&rak_heatpump_control.fanSpeedLevel7), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_FAN_SPEED_LVL_8, STR("rak_FanSpeedLevel_8"), STR("RPM"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3015, 1,
     RAK_HP_CONTROL_OFFSET(fanSpeedLevel8), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_CONFIGURATION_FAN, HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_8, (int)&rak_heatpump_control.fanSpeedLevel8), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_FAN_SPEED_LVL_9, STR("rak_FanSpeedLevel_9"), STR("RPM"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3016, 1,
     RAK_HP_CONTROL_OFFSET(fanSpeedLevel9), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_CONFIGURATION_FAN, HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_9, (int)&rak_heatpump_control.fanSpeedLevel9), PAR_PERMS_READ_WRITE},

    // Manufacturer Data
    {CID_HP_RAK_WATER_OUT_TEMPERATURE, STR("rak_WaterOutTemperature"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2112, 1,
     RAK_HP_STATUS_OFFSET(waterOutTemp), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_WATER_OUT_TEMPERATURE, (int)&rak_heatpump_status.waterOutTemp), PAR_PERMS_READ},
    {CID_HP_RAK_FAN_SPEED_1, STR("rak_FanSpeed_1"), STR("RPM"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2134, 1,
     RAK_HP_STATUS_OFFSET(fanSpeed1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_FAN_SPEED_1, (int)&rak_heatpump_status.fanSpeed1), PAR_PERMS_READ},
    {CID_HP_RAK_FAN_SPEED_2, STR("rak_FanSpeed_2"), STR("RPM"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2182, 1,
     RAK_HP_STATUS_OFFSET(fanSpeed2), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_FAN_SPEED_2, (int)&rak_heatpump_status.fanSpeed2), PAR_PERMS_READ},
    {CID_HP_RAK_EEV_OPENING_1, STR("rak_EevOpening_1"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2122, 1,
     RAK_HP_STATUS_OFFSET(eevOpening1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_EEVOPENING_1, (int)&rak_heatpump_status.eevOpening1), PAR_PERMS_READ},
    {CID_HP_RAK_EEV_OPENING_2, STR("rak_EevOpening_2"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2170, 1,
     RAK_HP_STATUS_OFFSET(eevOpening1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_EEVOPENING_2, (int)&rak_heatpump_status.eevOpening2), PAR_PERMS_READ},
    {CID_HP_RAK_EEV_MODE_1, STR("rak_EevMode_1"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3051, 1,
     RAK_HP_STATUS_OFFSET(eevMode1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_EEV_MODE_1, (int)&rak_heatpump_status.eevMode1), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_EEV_MODE_2, STR("rak_EevMode_2"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3055, 1,
     RAK_HP_STATUS_OFFSET(eevMode1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_EEV_MODE_2, (int)&rak_heatpump_status.eevMode2), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_FAN_MODE_1, STR("rak_FanMode_1"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3053, 1,
     RAK_HP_STATUS_OFFSET(fanMode1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_FAN_MODE_1, (int)&rak_heatpump_status.fanMode1), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_FAN_MODE_2, STR("rak_FanMode_2"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3057, 1,
     RAK_HP_STATUS_OFFSET(fanMode2), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_FAN_MODE_2, (int)&rak_heatpump_status.fanMode2), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_COMPRESSOR_MODE_1, STR("rak_Compressor_Mode_1"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3054, 1,
     RAK_HP_STATUS_OFFSET(compressorMode1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_COMPRESSOR_MODE_1, (int)&rak_heatpump_status.compressorMode1), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_COMPRESSOR_MODE_2, STR("rak_Compressor_Mode_2"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3058, 1,
     RAK_HP_STATUS_OFFSET(compressorMode2), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_COMPRESSOR_MODE_2, (int)&rak_heatpump_status.compressorMode2), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_EEV_MANUAL_1, STR("rak_EevManual_1"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2008, 1,
     RAK_HP_STATUS_OFFSET(eevManual1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_EEV_MANUAL_1, (int)&rak_heatpump_status.eevManual1), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_EEV_MANUAL_2, STR("rak_EevManual_2"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2012, 1,
     RAK_HP_STATUS_OFFSET(eevManual2), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_EEV_MANUAL_2, (int)&rak_heatpump_status.eevManual2), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_FAN_SPEED_MANUAL_1, STR("rak_FanSpeed_Manual_1"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2010, 1,
     RAK_HP_STATUS_OFFSET(fanSpeedManual1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_FAN_SPEED_MANUAL_1, (int)&rak_heatpump_status.fanSpeedManual1), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_FAN_SPEED_MANUAL_2, STR("rak_FanSpeed_Manual_2"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2014, 1,
     RAK_HP_STATUS_OFFSET(fanSpeedManual2), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_FAN_SPEED_MANUAL_2, (int)&rak_heatpump_status.fanSpeedManual2), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_COMPRESSOR_SPEED_MANUAL_1, STR("rak_Compressor_Speed_Manual_1"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2011, 1,
     RAK_HP_STATUS_OFFSET(compressorSpeedManual1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_COMPRESSOR_SPEED_MANUAL_1, (int)&rak_heatpump_status.compressorSpeedManual1), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_COMPRESSOR_SPEED_MANUAL_2, STR("rak_Compressor_Speed_Manual_2"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2015, 1,
     RAK_HP_STATUS_OFFSET(compressorSpeedManual2), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_COMPRESSOR_SPEED_MANUAL_2, (int)&rak_heatpump_status.compressorSpeedManual2), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_DISCHARGE_TEMPERATURE_1, STR("rak_Discharge_Temperature_1"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2113, 1,
     RAK_HP_STATUS_OFFSET(dischargeTemperature1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_DISCHARGE_TEMPERATURE_1, (int)&rak_heatpump_status.dischargeTemperature1), PAR_PERMS_READ},
    {CID_HP_RAK_DISCHARGE_TEMPERATURE_2, STR("rak_Discharge_Temperature_2"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2161, 1,
     RAK_HP_STATUS_OFFSET(dischargeTemperature2), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_DISCHARGE_TEMPERATURE_2, (int)&rak_heatpump_status.dischargeTemperature2), PAR_PERMS_READ},
    {CID_HP_RAK_SUCTION_TEMPERATURE_1, STR("rak_Suction_Temperature_1"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2114, 1,
     RAK_HP_STATUS_OFFSET(suctionTemperature1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_SUCTION_TEMPERATURE_1, (int)&rak_heatpump_status.suctionTemperature2), PAR_PERMS_READ},
    {CID_HP_RAK_SUCTION_TEMPERATURE_2, STR("rak_Suction_Temperature_2"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2162, 1,
     RAK_HP_STATUS_OFFSET(suctionTemperature2), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_SUCTION_TEMPERATURE_2, (int)&rak_heatpump_status.suctionTemperature2), PAR_PERMS_READ},      
    {CID_HP_RAK_COIL_TEMPERATURE_1, STR("rak_Coil_Temperature_1"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2117, 1,
     RAK_HP_STATUS_OFFSET(coilTemperature1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_COILTEMPERATURE_1, (int)&rak_heatpump_status.coilTemperature1), PAR_PERMS_READ},
    {CID_HP_RAK_COIL_TEMPERATURE_2, STR("rak_Coil_Temperature_2"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2165, 1,
     RAK_HP_STATUS_OFFSET(coilTemperature2), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_COILTEMPERATURE_2, (int)&rak_heatpump_status.coilTemperature2), PAR_PERMS_READ},
    {CID_HP_RAK_DEMAND_FOR_DEFROSTING, STR("rak_Demand_For_Defrosting"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2109, 1,
     RAK_HP_STATUS_OFFSET(demandForDefrosting), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA, HP_RAK_WORK_BIT_NR_DEMAND_FOR_DEFROSTING, (int)&rak_heatpump_status.demandForDefrosting), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_HIGHT_PRESSURE_1, STR("rak_High_Pressure_1"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2124, 1,
     RAK_HP_STATUS_OFFSET(highPressure1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_COMPRESSOR_MNUFACTURER_DATA, HP_RAK_WORK_BIT_NR_HIGH_PRESSURE_1, (int)&rak_heatpump_status.highPressure1), PAR_PERMS_READ},
    {CID_HP_RAK_HIGHT_PRESSURE_2, STR("rak_High_Pressure_2"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2172, 1,
     RAK_HP_STATUS_OFFSET(highPressure2), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_COMPRESSOR_MNUFACTURER_DATA, HP_RAK_WORK_BIT_NR_HIGH_PRESSURE_2, (int)&rak_heatpump_status.highPressure2), PAR_PERMS_READ},
    {CID_HP_RAK_LOW_PRESSURE_1, STR("rak_Low_Pressure_1"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2125, 1,
     RAK_HP_STATUS_OFFSET(lowPressure1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_COMPRESSOR_MNUFACTURER_DATA, HP_RAK_WORK_BIT_NR_LOW_PRESSURE_1, (int)&rak_heatpump_status.lowPressure1), PAR_PERMS_READ},
    {CID_HP_RAK_LOW_PRESSURE_2, STR("rak_Low_Pressure_2"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2173, 1,
     RAK_HP_STATUS_OFFSET(lowPressure2), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_COMPRESSOR_MNUFACTURER_DATA, HP_RAK_WORK_BIT_NR_LOW_PRESSURE_2, (int)&rak_heatpump_status.lowPressure2), PAR_PERMS_READ},
    {CID_HP_RAK_COMPRESSOR_TARGET_FREQ_1, STR("rak_Compressor_Target_Frequency_1"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2128, 1,
     RAK_HP_STATUS_OFFSET(compressorTargetFrequency1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_COMPRESSOR_MNUFACTURER_DATA, HP_RAK_WORK_BIT_NR_COMPRESSOR_TARGET_FREQUENCY_1, (int)&rak_heatpump_status.compressorTargetFrequency1), PAR_PERMS_READ},
    {CID_HP_RAK_COMPRESSOR_TARGET_FREQ_2, STR("rak_Compressor_Target_Frequency_2"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2176, 1,
     RAK_HP_STATUS_OFFSET(compressorTargetFrequency2), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_COMPRESSOR_MNUFACTURER_DATA, HP_RAK_WORK_BIT_NR_COMPRESSOR_TARGET_FREQUENCY_2, (int)&rak_heatpump_status.compressorTargetFrequency2), PAR_PERMS_READ},
    {CID_HP_RAK_COMPRESSOR_FREQ_1, STR("rak_Compressor_Frequency_1"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2129, 1,
     RAK_HP_STATUS_OFFSET(compressorFrequency1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_COMPRESSOR_MNUFACTURER_DATA, HP_RAK_WORK_BIT_NR_COMPRESSOR_FREQUENCY_1, (int)&rak_heatpump_status.compressorFrequency1), PAR_PERMS_READ},
    {CID_HP_RAK_COMPRESSOR_FREQ_2, STR("rak_Compressor_Frequency_2"), STR("/"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2177, 1,
     RAK_HP_STATUS_OFFSET(compressorFrequency1), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_COMPRESSOR_MNUFACTURER_DATA, HP_RAK_WORK_BIT_NR_COMPRESSOR_FREQUENCY_2, (int)&rak_heatpump_status.compressorFrequency2), PAR_PERMS_READ},           

    // Heatpump compressor configuration 0 Data
    {CID_HP_RAK_MIN_FREQUENCY, STR("rak_MinFrequency"), STR("Hz"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2006, 1,
     RAK_HP_CONTROL_OFFSET(minFrequency), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_COMPRESSOR_CONFIGURATION_DATA_0, HP_RAK_WORK_BIT_NR_MIN_FREQUENCY, (int)&rak_heatpump_control.minFrequency), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_MAX_FREQUENCY, STR("rak_MaxFrequency"), STR("Hz"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2007, 1,
     RAK_HP_CONTROL_OFFSET(maxFrequency), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_COMPRESSOR_CONFIGURATION_DATA_0, HP_RAK_WORK_BIT_NR_MAX_FREQUENCY, (int)&rak_heatpump_control.maxFrequency), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_MAX_WATER_TEMP_TARGET, STR("rak_MaxWater_Temperature_Target"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3070, 1,
     RAK_HP_CONTROL_OFFSET(rakMaxWaterTempTarget), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_GENERAL_CONFIGURATION, HP_RAK_WORK_BIT_NR_MAX_WATER_TEMP_TARGET, (int)&rak_heatpump_control.rakMaxWaterTempTarget), PAR_PERMS_READ},
    {CID_HP_RAK_RESTART_DELTA_T, STR("rak_Restart_Delta_T"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2019, 1,
     RAK_HP_CONTROL_OFFSET(restartDeltaT), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_TEMPERATURE_CONFIGURATION_DATA, HP_RAK_WORK_BIT_NR_RESTART_DELTA_T, (int)&rak_heatpump_control.restartDeltaT), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_STOP_DELTA_T, STR("rak_Stop_Delta_T"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2020, 1,
     RAK_HP_CONTROL_OFFSET(stopDeltaT), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_TEMPERATURE_CONFIGURATION_DATA, HP_RAK_WORK_BIT_NR_STOP_DELTA_T, (int)&rak_heatpump_control.stopDeltaT), PAR_PERMS_READ_WRITE},
     // Heatpump defrosting configuration data
    {CID_HP_RAK_DEFROSTING_INTERVAL, STR("rak_Defrosting_Interval"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3000, 1,
     RAK_HP_CONTROL_OFFSET(defrostingInterval), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_DEFROSTING_CONFIGURATION_DATA, HP_RAK_WORK_BIT_DEFROSTING_INTERVAL, (int)&rak_heatpump_control.defrostingInterval), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_DEFROSTING_ON, STR("rak_Defrosting_On"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2003, 1,
     RAK_HP_CONTROL_OFFSET(defrostingOn), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_DEFROSTING_CONFIGURATION_DATA, HP_RAK_WORK_BIT_NR_DEFROSTING_ON, (int)&rak_heatpump_control.defrostingOn), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_AMBIENT_TEMPERATURE_START_DEFROSTING, STR("rak_Ambient_Temperature_Start_Desfrosting"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3001, 1,
     RAK_HP_CONTROL_OFFSET(ambientTemperatureStartDesfrosting), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_DEFROSTING_CONFIGURATION_DATA, HP_RAK_WORK_BIT_NR_START_DEFROST, (int)&rak_heatpump_control.ambientTemperatureStartDesfrosting), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_AMBIENT_TEMPERATURE_STOP_DEFROSTING, STR("rak_Ambient_Temperature_Stop_Desfrosting"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3002, 1,
     RAK_HP_CONTROL_OFFSET(ambientTemperatureStopDesfrosting), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_DEFROSTING_CONFIGURATION_DATA, HP_RAK_WORK_BIT_NR_STOP_DEFROST, (int)&rak_heatpump_control.ambientTemperatureStopDesfrosting), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_INLET_TEMPERATURE_FOR_DEFROST, STR("rak_Inlet_Temperature_For_Defrost"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 2999, 1,
     RAK_HP_CONTROL_OFFSET(inletTemperatureForDefrost), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_DEFROSTING_CONFIGURATION_DATA, HP_RAK_WORK_BIT_NR_INLET_TEMPERATURE_FOR_DEFROST, (int)&rak_heatpump_control.inletTemperatureForDefrost), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_OUTLET_TEMPERATURE_FOR_DEFROST, STR("rak_Outlet_Temperature_For_Defrost"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3003, 1,
     RAK_HP_CONTROL_OFFSET(outletTemperatureForDefrost), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_DEFROSTING_CONFIGURATION_DATA, HP_RAK_WORK_BIT_NR_OTLET_TEMPERATURE_FOR_DEFROST, (int)&rak_heatpump_control.outletTemperatureForDefrost), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_COIL_TEMPERATURE_FOR_DEFROST, STR("rak_Coil_Temperature_For_Defrost"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3004, 1,
     RAK_HP_CONTROL_OFFSET(coilTemperatureForDefrost), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_DEFROSTING_CONFIGURATION_DATA, HP_RAK_WORK_BIT_NR_COIL_TEMPERATURE_FOR_DEFROST, (int)&rak_heatpump_control.coilTemperatureForDefrost), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_FREQUENCY_FOR_DEFROST, STR("rak_Frequency_For_Defrost"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3005, 1,
     RAK_HP_CONTROL_OFFSET(frequencyForDefrost), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_DEFROSTING_CONFIGURATION_DATA, HP_RAK_WORK_BIT_NR_FREQUENCY_FOR_DEFROST, (int)&rak_heatpump_control.frequencyForDefrost), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_EVV_OPENING_FOR_DEFROST, STR("rak_Eev_Opening_For_Defrost"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3006, 1,
     RAK_HP_CONTROL_OFFSET(eevOpeningForDefrost), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_DEFROSTING_CONFIGURATION_DATA, HP_RAK_WORK_BIT_NR_EVV_OPENING_FOR_DEFROST, (int)&rak_heatpump_control.eevOpeningForDefrost), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_TEMPERATURE_DIF_BETWEEN_IN_OUT, STR("rak_Temperature_Difference_Between_In_And_Out"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3035, 1,
     RAK_HP_CONTROL_OFFSET(temperatureDifferenceBetweenWaterInAndOut), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_TEMPERATURE_CONFIGURATION_DATA, HP_RAK_WORK_BIT_NR_DIFFERENCE_BETWEEN_WATER_IN_AND_OUT, (int)&rak_heatpump_control.temperatureDifferenceBetweenWaterInAndOut), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_HIGHEST_COMPRESSOR_FREQUENCY, STR("rak_Highest_Compressor_Frequency"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3007, 1,
     RAK_HP_CONTROL_OFFSET(highestCompressorFrequency), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_COMPRESSOR_CONFIGURATION_DATA_0, HP_RAK_WORK_BIT_NR_HIGHEST_COMPRESSOR_FREQUENCY, (int)&rak_heatpump_control.highestCompressorFrequency), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_PROTECTION_VALUE_TOO_HIGH, STR("rak_Protection_Value_Too_High"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3036, 1,
     RAK_HP_CONTROL_OFFSET(protectionValueHpTooHigh), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_COMPRESSOR_CONFIGURATION_DATA_0, HP_RAK_WORK_BIT_NR_PROTECTION_VALUE_HP_TOO_HIGH, (int)&rak_heatpump_control.protectionValueHpTooHigh), PAR_PERMS_READ_WRITE},
    {CID_HP_RAK_PROTECTION_VALUE_TOO_LOW, STR("rak_Protection_Value_Too_Low"), STR("C"), MB_HEATPUMP_RAK, MB_PARAM_HOLDING, 3037, 1,
     RAK_HP_CONTROL_OFFSET(protectionValueLpTooLow), PARAM_TYPE_U16, 2, OPTS(KEY_ID_HP_RAK_COMPRESSOR_CONFIGURATION_DATA_0, HP_RAK_WORK_BIT_NR_PROTECTION_VALUE_HP_TOO_LOW, (int)&rak_heatpump_control.protectionValueLpTooLow), PAR_PERMS_READ_WRITE},
         
