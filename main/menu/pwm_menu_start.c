#include "pwm_menu_start.h"
#include "pwm_config_data.h"

#define START_VERT_SIZE 100

static lv_obj_t *running_container = NULL;
static lv_obj_t *table_container = NULL;
static lv_obj_t *stop_btn = NULL;
static lv_obj_t *main_options_table = NULL;
static lv_obj_t *hi_options_table = NULL;
static lv_obj_t *lo_options_table = NULL;
static lv_obj_t *hi_channel_label = NULL;
static lv_obj_t *lo_channel_label = NULL;

static void menu_update(pwm_menu_handle_t *handle);

static void init_start_screen(pwm_menu_handle_t *handle);
static void init_options_table(pwm_menu_handle_t *handle);
static void init_hi_channel_table(pwm_menu_handle_t *handle);
static void init_lo_channel_table(pwm_menu_handle_t *handle);
static void init_stop_btn(pwm_menu_handle_t *handle);
static void stop_handler_cb(lv_event_t *e);
static void screen_loaded_event_cb(lv_event_t *e);

void pwm_menu_start_init(pwm_menu_handle_t *handle) {
    running_container = lv_obj_create(handle->menu_data->screens.start);
    stop_btn = lv_btn_create(running_container);
    table_container = lv_obj_create(handle->menu_data->screens.start);
    main_options_table = lv_table_create(table_container);
    hi_channel_label = lv_label_create(table_container);
    hi_options_table = lv_table_create(table_container);
    lo_channel_label = lv_label_create(table_container);
    lo_options_table = lv_table_create(table_container);
    
    init_start_screen(handle);
    init_stop_btn(handle);
    init_options_table(handle);
    init_hi_channel_table(handle);
    init_lo_channel_table(handle);

    lv_obj_add_event_cb(handle->menu_data->screens.settings, screen_loaded_event_cb, LV_EVENT_SCREEN_LOADED, handle);
}

static void menu_update(pwm_menu_handle_t *handle) {
    char title[0x40];
    lv_table_set_cell_value(lo_options_table, 0, 1, handle->pwm_config->channel_hi.enabled ? "On" : "Off");
    snprintf(title, 0x40, "%u us", handle->pwm_config->channel_hi.delay);
    lv_table_set_cell_value(lo_options_table, 1, 1, title);
    snprintf(title, 0x40, "%u us", handle->pwm_config->channel_hi.duration);
    lv_table_set_cell_value(lo_options_table, 2, 1, title);
    snprintf(title, 0x40, "%u %%", handle->pwm_config->channel_hi.duty);
    lv_table_set_cell_value(lo_options_table, 3, 1, title);

    lv_table_set_cell_value(hi_options_table, 0, 1, handle->pwm_config->channel_lo.enabled ? "On" : "Off");
    snprintf(title, 0x40, "%u us", handle->pwm_config->channel_lo.delay);
    lv_table_set_cell_value(hi_options_table, 1, 1, title);
    snprintf(title, 0x40, "%u us", handle->pwm_config->channel_lo.duration);
    lv_table_set_cell_value(hi_options_table, 2, 1, title);
    snprintf(title, 0x40, "%u %%", handle->pwm_config->channel_hi.duty);
    lv_table_set_cell_value(hi_options_table, 3, 1, title);

    lv_table_set_cell_value(main_options_table, 0, 1, handle->frequency_labels[handle->pwm_config->frequency_index]);
    lv_table_set_cell_value(main_options_table, 1, 1, handle->deadtime_labels[handle->pwm_config->deadtime_index]);    
}

static void init_start_screen(pwm_menu_handle_t *handle) {
    lv_obj_t *screen = handle->menu_data->screens.start;
    lv_obj_set_size(running_container, handle->horz_res, START_VERT_SIZE);
    lv_obj_align(running_container, LV_ALIGN_TOP_MID, 0, 0);
    // Настраиваем flexbox
    lv_obj_set_flex_flow(running_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(running_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_size(table_container, handle->horz_res, handle->vert_res - START_VERT_SIZE);
    lv_obj_align(table_container, LV_ALIGN_TOP_MID, 0, 100);

    // Настраиваем flexbox
    lv_obj_set_flex_flow(table_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(table_container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
}

static void init_stop_btn(pwm_menu_handle_t *handle) {
    lv_obj_set_size(stop_btn, 120, 50);
    lv_obj_add_event_cb(stop_btn, stop_handler_cb, LV_EVENT_CLICKED, handle);
    
    lv_obj_t *stop_label = lv_label_create(stop_btn);
    lv_label_set_text(stop_label, "Stop");
    lv_obj_center(stop_label);
}

static void init_options_table(pwm_menu_handle_t *handle) {
    lv_obj_align(main_options_table, LV_ALIGN_TOP_MID, 0, 100);

    lv_table_set_cell_value(main_options_table, 0, 0, "PWM Frequency");
    lv_table_set_cell_value(main_options_table, 1, 0, "Dead Time");
}

static void init_hi_channel_table(pwm_menu_handle_t *handle) {
    char title[0x20];
    lv_label_set_text(hi_channel_label, "HI Channel");
    lv_obj_center(hi_channel_label);

    lv_table_set_cell_value(hi_options_table, 0, 0, "Enabled");
    lv_table_set_cell_value(hi_options_table, 1, 0, "Delay");
    lv_table_set_cell_value(hi_options_table, 2, 0, "Duration");
    lv_table_set_cell_value(hi_options_table, 3, 0, "Duty");
}

static void init_lo_channel_table(pwm_menu_handle_t *handle) {
    lv_label_set_text(lo_channel_label, "LO Channel");
    lv_obj_center(lo_channel_label);

    lv_table_set_cell_value(lo_options_table, 0, 0, "Enabled");
    lv_table_set_cell_value(lo_options_table, 1, 0, "Delay");
    lv_table_set_cell_value(lo_options_table, 2, 0, "Duration");
    lv_table_set_cell_value(lo_options_table, 3, 0, "Duty");
}

static void stop_handler_cb(lv_event_t *e) {
    pwm_menu_handle_t *handle = (pwm_menu_handle_t *)lv_event_get_user_data(e);
    lv_scr_load(handle->menu_data->screens.main);
}

static void screen_loaded_event_cb(lv_event_t *e) {
    pwm_menu_handle_t *handle = (pwm_menu_handle_t *)lv_event_get_user_data(e);
    menu_update(handle);
}