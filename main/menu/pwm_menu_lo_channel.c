#include "pwm_menu_lo_channel.h"
#include "pwm_config_data.h"

static lv_obj_t *container = NULL;
static lv_obj_t *delay_label = NULL;
static lv_obj_t *duration_label = NULL;
static lv_obj_t *duty_label = NULL;
static lv_obj_t *enable_switch = NULL;
static lv_obj_t *delay_slider = NULL;
static lv_obj_t *duration_slider = NULL;
static lv_obj_t *duty_slider = NULL;

static void menu_update(pwm_menu_handle_t *handle);

static void create_main_container(pwm_menu_handle_t *handle);
static void create_switch_panel(pwm_menu_handle_t *handle);
static void create_back_btns(pwm_menu_handle_t *handle);
static void create_delay_panel(pwm_menu_handle_t *handle);
static void create_duration_panel(pwm_menu_handle_t *handle);
static void create_duty_panel(pwm_menu_handle_t *handle);

static void back_handler_cb(lv_event_t *event);
static void main_handler_cb(lv_event_t *event);
static void delay_handler_cb(lv_event_t *event);
static void duration_handler_cb(lv_event_t *event);
static void duty_handler_cb(lv_event_t *event);
static void enable_handler_cb(lv_event_t *event);
static void screen_loaded_event_cb(lv_event_t *e);

inline static void update_delay_label(pwm_menu_handle_t *handle);
inline static void update_duration_label(pwm_menu_handle_t *handle);
inline static void update_duty_label(pwm_menu_handle_t *handle);

void pwm_menu_lo_channel_init(pwm_menu_handle_t *handle) {
    lv_obj_add_event_cb(handle->menu_data->screens.lo_channel, screen_loaded_event_cb, LV_EVENT_SCREEN_LOADED, handle);
    create_main_container(handle);
    lv_obj_t *label = lv_label_create(container);
    lv_label_set_text(label, "Lo Channel");
    create_back_btns(handle);
    create_switch_panel(handle);
    create_delay_panel(handle);
    create_duration_panel(handle);
    create_duty_panel(handle);
}

void menu_update(pwm_menu_handle_t *handle) {
    update_delay_label(handle);
    update_duration_label(handle);
    update_duty_label(handle);
    if (handle->pwm_config->channel_lo.enabled) {
        lv_obj_add_state(enable_switch, LV_STATE_CHECKED);
    } else {
        lv_obj_remove_state(enable_switch, LV_STATE_CHECKED);
    }
    
    lv_slider_set_value(delay_slider, handle->pwm_config->channel_lo.delay, LV_ANIM_OFF);
    lv_slider_set_value(duration_slider, handle->pwm_config->channel_lo.duration, LV_ANIM_OFF);
    lv_slider_set_value(duty_slider, handle->pwm_config->channel_lo.duty, LV_ANIM_OFF);
}

static void create_main_container(pwm_menu_handle_t *handle) {
    container = lv_obj_create(handle->menu_data->screens.lo_channel);
    lv_obj_set_size(container, handle->horz_res, LV_SIZE_CONTENT);    
    // Настраиваем flexbox
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
}

static void create_back_btns(pwm_menu_handle_t *handle) {
    lv_obj_t *cont = lv_obj_create(container);
    lv_obj_set_size(cont, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0xF0F0F0), 0);
    lv_obj_set_style_border_width(cont, 1, 0);

    lv_obj_t *btn = lv_btn_create(cont);
    lv_obj_t *label = lv_label_create(btn);
    lv_obj_add_event_cb(btn, back_handler_cb, LV_EVENT_CLICKED, (void *)handle);
    lv_label_set_text(label, "< Settings");
    lv_obj_center(label);

    btn = lv_btn_create(cont);
    label = lv_label_create(btn);
    lv_obj_add_event_cb(btn, main_handler_cb, LV_EVENT_CLICKED, (void *)handle);
    lv_label_set_text(label, "Main >");
    lv_obj_center(label);
}

