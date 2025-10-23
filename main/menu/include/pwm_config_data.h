#ifndef PWM_CONFIG_DATA_H
#define PWM_CONFIG_DATA_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "lvgl/lvgl.h"

typedef enum {
    MODE_TEST=0x0,
    MODE_OPERATION=0x1
} mode_handle_t;

// Структура для канала PWM
typedef struct {
    bool enabled;
    uint16_t delay;
    uint16_t duration;
    uint16_t duty;  // 0-100%
} pwm_channel_t;

typedef struct pwm_menu_data {
    struct {
        lv_obj_t *main;
        lv_obj_t *start;
        lv_obj_t *settings;
        lv_obj_t *hi_channel;
        lv_obj_t *lo_channel;
    } screens;
} pwm_menu_data_t;

// Главная структура конфигурации PWM
typedef struct pwm_config {
    bool running;
    mode_handle_t mode;
    int frequency_index;  // индекс в массиве частот
    int deadtime_index;  // индекс в массиве dead time
    pwm_channel_t channel_hi;
    pwm_channel_t channel_lo;
} pwm_config_t;

typedef struct pwm_menu_handle {
    uint16_t horz_res;
    uint16_t vert_res;
    pwm_config_t *pwm_config;
    pwm_menu_data_t *menu_data;
    const uint32_t *frequency_values;
    const char **frequency_labels;
    uint8_t frequency_count;
    const uint16_t *deadtime_values;
    const char **deadtime_labels;
    uint8_t dead_time_count;
} pwm_menu_handle_t;

void config_data_init(pwm_menu_handle_t *data, pwm_config_t *pwm_config);

#endif /* PWM_CONFIG_DATA_H */
