#include "pwm_menu.h"
#include "pwm_menu_main.h"
#include "pwm_menu_start.h"
#include "pwm_menu_settings.h"
#include "pwm_menu_hi_channel.h"
#include "pwm_menu_lo_channel.h"
#include "pwm_config_data.h"

void pwm_menu_init(pwm_menu_handle_t *handle, uint16_t horz_res, uint16_t vert_res) {
    static pwm_menu_data_t menu_data = {0};
    handle->menu_data = &menu_data;

    handle->horz_res = horz_res;
    handle->vert_res = vert_res;
    
    menu_data.screens.main = lv_obj_create(NULL);
    menu_data.screens.start = lv_obj_create(NULL);
    menu_data.screens.settings = lv_obj_create(NULL);
    menu_data.screens.hi_channel = lv_obj_create(NULL);
    menu_data.screens.lo_channel = lv_obj_create(NULL);

    pwm_menu_main_init(handle);
    pwm_menu_start_init(handle);
    pwm_menu_settings_init(handle);
    pwm_menu_hi_channel_init(handle);
    pwm_menu_lo_channel_init(handle);

    lv_scr_load(menu_data.screens.main);
}