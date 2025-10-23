#include "lvgl_port.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_log.h"
#include <sys/lock.h>

static const char *TAG = "LVGL_PORT";
static _lock_t lvgl_api_lock;
static TaskHandle_t lvgl_task_handle = NULL;

#define LVGL_TICK_PERIOD_MS    2
#define LVGL_TASK_MAX_DELAY_MS 500
#define LVGL_TASK_MIN_DELAY_MS 1000 / CONFIG_FREERTOS_HZ
#define LVGL_TASK_STACK_SIZE   (8 * 1024)
#define LVGL_TASK_PRIORITY     2

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

static void increase_lvgl_tick(void *arg) {
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

static void lvgl_port_task(void *arg) {
    ESP_LOGI(TAG, "Starting LVGL task");
    uint32_t time_till_next_ms = 0;
    
    while (1) {
        lvgl_port_lock();
        time_till_next_ms = lv_timer_handler();
        lvgl_port_unlock();
        
        time_till_next_ms = MAX(time_till_next_ms, LVGL_TASK_MIN_DELAY_MS);
        time_till_next_ms = MIN(time_till_next_ms, LVGL_TASK_MAX_DELAY_MS);
        vTaskDelay(pdMS_TO_TICKS(time_till_next_ms));
    }
}

void lvgl_port_init(void) {
    ESP_LOGI(TAG, "Initializing LVGL library");
    lv_init();  // Это должно быть здесь
    
    // Создание таймера для LVGL tick
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &increase_lvgl_tick,
        .name = "lvgl_tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_PERIOD_MS * 1000));
}

void lvgl_port_start(void) {
    ESP_LOGI(TAG, "Creating LVGL task");
    xTaskCreate(lvgl_port_task, "LVGL", LVGL_TASK_STACK_SIZE, NULL, 
                LVGL_TASK_PRIORITY, &lvgl_task_handle);
}

void lvgl_port_lock(void) {
    _lock_acquire(&lvgl_api_lock);
}

void lvgl_port_unlock(void) {
    _lock_release(&lvgl_api_lock);
}