static void create_switch_panel(pwm_menu_handle_t *handle) {
    lv_obj_t *label = lv_label_create(container);
    lv_label_set_text(label, "Enable");
    enable_switch = lv_switch_create(container);
    lv_obj_set_size(enable_switch, 100, 30);
    lv_obj_add_event_cb(enable_switch, enable_handler_cb, LV_EVENT_VALUE_CHANGED, handle);
}

static void create_delay_panel(pwm_menu_handle_t *handle) {
    delay_label = lv_label_create(container);
    update_delay_label(handle);
    delay_slider = lv_slider_create(container);
    lv_slider_set_range(delay_slider, 0, 1000);
    lv_obj_add_event_cb(delay_slider, delay_handler_cb, LV_EVENT_VALUE_CHANGED, handle);
}

static void create_duration_panel(pwm_menu_handle_t *handle) {
    duration_label = lv_label_create(container);
    update_duration_label(handle);
    duration_slider = lv_slider_create(container);
    lv_slider_set_range(duration_slider, 0, 1000);
    lv_obj_add_event_cb(duration_slider, duration_handler_cb, LV_EVENT_VALUE_CHANGED, handle);
}

static void create_duty_panel(pwm_menu_handle_t *handle) {
    duty_label = lv_label_create(container);
    update_duty_label(handle);
    duty_slider = lv_slider_create(container);
    lv_slider_set_range(duty_slider, 0, 100);
    lv_obj_add_event_cb(duty_slider, duty_handler_cb, LV_EVENT_VALUE_CHANGED, handle);
    lv_slider_set_value(duty_slider, handle->pwm_config->channel_lo.duty, LV_ANIM_OFF);
}

static void back_handler_cb(lv_event_t *event) {
    pwm_menu_handle_t *handle = lv_event_get_user_data(event);
    lv_scr_load(handle->menu_data->screens.settings);
}

static void main_handler_cb(lv_event_t *event) {
    pwm_menu_handle_t *handle = lv_event_get_user_data(event);
    lv_scr_load(handle->menu_data->screens.main);
}

static void delay_handler_cb(lv_event_t *event) {
    pwm_menu_handle_t *handle = lv_event_get_user_data(event);
    handle->pwm_config->channel_lo.delay = (uint16_t)lv_slider_get_value(delay_slider);
    update_delay_label(handle);
}

static void duration_handler_cb(lv_event_t *event) {
    pwm_menu_handle_t *handle = lv_event_get_user_data(event);
    handle->pwm_config->channel_lo.duration = (uint16_t)lv_slider_get_value(duration_slider);
    update_duration_label(handle);
}

static void duty_handler_cb(lv_event_t *event) {
    pwm_menu_handle_t *handle = lv_event_get_user_data(event);
    handle->pwm_config->channel_lo.duty = (uint16_t)lv_slider_get_value(duty_slider);
    // printf("Hi: %u, Lo: %u\n", handle->pwm_config->channel_hi.duty, handle->pwm_config->channel_lo.duty);
    if (handle->pwm_config->channel_hi.duty + handle->pwm_config->channel_lo.duty > 100) {
        handle->pwm_config->channel_hi.duty = 100 - handle->pwm_config->channel_lo.duty;
    }
    update_duty_label(handle);
}

static void enable_handler_cb(lv_event_t *event) {
    pwm_menu_handle_t *handle = lv_event_get_user_data(event);
    if (lv_obj_has_state(enable_switch, LV_STATE_CHECKED)) {
        handle->pwm_config->channel_lo.enabled = true;
    } else {
        handle->pwm_config->channel_lo.enabled = false;
    }
}

static void screen_loaded_event_cb(lv_event_t *e) {
    pwm_menu_handle_t *handle = lv_event_get_user_data(e);
    menu_update(handle);
}

inline static void update_delay_label(pwm_menu_handle_t *handle) {
    lv_label_set_text_fmt(delay_label, "Delay: %u", handle->pwm_config->channel_lo.delay);
}

inline static void update_duration_label(pwm_menu_handle_t *handle) {
    lv_label_set_text_fmt(duration_label, "Duration: %u", handle->pwm_config->channel_lo.duration);
}

inline static void update_duty_label(pwm_menu_handle_t *handle) {
    lv_label_set_text_fmt(duty_label, "Duty: %u", handle->pwm_config->channel_lo.duty);
}