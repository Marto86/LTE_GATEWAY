#ifndef __HEATPUMP_RAK_H__
#define __HEATPUMP_RAK_H__

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
  int16_t waterInTemp;
  int16_t ambientInTemp;
  int16_t heatingTempOrder;
  int16_t coolingTempOrder;
  uint16_t mode;
  uint16_t isDefrosting;
  uint16_t isWaterPumpEnabled;
  uint16_t circulationRunningMode;
  uint16_t isFlowing;
  uint16_t waterOutTemp;
  uint16_t fanSpeed1;
  uint16_t fanSpeed2;
  uint16_t eevOpening1;
  uint16_t eevOpening2;
  uint16_t eevMode1;
  uint16_t eevMode2;
  uint16_t fanMode1;
  uint16_t fanMode2;
  uint16_t compressorMode1;
  uint16_t compressorMode2;
  uint16_t eevManual1;
  uint16_t eevManual2;
  uint16_t fanSpeedManual1;
  uint16_t fanSpeedManual2;
  uint16_t compressorSpeedManual1;
  uint16_t compressorSpeedManual2;
  uint16_t dischargeTemperature1;
  uint16_t dischargeTemperature2;
  uint16_t suctionTemperature1;
  uint16_t suctionTemperature2;
  uint16_t coilTemperature1;
  uint16_t coilTemperature2;
  uint16_t demandForDefrosting;
  uint16_t highPressure1;
  uint16_t highPressure2;
  uint16_t lowPressure1;
  uint16_t lowPressure2;
  uint16_t compressorTargetFrequency1;
  uint16_t compressorTargetFrequency2;
  uint16_t compressorFrequency1;
  uint16_t compressorFrequency2;
} rak_heatpump_status_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
  uint16_t errors2104;
  uint16_t errors2105;
  uint16_t errors2136;
  uint16_t errors2137;
  uint16_t errors2138;
  uint16_t errors2184;
  uint16_t errors2185;
  uint16_t errors2186;
  uint16_t modbusCommunicationError;
} rak_heatpump_errors_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
  uint16_t fanSpeedLevel1;
  uint16_t fanSpeedLevel2;
  uint16_t fanSpeedLevel3;
  uint16_t fanSpeedLevel4;
  uint16_t fanSpeedLevel5;
  uint16_t fanSpeedLevel6;
  uint16_t fanSpeedLevel7;
  uint16_t fanSpeedLevel8;
  uint16_t fanSpeedLevel9;
  uint16_t minFrequency;
  uint16_t maxFrequency;
  int16_t rakMaxWaterTempTarget;
  int16_t restartDeltaT;
  int16_t stopDeltaT;
  int16_t defrostingInterval;
  uint16_t defrostingOn;
  int16_t ambientTemperatureStartDesfrosting;
  int16_t ambientTemperatureStopDesfrosting;
  int16_t inletTemperatureForDefrost;
  int16_t outletTemperatureForDefrost;
  int16_t coilTemperatureForDefrost;
  uint16_t frequencyForDefrost;
  uint16_t eevOpeningForDefrost;
  int16_t temperatureDifferenceBetweenWaterInAndOut;
  int16_t highestCompressorFrequency;
  int16_t protectionValueHpTooHigh;
  int16_t protectionValueLpTooLow;
} rak_heatpump_control_t;
#pragma pack(pop)

typedef enum
{
  KEY_ID_HP_RAK_REPORT = 0,
  KEY_ID_HP_RAK_WORK_DATA = 0x01,
  KEY_ID_HP_RAK_ERRORS = 0x02,
  KEY_ID_HP_RAK_MANUFACTURER_DATA = 0x03,
  KEY_ID_HP_RAK_GENERAL_CONFIGURATION = 0x0A,
  KEY_ID_HP_RAK_CONFIGURATION_FAN = 0x0B,
  KEY_ID_HP_RAK_COMPRESSOR_CONFIGURATION_DATA_0 = 0x0C,
  KEY_ID_HP_RAK_COMPRESSOR_CONFIGURATION_DATA_1 = 0x0D,
  KEY_ID_HP_RAK_MANUAL_MANUFACTURER_DATA = 0x10,
  KEY_ID_HP_RAK_COMPRESSOR_MNUFACTURER_DATA = 0x11,
  KEY_ID_HP_RAK_DEFROSTING_CONFIGURATION_DATA = 0x20,
  KEY_ID_HP_RAK_TEMPERATURE_CONFIGURATION_DATA = 0x21,
  KEY_ID_HP_RAK_RAW_DATA_ = 0x80,
} heatpump_rak_key_id_t;

