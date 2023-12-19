#ifndef __INVERTER_02_H__
#define __INVERTER_02_H__

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
    int16_t inv02waterInTemp;
    int16_t inv02ambientInTemp;
    int16_t inv02TempOrder;
    uint16_t inv02status; // inv02isOn + inv02mode
    uint16_t inv02isDefrosting;
    uint16_t inv02CirculationMode;
    uint16_t inv02isFlowing;
    uint16_t inv02waterOutTemp;
    uint16_t inv02fanSpeed1;
    uint16_t inv02fanSpeed2;
    int16_t inv02AcVoltage;
    int16_t inv02AcCurrent;
    uint16_t inv02switches;
    uint16_t inv02eevManual1;
    uint16_t inv02dischargeTemperature1;
    uint16_t inv02suctionTemperature1; 
    uint16_t inv02coilTemperature1;
    uint16_t inv02targetFrequency;
    uint16_t inv02currentFrequency;
    uint16_t inv02eevMode1;
    uint16_t inv02eevOpening1;
} inverter_02_status_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
    uint16_t inv02Error501;
    uint16_t inv02Error502;
    uint16_t inv02Error503;
    uint16_t inv02CompresorError;
    uint16_t modbusCommunicationError;
} inverter_02_errors_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
    uint16_t inv02fanSpeedLevel0;
    uint16_t inv02fanSpeedLevel1;
    uint16_t inv02fanSpeedLevel2;

    uint16_t inv02heatingFrequeyncy0;
    uint16_t inv02heatingFrequeyncy1;
    uint16_t inv02heatingFrequeyncy2;
    uint16_t inv02heatingFrequeyncy3;
    uint16_t inv02heatingFrequeyncy4;
    uint16_t inv02heatingFrequeyncy5;
    uint16_t inv02heatingFrequeyncy6;
    uint16_t inv02heatingFrequeyncy7;
    uint16_t inv02heatingFrequeyncy8;
    uint16_t inv02heatingFrequeyncy9;
    uint16_t inv02heatingFrequeyncy10;

    uint16_t inv02coolingFrequency0;
    uint16_t inv02coolingFrequency1;
    uint16_t inv02coolingFrequency2;
    uint16_t inv02coolingFrequency3;
    uint16_t inv02coolingFrequency4;
    uint16_t inv02coolingFrequency5;
    uint16_t inv02coolingFrequency6;
    uint16_t inv02coolingFrequency7;
    uint16_t inv02coolingFrequency8;
    uint16_t inv02coolingFrequency9;
    uint16_t inv02coolingFrequency10;
    uint16_t inv02manFanSpeed1;
    uint16_t inv02manFanSpeed2;
    uint16_t inv02MaxWateTempTarget;
    uint16_t inv02fanMotorModel;
    uint16_t inv02compressorModel;
    uint16_t inv02defrostingInterval;
    uint16_t inv02ambientTemperatureStartDefrost;
    uint16_t inv02ambientTemperatureStopDefrost;
    uint16_t inv02rStartDeltaT;
    uint16_t inv02rStopDeltaT;
    uint16_t inv02compensationTemperature;
    uint16_t inv02minimumAmbientTemperature;
    uint16_t inv02ambientTemperatureDeviation;
    uint16_t inv02temperatureDiffInnOut;
    uint16_t inv02ambTempToStartBottomPlateHeater;
    
    } inverter_02_control_t;
#pragma pack(pop)

typedef enum
{
    KEY_ID_HP_INV02_REPORT = 0,
    KEY_ID_HP_INV02_WORK_DATA = 0x01,
    KEY_ID_HP_INV02_ERRORS = 0x02,
    KEY_ID_HP_INV02_MANUFACTURER_DATA = 0x03,
    KEY_ID_HP_INV02_GENERAL_CONFIGURATION = 0x0A,
    KEY_ID_HP_INV02_CONFIGURATION_FAN = 0x0B,
    KEY_ID_HP_INV02_COMPRESSOR_CONFIGURATION_DATA_0 = 0x0C,
    KEY_ID_HP_INV02_COMPRESSOR_CONFIGURATION_DATA_1 = 0x0D,
    KEY_ID_HP_INV02_MANUAL_MANUFACTURER_DATA = 0x10,
    KEY_ID_HP_INV02_DEFROSTING_CONFIGURATION_DATA = 0x20,
    KEY_ID_HP_INV02_TEMPERATURE_CONFIGURATION_DATA = 0x21,
    KEY_ID_HP_INV02_RAW_DATA_ = 0x80,
    KEY_ID_HP_INV02_PH_RAW_DATA_ = 0x81,
    KEY_ID_HP_INV02_REDOX_RAW_DATA_ = 0x82,
    KEY_ID_HP_INV02_SALT_RAW_DATA_ = 0x83,
    KEY_ID_HP_INV02_CONFIGURATION_SET,
    KEY_ID_HP_INV_TEST_COMMUNICATION,

} inverter_02_key_id_t;

enum
{
    HP_INV02_WORK_BIT_NR_WATER_IN_TEMPERATURE = 0,
    HP_INV02_WORK_BIT_NR_AMBIENT_IN_TEMPERATURE,
    HP_INV02_WORK_BIT_NR_TEMPERATURE_ORDER,
    HP_INV02_WORK_BIT_NR_IS_ON = 3,
    HP_INV02_WORK_BIT_NR_MODE = 4,
    HP_INV02_WORK_BIT_NR_IS_DEFROSTING = 5,
    HP_INV02_WORK_BIT_NR_SWTCHES = 6,
    HP_INV02_WORK_BIT_NR_RUNING_MODE = 14,
};

enum
{

