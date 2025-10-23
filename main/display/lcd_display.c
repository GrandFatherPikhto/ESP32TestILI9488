#include "lcd_display.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "config.h"

static const char *TAG = "LCD";

#if CONFIG_EXAMPLE_LCD_CONTROLLER_ILI9341
#include "esp_lcd_ili9341.h"
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_GC9A01
#include "esp_lcd_gc9a01.h"
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_ILI9488
#include "esp_lcd_ili9488.h"
#endif


// Конфигурация по умолчанию
static const lcd_config_t default_lcd_config = {
    .host = SPI2_HOST,
    .pixel_clock_hz = 20 * 1000 * 1000,
    .pin_num_sclk = 18,
    .pin_num_mosi = 19,
    .pin_num_miso = 21,
    .pin_num_dc = 5,
    .pin_num_rst = 3,
    .pin_num_cs = 4,
    .pin_num_bk_light = 2,
    .h_res = LCD_H_RES,
    .v_res = LCD_V_RES,
    .cmd_bits = 8,
    .param_bits = 8
};

static bool notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, 
                                   esp_lcd_panel_io_event_data_t *edata, 
                                   void *user_ctx) {
    lv_display_t *disp = (lv_display_t *)user_ctx;
    lv_display_flush_ready(disp);
    return false;
}

static void lvgl_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    esp_lcd_panel_handle_t panel_handle = lv_display_get_user_data(disp);
    esp_lcd_panel_draw_bitmap(panel_handle, area->x1, area->y1, 
                             area->x2 + 1, area->y2 + 1, px_map);
}

esp_err_t lcd_display_init(lcd_handle_t *lcd) {
    if (lcd == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    lcd->config = default_lcd_config;
    
    ESP_LOGI(TAG, "Initializing LCD display");

    // Инициализация backlight GPIO
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << lcd->config.pin_num_bk_light
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    lcd_display_set_backlight(false);

    // Инициализация SPI bus
    spi_bus_config_t buscfg = {
        .sclk_io_num = lcd->config.pin_num_sclk,
        .mosi_io_num = lcd->config.pin_num_mosi,
        .miso_io_num = lcd->config.pin_num_miso,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = lcd->config.h_res * 80 * sizeof(uint16_t),
    };
    ESP_ERROR_CHECK(spi_bus_initialize(lcd->config.host, &buscfg, SPI_DMA_CH_AUTO));

    // Установка panel IO
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = lcd->config.pin_num_dc,
        .cs_gpio_num = lcd->config.pin_num_cs,
        .pclk_hz = lcd->config.pixel_clock_hz,
        .lcd_cmd_bits = lcd->config.cmd_bits,
        .lcd_param_bits = lcd->config.param_bits,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)lcd->config.host, 
                                           &io_config, &lcd->io_handle));

    // Инициализация panel
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = lcd->config.pin_num_rst,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
        .bits_per_pixel = LCD_BITS_PER_PIXEL,
    };

#if CONFIG_EXAMPLE_LCD_CONTROLLER_ILI9341
    ESP_ERROR_CHECK(esp_lcd_new_panel_ili9341(lcd->io_handle, &panel_config, &lcd->panel_handle));
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_GC9A01
    ESP_ERROR_CHECK(esp_lcd_new_panel_gc9a01(lcd->io_handle, &panel_config, &lcd->panel_handle));
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_ILI9488
    panel_config.bits_per_pixel = 24;
    ESP_ERROR_CHECK(esp_lcd_new_panel_ili9488(lcd->io_handle, &panel_config, 
                                            lcd->config.h_res * 20 * sizeof(uint16_t), 
                                            &lcd->panel_handle));
#endif

    ESP_ERROR_CHECK(esp_lcd_panel_reset(lcd->panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(lcd->panel_handle));
    
#if CONFIG_EXAMPLE_LCD_CONTROLLER_GC9A01
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(lcd->panel_handle, true));
#endif

    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(lcd->panel_handle, true));

    return ESP_OK;
}

void lcd_display_set_backlight(bool on) {
    gpio_set_level(default_lcd_config.pin_num_bk_light, on ? 1 : 0);
}

lv_display_t* lcd_display_init_lvgl(lcd_handle_t *lcd) {
    // Создание LVGL display
    lcd->lvgl_display = lv_display_create(lcd->config.h_res, lcd->config.v_res);
    
    // Выделение буферов
    size_t draw_buffer_sz = lcd->config.h_res * 20 * sizeof(lv_color16_t);
    void *buf1 = spi_bus_dma_memory_alloc(lcd->config.host, draw_buffer_sz, 0);
    void *buf2 = spi_bus_dma_memory_alloc(lcd->config.host, draw_buffer_sz, 0);
    
    lv_display_set_buffers(lcd->lvgl_display, buf1, buf2, draw_buffer_sz, 
                          LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_user_data(lcd->lvgl_display, lcd->panel_handle);
    lv_display_set_color_format(lcd->lvgl_display, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(lcd->lvgl_display, lvgl_flush_cb);

    // Регистрация callback для уведомления о готовности сброса
    const esp_lcd_panel_io_callbacks_t cbs = {
        .on_color_trans_done = notify_lvgl_flush_ready,
    };
    ESP_ERROR_CHECK(esp_lcd_panel_io_register_event_callbacks(lcd->io_handle, 
                                                             &cbs, lcd->lvgl_display));

    return lcd->lvgl_display;
}

lv_display_t* lcd_display_get_lvgl_handle(lcd_handle_t *lcd) {
    return lcd->lvgl_display;
}
