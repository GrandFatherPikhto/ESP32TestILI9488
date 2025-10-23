#ifndef TOUCH_CONTROLLER_H
#define TOUCH_CONTROLLER_H

#include "esp_lcd_touch.h"
#include "lvgl.h"

#if CONFIG_EXAMPLE_LCD_TOUCH_CONTROLLER_STMPE610
#include "esp_lcd_touch_stmpe610.h"
#elif CONFIG_EXAMPLE_LCD_TOUCH_CONTROLLER_XPT2046
#include "esp_lcd_touch_xpt2046.h"
#endif

typedef struct {
    esp_lcd_touch_handle_t touch_handle;
    esp_lcd_panel_io_handle_t io_handle;
    int pin_num_cs;
    int h_res;
    int v_res;
} touch_handle_t;

esp_err_t touch_controller_init(touch_handle_t *touch);
lv_indev_t* touch_controller_init_lvgl_input(touch_handle_t *touch, lv_display_t *display);

#endif /* TOUCH_CONTROLLER_H */