enum  // work data 
{  
  HP_RAK_WORK_BIT_NR_WATER_IN_TEMPERATURE = 0,
  HP_RAK_WORK_BIT_NR_AMBIENT_IN_TEMPERATURE,
  HP_RAK_WORK_BIT_NR_MODE = 4,
  HP_RAK_WORK_BIT_NR_IS_DEFROSTING = 5,
  HP_RAK_WORK_BIT_NR_IS_FLOWING = 6,
  HP_RAK_WORK_BIT_NR_IS_WATER_PUMP_ENABLED = 7,
  HP_RAK_WORK_BIT_NR_HEATING_TEMPERATURE_ORDER = 12,
  HP_RAK_WORK_BIT_NR_COOLING_TEMPERATURE_ORDER = 13,
  HP_RAK_WORK_BIT_NR_RAK_CIRCULATION_RUNNING_MODE,
};

enum // errors data 
{
  ERRORS_BIT_NR_HP_RAK_ERROR2104 = 0,
  ERRORS_BIT_NR_HP_RAK_ERROR2105,
  ERRORS_BIT_NR_HP_RAK_ERROR2136,
  ERRORS_BIT_NR_HP_RAK_ERROR2137,
  ERRORS_BIT_NR_HP_RAK_ERROR2138,
  ERRORS_BIT_NR_HP_RAK_ERROR2184,
  ERRORS_BIT_NR_HP_RAK_ERROR2185,
  ERRORS_BIT_NR_HP_RAK_ERROR2186,
  ERRORS_BIT_NR_HP_RAK_MODBUS_COMMUNICATION_ERROR = 15,
};




enum // maufacturer data
{
  HP_RAK_WORK_BIT_NR_WATER_OUT_TEMPERATURE = 0,
  HP_RAK_WORK_BIT_NR_FAN_SPEED_1,
  HP_RAK_WORK_BIT_NR_FAN_SPEED_2,
  HP_RAK_WORK_BIT_NR_DISCHARGE_TEMPERATURE_1 = 6,
  HP_RAK_WORK_BIT_NR_SUCTION_TEMPERATURE_1,
  HP_RAK_WORK_BIT_NR_COILTEMPERATURE_1,
  HP_RAK_WORK_BIT_NR_DISCHARGE_TEMPERATURE_2,
  HP_RAK_WORK_BIT_NR_SUCTION_TEMPERATURE_2,
  HP_RAK_WORK_BIT_NR_COILTEMPERATURE_2,
  HP_RAK_WORK_BIT_NR_EEVOPENING_1 = 14,
  HP_RAK_WORK_BIT_NR_EEVOPENING_2,
};
enum //  Heatpump manual manufacturer data
{
  HP_RAK_WORK_BIT_NR_EEV_MANUAL_1,
  HP_RAK_WORK_BIT_NR_EEV_MANUAL_2,
  HP_RAK_WORK_BIT_NR_FAN_SPEED_MANUAL_1,
  HP_RAK_WORK_BIT_NR_FAN_SPEED_MANUAL_2,
  HP_RAK_WORK_BIT_NR_COMPRESSOR_SPEED_MANUAL_1,
  HP_RAK_WORK_BIT_NR_COMPRESSOR_SPEED_MANUAL_2,
  HP_RAK_WORK_BIT_NR_EEV_MODE_1,
  HP_RAK_WORK_BIT_NR_EEV_MODE_2,
  HP_RAK_WORK_BIT_NR_FAN_MODE_1,
  HP_RAK_WORK_BIT_NR_FAN_MODE_2,
  HP_RAK_WORK_BIT_NR_COMPRESSOR_MODE_1,
  HP_RAK_WORK_BIT_NR_COMPRESSOR_MODE_2,
  HP_RAK_WORK_BIT_NR_DEMAND_FOR_DEFROSTING, 
};
enum //  Heatpump compressor manufacturer data
{
HP_RAK_WORK_BIT_NR_HIGH_PRESSURE_1,
HP_RAK_WORK_BIT_NR_LOW_PRESSURE_1,
HP_RAK_WORK_BIT_NR_COMPRESSOR_TARGET_FREQUENCY_1,
HP_RAK_WORK_BIT_NR_COMPRESSOR_FREQUENCY_1,
HP_RAK_WORK_BIT_NR_HIGH_PRESSURE_2,
HP_RAK_WORK_BIT_NR_LOW_PRESSURE_2,
HP_RAK_WORK_BIT_NR_COMPRESSOR_TARGET_FREQUENCY_2,
HP_RAK_WORK_BIT_NR_COMPRESSOR_FREQUENCY_2,
};
enum //  Heatpump general configuration data
{
 HP_RAK_WORK_BIT_NR_MAX_WATER_TEMP_TARGET,
};

enum    //Heatpump fan configuration data
{
   HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_1 = 1,
   HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_2,
   HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_3,
   HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_4,
   HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_5,
   HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_6,
   HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_7,
   HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_8,
   HP_RAK_WORK_BIT_NR_FAN_SPEED_LVL_9,
};

