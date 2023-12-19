#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/gpio.h"
#include "appconfig.h"
#include "board_leds.h"
#include "led_indicator.h"

static led_indicator_handle_t led_handle_power = NULL;
static led_indicator_handle_t led_handle_wifi = NULL;
static led_indicator_handle_t led_handle_modbus = NULL;
static led_indicator_handle_t led_handle_4g = NULL;

void initialize_board_leds(void)
{
    led_indicator_config_t config_power = {
        .off_level = 0,
        .mode = LED_GPIO_MODE,
    };
    led_handle_power = led_indicator_create(LED_POWER, &config_power);
    led_handle_wifi = led_indicator_create(LED_WIFI, &config_power);
    led_handle_modbus = led_indicator_create(LED_MODBUS, &config_power);
    led_handle_4g = led_indicator_create(LED_4G, &config_power);
}

static int _power_state = -1;
static int _wifi_state = -1;
static int _modbus_state = -1;
static int _4g_state = -1;

static void set_device_state_invalid(LedDevice device)
{
    switch (device)
    {
    case Power:
        _power_state = -1;
        break;
    case Wifi:
        _wifi_state = -1;
        break;
    case Modbus:
        _modbus_state = -1;
        break;
    case LTE4G:
        _4g_state = -1;
        break;
    default:
        break;
    }
}

void led_power_up_test(void)
{
    set_led_behaviour(Power, On);
    set_led_behaviour(Wifi, On);
    set_led_behaviour(Modbus, On);
    set_led_behaviour(LTE4G, On);

    vTaskDelay(pdMS_TO_TICKS(LED_DELAY_MS));

    // set_led_behaviour(Power, Off);
    set_led_behaviour(Wifi, Off);
    set_led_behaviour(Modbus, Off);
    set_led_behaviour(LTE4G, Off);
}

static void set_led_behaviour_internal(LedDevice device, led_indicator_handle_t handle, LedBehaviour state)
{
    switch (state)
    {
    case On:
        set_device_state_invalid(device);
        gpio_set_level(device, 1);
        break;
    case Off:
        set_device_state_invalid(device);
        gpio_set_level(device, 0);
        break;
    case SlowBlinking:
        led_indicator_start(handle, BLINK_CONNECTED);
        break;
    case StopSlowBlinking:
        led_indicator_stop(handle, BLINK_CONNECTED);
        break;
    case FastBlinking:
        led_indicator_start(handle, BLINK_FACTORY_RESET);
        break;
    case StopFastBlinking:
        led_indicator_stop(handle, BLINK_FACTORY_RESET);
        break;
    default:
        break;
    }
}

void set_led_behaviour(LedDevice led_device, LedBehaviour led_state)
{
    switch (led_device)
    {
    case Power:
        if (_power_state >= 0)
            led_indicator_stop(led_handle_power, _power_state);

        set_led_behaviour_internal(led_device, led_handle_power, led_state);
        break;
    case Wifi:
        if (_wifi_state >= 0)
            led_indicator_stop(led_handle_wifi, _wifi_state);

        set_led_behaviour_internal(led_device, led_handle_wifi, led_state);
        break;
    case Modbus:
        if (_modbus_state >= 0)
            led_indicator_stop(led_handle_modbus, _modbus_state);

        set_led_behaviour_internal(led_device, led_handle_modbus, led_state);
        break;
    case LTE4G:
        if (_4g_state >= 0)
            led_indicator_stop(led_handle_4g, _4g_state);

        set_led_behaviour_internal(led_device, led_handle_4g, led_state);
        break;
    default:
        break;
    }
}