#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "driver/spi_master.h"
#include "lvgl.h"

// Конфигурация LCD
typedef struct {
    int host;
    int pixel_clock_hz;
    int pin_num_sclk;
    int pin_num_mosi;
    int pin_num_miso;
    int pin_num_dc;
    int pin_num_rst;
    int pin_num_cs;
    int pin_num_bk_light;
    int h_res;
    int v_res;
    int cmd_bits;
    int param_bits;
} lcd_config_t;

// Структура для хранения handles LCD
typedef struct {
    esp_lcd_panel_io_handle_t io_handle;
    esp_lcd_panel_handle_t panel_handle;
    lv_display_t *lvgl_display;
    lcd_config_t config;
} lcd_handle_t;

// Public функции
esp_err_t lcd_display_init(lcd_handle_t *lcd);
void lcd_display_set_backlight(bool on);
lv_display_t* lcd_display_get_lvgl_handle(lcd_handle_t *lcd);
lv_display_t* lcd_display_init_lvgl(lcd_handle_t *lcd);

#endif /* LCD_DISPLAY_H */