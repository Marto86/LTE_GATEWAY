#ifndef __BOARD_LEDS_H__
#define __BOARD_LEDS_H__

#define LED_DELAY_MS (3000)
#include "driver/gpio.h"
#include "appconfig.h"

typedef enum LedDevice
{
    Power = LED_POWER,
    Wifi = LED_WIFI,
    Modbus = LED_MODBUS,
    LTE4G = LED_4G
} LedDevice;

typedef enum LedBehaviour
{
    On,
    Off,
    SlowBlinking,
    StopSlowBlinking,
    FastBlinking,
    StopFastBlinking,
} LedBehaviour;

void initialize_board_leds(void);

void led_power_up_test(void);

void led_error_behaviour(LedDevice led_device);
void set_led_behaviour(LedDevice led_device, LedBehaviour led_state);

#endif // __BOARD_LEDS_H__