    HP_INV02_ERRORS_BIT_NR_ERROR501 = 8,
    HP_INV02_ERRORS_BIT_NR_ERROR502,
    HP_INV02_ERRORS_BIT_NR_ERROR503,
    HP_INV02_ERRORS_BIT_NR_COMPRESSOR_ERROR,
    HP_MODBUS_COMMUNICATION_ERROR = 15,
};
enum
{
    HP_INV02_WORK_BIT_NR_WATER_OUT_TEMPERATURE = 0, // manufacturer data
    HP_INV02_WORK_BIT_NR_FAN_SPEED_1,
    HP_INV02_WORK_BIT_NR_FAN_SPEED_2,
    HP_INV02_WORK_BIT_NR_AC_VOLTAGE,
    HP_INV02_WORK_BIT_NR_AC_CURRENT,
    HP_INV02_WORK_BIT_NR_DICHARGE_TEMPERATURE = 6 ,
    HP_INV02_WORK_BIT_NR_SUCTION_TEMPERATURE_1,
    HP_INV02_WORK_BIT_NR_COIL_TEMPERATURE_1,
    HP_INV02_WORK_BIT_NR_TARGET_FREQUENCY = 12,
    HP_INV02_WORK_BIT_NR_CURRENT_FREQUENCY,
    HP_INV02_WORK_BIT_NR_EEV_OPENNING_1 = 14,
    HP_INV02_WORK_BIT_NR_EEV_OPENNING_2,
};

enum                                          //Manual manufacturer data
{
    HP_INV02_WORK_BIT_NR_EEV_MANUAL_1 = 0,
    HP_INV02_WORK_BIT_NR_EEV_MODE_1 = 6,
};

enum                                        //Heatpump general configuration data
{
   HP_INV02_WORK_BIT_NR_MAXIMUM_WATER_TEMP_TARGET,
   HP_INV02_WORK_BIT_NR_FAN_MOTOR_MODEL,
   HP_INV02_WORK_BIT_NR_COMPRESSOR_MODEL
};

enum                                              //Heatpump defrosting configurationdata
{ 
   HP_INV02_WORK_BIT_DEFROSTING_INTERVAL,
   HP_INV02_WORK_BIT_NR_START_DEFROST = 7,
   HP_INV02_WORK_BIT_NR_STOP_DEFROST = 8,
};

enum                                                // Heatpump temperature configurationdata
{
   HP_INV02_WORK_BIT_NR_RESTART_DELTA_T = 0, 
   HP_INV02_WORK_BIT_NR_STOP_DELTA_T,
   HP_INV02_WORK_BIT_NR_COMPENSATION_TEMPERATURE,
   HP_INV02_WORK_BIT_NR_MINIMUM_AMBIENT_TEMPERATURE,
   HP_INV02_WORK_BIT_NR_AMBIENT_TEMPERATURE_DEVIATION,
   HP_INV02_WORK_BIT_NR_TEMPERATURE_DIFF_IN_OUT,
   HP_INV02_WORK_BIT_NR_AMBIENT_TEMPERATURE_START_BOTOM_HEATER,
};

typedef struct
{
    // This is the LSB
    uint16_t WaterInTemperature : 1;
    uint16_t AmbientInTemperature : 1;
    uint16_t TemperatureOrder : 1;
    uint16_t IsOn : 1;
    uint16_t Mode : 1;
    uint16_t IsDefrosting : 1;
    uint16_t IsFlowing : 1;
    uint16_t IsWaterPumpEnabled : 1;
    uint16_t Error501 : 1;
    uint16_t Error502 : 1;
    uint16_t Error503 : 1;
    uint16_t CompressorError : 1;

} inverter_02_work_data_bits_t;

typedef union
{
    inverter_02_work_data_bits_t bit;
    uint16_t value;
} inverter_02_work_data_t;

typedef struct
{
    uint16_t Error501 : 1;
    uint16_t Error502 : 1;
    uint16_t Error503 : 1;
    uint16_t compressorError : 1;
} inverter_02_error_bits_t;

// Enumeration of modbus device addresses accessed by master device
enum
{
    MB_HEATPUMP_INV02 = 17 // Only one slave device used for the test (add other slave addresses here)
};

#define CID_HP_INV02_OFFSET CID_INV02_WATER_IN_TEMPERATURE
#define CID_HP_INV02_MAX_NUMBER CID_INV02_CURRENT_FREQUENCY

// The macro to get offset for parameter in the appropriate structure
#define INVERTER_02_STATUS_OFFSET(field) ((uint16_t)(offsetof(inverter_02_status_t, field) + 1))
#define INVERTER_02_STATUS_PTR(param) ((void *)(&inverter_02_status + param->param_offset - 1))

#define INVERTER_02_ERROR_OFFSET(field) ((uint16_t)(offsetof(inverter_02_errors_t, field) + 1))
#define INVERTER_02_ERROR_PTR(param) ((void *)(&inverter_02_errors + param->param_offset - 1))

#define INVERTER_02_CONTROL_OFFSET(field) ((uint16_t)(offsetof(inverter_02_control_t, field) + 1))
#define INVERTER_02_CONTROL_PTR(param) ((void *)(&inverter_02_control + param->param_offset - 1))

extern inverter_02_status_t inverter_02_status;
extern inverter_02_errors_t inverter_02_errors;
extern inverter_02_control_t inverter_02_control;

void inverter_02_operate(inverter_02_key_id_t key_id, void *mqtt_client, bool force_resend, ContainerTypeEnum container_type);

void inverter_02_work(time_t *current_time, time_t *previous_force_time, time_t *previous_sent_time, time_t *all_data_previous_sent_time);
#endif // !defined(__INVERTER_02_H__)
