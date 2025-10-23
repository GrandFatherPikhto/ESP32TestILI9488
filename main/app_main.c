#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "lvgl.h"

#include "lcd_display.h"
#include "touch_controller.h"
#include "lvgl_port.h"

#include "config.h"

static const char *TAG = "MAIN";

extern void example_lvgl_demo_ui(lv_disp_t *disp);

static void debug_displays(void) {
    lv_display_t *disp = lv_display_get_next(NULL);
    int count = 0;
    
    while (disp) {
        count++;
        ESP_LOGI("DEBUG", "Display %d: %dx%d, addr: %p", count,
                 lv_display_get_horizontal_resolution(disp),
                 lv_display_get_vertical_resolution(disp),
                 disp);
        disp = lv_display_get_next(disp);
    }
    
    ESP_LOGI("DEBUG", "Total displays found: %d", count);
}

void app_main(void) {
    ESP_LOGI(TAG, "Starting application");
    vTaskDelay(pdMS_TO_TICKS(1000));

    // ПЕРВОЕ: Инициализация LVGL
    lvgl_port_init();
    ESP_LOGI(TAG, "LVGL Init");
    vTaskDelay(pdMS_TO_TICKS(500));

    // ВТОРОЕ: Инициализация LCD
    lcd_handle_t lcd = {0};
    ESP_ERROR_CHECK(lcd_display_init(&lcd));
    lv_display_t *display = lcd_display_init_lvgl(&lcd);
    lcd_display_set_backlight(true);

    // Проверка количества дисплеев после инициализации LCD
    debug_displays();

    // ТРЕТЬЕ: Инициализация тачскрина
    touch_handle_t touch = {
        .pin_num_cs = 15,
        .h_res = LCD_H_RES,
        .v_res = LCD_V_RES
    };
    touch_controller_init(&touch);

    // ЧЕТВЕРТОЕ: Настройка тачскрина в LVGL и запуск задачи
    touch_controller_init_lvgl_input(&touch, display);
    lvgl_port_start();

    // ПЯТОЕ: Создание UI
    lvgl_port_lock();
    
    // Проверка перед созданием меню
    debug_displays();
    
    example_lvgl_demo_ui(display);
    
    // Проверка после создания меню
    debug_displays();
    
    lvgl_port_unlock();

    ESP_LOGI(TAG, "Application started successfully");
}
