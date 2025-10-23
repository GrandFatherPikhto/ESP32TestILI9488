#ifndef PWM_MENU_H
#define PWM_MENU_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "lvgl/lvgl.h"

typedef struct pwm_menu_handle pwm_menu_handle_t;

void pwm_menu_init(pwm_menu_handle_t *handle, uint16_t horz_res, uint16_t vert_res);

#endif /* PWM_MENU_H */