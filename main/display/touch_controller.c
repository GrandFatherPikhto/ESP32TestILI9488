#include "touch_controller.h"
#include "esp_log.h"

static const char *TAG = "TOUCH";

static void lvgl_touch_cb(lv_indev_t *indev, lv_indev_data_t *data) {
    esp_lcd_touch_handle_t touch_pad = lv_indev_get_user_data(indev);
    uint16_t touchpad_x[1] = {0};
    uint16_t touchpad_y[1] = {0};
    uint8_t touchpad_cnt = 0;

    esp_lcd_touch_read_data(touch_pad);
    bool touchpad_pressed = esp_lcd_touch_get_coordinates(touch_pad, touchpad_x, touchpad_y, 
                                                         NULL, &touchpad_cnt, 1);

    if (touchpad_pressed && touchpad_cnt > 0) {
        data->point.x = touchpad_x[0];
        data->point.y = touchpad_y[0];
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

esp_err_t touch_controller_init(touch_handle_t *touch) {
#if CONFIG_EXAMPLE_LCD_TOUCH_ENABLED
    ESP_LOGI(TAG, "Initializing touch controller");

    esp_lcd_panel_io_spi_config_t tp_io_config =
    #ifdef CONFIG_EXAMPLE_LCD_TOUCH_CONTROLLER_STMPE610
        ESP_LCD_TOUCH_IO_SPI_STMPE610_CONFIG(touch->pin_num_cs);
    #elif CONFIG_EXAMPLE_LCD_TOUCH_CONTROLLER_XPT2046
        ESP_LCD_TOUCH_IO_SPI_XPT2046_CONFIG(touch->pin_num_cs);
    #endif

    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI2_HOST, 
                                           &tp_io_config, &touch->io_handle));

    esp_lcd_touch_config_t tp_cfg = {
        .x_max = touch->h_res,
        .y_max = touch->v_res,
        .rst_gpio_num = -1,
        .int_gpio_num = -1,
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = CONFIG_EXAMPLE_LCD_MIRROR_Y,
        },
    };

#if CONFIG_EXAMPLE_LCD_TOUCH_CONTROLLER_STMPE610
    ESP_ERROR_CHECK(esp_lcd_touch_new_spi_stmpe610(touch->io_handle, &tp_cfg, &touch->touch_handle));
#elif CONFIG_EXAMPLE_LCD_TOUCH_CONTROLLER_XPT2046
    ESP_ERROR_CHECK(esp_lcd_touch_new_spi_xpt2046(touch->io_handle, &tp_cfg, &touch->touch_handle));
#endif

    return ESP_OK;
#else
    return ESP_ERR_NOT_SUPPORTED;
#endif
}

lv_indev_t* touch_controller_init_lvgl_input(touch_handle_t *touch, lv_display_t *display) {
#if CONFIG_EXAMPLE_LCD_TOUCH_ENABLED
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_display(indev, display);
    lv_indev_set_user_data(indev, touch->touch_handle);
    lv_indev_set_read_cb(indev, lvgl_touch_cb);
    return indev;
#else
    return NULL;
#endif
}
