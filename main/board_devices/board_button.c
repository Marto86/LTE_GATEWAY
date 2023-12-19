#include "driver/gpio.h"
#include "esp_log.h"
#include "board_button.h"
#include "sntp.h"
#include "board_leds.h"

void IRAM_ATTR gpio_isr_handler(void* arg)
{
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    int64_t time_us = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;

    //ESP_LOGI("BOARD_BUTTON", "Btn interrupt handler");
    //board_btn_info.is_down = !board_btn_info.is_down;
    int level = gpio_get_level(GPIO_NUM_0);
    if(level > 0) {
        set_led_behaviour(Modbus, Off);
        board_btn_info.is_pressed = false;

        board_btn_info.release_time_us = time_us;
    }
    else {
        set_led_behaviour(Modbus, On);
        board_btn_info.is_pressed = true;

        board_btn_info.press_time_us = time_us;
        board_btn_info.was_btn_clicked_flag = true;
    }
}

void initialize_board_button(void)
{
    // button_handle_t dev_on_off_btn = iot_button_create(BUTTON_ON_OFF, BUTTON_ACTIVE_LEVEL);
    // iot_button_set_evt_cb(dev_on_off_btn, BUTTON_CB_TAP, button_tap_cb, &dev_on_btn_num);
        //zero-initialize the config structure.
    gpio_config_t io_conf = {};
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    //set as output mode
    io_conf.mode = GPIO_MODE_INPUT;
    //bit mask of the pins that you want to set,e.g.GPIO0
    io_conf.pin_bit_mask = (1ULL<<GPIO_NUM_0);
    //disable pull-down mode
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    //disable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    // //install gpio isr service
    gpio_install_isr_service(0/*ESP_INTR_FLAG_DEFAULT*/);
    // //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_NUM_0, gpio_isr_handler, NULL);
}