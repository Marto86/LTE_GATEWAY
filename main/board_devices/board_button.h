#ifndef __BOARD_BUTTON_H__
#define __BOARD_BUTTON_H__

#include "driver/gpio.h"

struct board_btn_info
{
    bool was_btn_clicked_flag;
    bool was_btn_hold;
    bool is_pressed;
    int64_t press_time_us;
    int64_t release_time_us;
} board_btn_info;

void IRAM_ATTR gpio_isr_handler(void *arg);

void initialize_board_button(void);

#endif // __BOARD_BUTTON_H__