enum //heatpum compressor configuration 0 data 
{
  HP_RAK_WORK_BIT_NR_MIN_FREQUENCY = 11,
  HP_RAK_WORK_BIT_NR_MAX_FREQUENCY = 12,
  HP_RAK_WORK_BIT_NR_HIGHEST_COMPRESSOR_FREQUENCY,
  HP_RAK_WORK_BIT_NR_PROTECTION_VALUE_HP_TOO_HIGH,
  HP_RAK_WORK_BIT_NR_PROTECTION_VALUE_HP_TOO_LOW,
};

enum // Heatpump defrosting configurationdata
{
  HP_RAK_WORK_BIT_DEFROSTING_INTERVAL,
  HP_RAK_WORK_BIT_NR_INLET_TEMPERATURE_FOR_DEFROST,
  HP_RAK_WORK_BIT_NR_OTLET_TEMPERATURE_FOR_DEFROST,
  HP_RAK_WORK_BIT_NR_COIL_TEMPERATURE_FOR_DEFROST,
  HP_RAK_WORK_BIT_NR_FREQUENCY_FOR_DEFROST,
  HP_RAK_WORK_BIT_NR_EVV_OPENING_FOR_DEFROST,
  HP_RAK_WORK_BIT_NR_DEFROSTING_ON,
  HP_RAK_WORK_BIT_NR_START_DEFROST = 7,
  HP_RAK_WORK_BIT_NR_STOP_DEFROST = 8,
 
};

enum // temperature configuration data
{
  HP_RAK_WORK_BIT_NR_RESTART_DELTA_T,
  HP_RAK_WORK_BIT_NR_STOP_DELTA_T,
  HP_RAK_WORK_BIT_NR_DIFFERENCE_BETWEEN_WATER_IN_AND_OUT = 5,};

enum
{
  HP_RAK_WORK_BIT_NR_TEMPERATURE_ORDER,
  HP_RAK_WORK_BIT_NR_ERROR501,
  HP_RAK_WORK_BIT_NR_ERROR502,
  HP_RAK_WORK_BIT_NR_ERROR503,
  HP_RAK_WORK_BIT_NR_COMPRESSOR_ERROR,
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
  uint16_t HeatingTemperatureOrder : 1;
  uint16_t CoolingTemperatureOrder : 1;

} heatpump_work_data_bits_t;

typedef union
{
  heatpump_work_data_bits_t bit;
  uint16_t value;
} heatpump_work_data_t;

typedef struct
{

  uint16_t Error2104 : 1;
  uint16_t Error2105 : 1;
  uint16_t Error2136 : 1;
  uint16_t Error2137 : 1;
  uint16_t Error2138 : 1;
  uint16_t Error2184 : 1;
  uint16_t Error2185 : 1;
  uint16_t Error2186 : 1;
  uint16_t Error501 : 1;
  uint16_t Error502 : 1;
  uint16_t Error503 : 1;
  uint16_t CompressorError : 1;
} heatpump_errors_t;

// Enumeration of modbus device addresses accessed by master device
enum
{
  MB_HEATPUMP_RAK = 1 // Only one slave device used for the test (add other slave addresses here)
};

#define MODBUS_CID_HP_RAK_OFFSET CID_HP_RAK_WATER_IN_TEMPERATURE
#define CID_HP_RAK_MAX_NUMBER CID_HP_RAK_PROTECTION_VALUE_TOO_LOW

// The macro to get offset for parameter in the appropriate structure
#define RAK_HP_STATUS_OFFSET(field) ((uint16_t)(offsetof(rak_heatpump_status_t, field) + 1))
#define RAK_HP_STATUS_PTR(param) ((void *)(&rak_heatpump_status + param->param_offset - 1))

#define RAK_HP_ERROR_OFFSET(field) ((uint16_t)(offsetof(rak_heatpump_errors_t, field) + 1))
#define RAK_HP_ERROR_PTR(param) ((void *)(&rak_heatpump_errors + param->param_offset - 1))

#define RAK_HP_CONTROL_OFFSET(field) ((uint16_t)(offsetof(rak_heatpump_control_t, field) + 1))
#define RAK_HP_CONTROL_PTR(param) ((void *)(&rak_heatpump_control + param->param_offset - 1))

extern rak_heatpump_status_t rak_heatpump_status;
extern rak_heatpump_errors_t rak_heatpump_errors;
extern rak_heatpump_control_t rak_heatpump_control;

void heatpump_rak_operate(heatpump_rak_key_id_t key_id, void *mqtt_client, bool force_resend, ContainerTypeEnum container_type);

void rak_01_work(time_t *current_time, time_t *previous_force_time, time_t *previous_sent_time, time_t *all_data_previous_sent_time);
#endif // !defined(__HEATPUMP_RAK_H__)
