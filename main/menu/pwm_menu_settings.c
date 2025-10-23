#include "pwm_menu_settings.h"
#include "pwm_menu_hi_channel.h"
#include "pwm_menu_lo_channel.h"
#include "pwm_config_data.h"

static lv_obj_t *settings_container = NULL;
static lv_obj_t *hi_enable_switch = NULL;
static lv_obj_t *lo_enable_switch = NULL;
static lv_obj_t *frequency_value_label = NULL;
static lv_obj_t *deadtime_value_label = NULL;
static lv_obj_t *channel_hi_container = NULL;
static lv_obj_t *channel_lo_container = NULL;

static void menu_update(pwm_menu_handle_t *handle);

static void init_settings_screen(pwm_menu_handle_t *handle);
static void create_back_btn(pwm_menu_handle_t *handle);
static void create_frequency_pane(pwm_menu_handle_t *handle);
static void create_deadtime_pane(pwm_menu_handle_t *handle);
static void create_hi_channel_btn(pwm_menu_handle_t *handle);
static void create_lo_channel_btn(pwm_menu_handle_t *handle);

inline static void frequency_value_update(pwm_menu_handle_t *handle);
inline static void deadtime_value_update(pwm_menu_handle_t *handle);

static void back_handler_cb(lv_event_t *event);
static void hi_enable_handler_cb(lv_event_t *e);
static void lo_enable_handler_cb(lv_event_t *e);
static void frequency_decrement_handler_cb(lv_event_t *e);
static void frequency_increment_handler_cb(lv_event_t *e);
static void deadtime_decrement_handler_cb(lv_event_t *e);
static void deadtime_increment_handler_cb(lv_event_t *e);
static void hi_channel_handler_cb(lv_event_t *e);
static void lo_channel_handler_cb(lv_event_t *e);
static void screen_loaded_event_cb(lv_event_t *e);

void pwm_menu_settings_init(pwm_menu_handle_t *handle) {
    lv_obj_add_event_cb(handle->menu_data->screens.settings, screen_loaded_event_cb, LV_EVENT_SCREEN_LOADED, handle);
    settings_container = lv_obj_create(handle->menu_data->screens.settings);

    init_settings_screen(handle);
    create_back_btn(handle);
    create_frequency_pane(handle);
    create_deadtime_pane(handle);
    create_hi_channel_btn(handle);
    create_lo_channel_btn(handle);
}

static void menu_update(pwm_menu_handle_t *handle) {
    frequency_value_update(handle);
    deadtime_value_update(handle);
}

static void create_back_btn(pwm_menu_handle_t *handle) {
    lv_obj_t *back_btn = lv_btn_create(settings_container);

    lv_obj_set_size(back_btn, 120, 40);
    lv_obj_add_event_cb(back_btn, back_handler_cb, LV_EVENT_CLICKED, handle);
    
    lv_obj_t *back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, "Main >");
    lv_obj_center(back_label);
}

static void init_settings_screen(pwm_menu_handle_t *handle) {
    lv_obj_t *settings_screen = handle->menu_data->screens.settings;

    lv_obj_set_size(settings_container, handle->horz_res, handle->vert_res);
    lv_obj_align(settings_container, LV_ALIGN_TOP_MID, 0, 0);
    // Настраиваем flexbox
    lv_obj_set_flex_flow(settings_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(settings_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    lv_obj_t *label = lv_label_create(settings_container);
    lv_label_set_text(label, "Options");
    lv_obj_center(label);
}

static void create_frequency_pane(pwm_menu_handle_t *handle) {
    // Контейнер для значения частоты
    lv_obj_t *frequency_label = lv_label_create(settings_container);
    lv_label_set_text(frequency_label, "PWM Frequency");

    lv_obj_t *cont = lv_obj_create(settings_container);
    lv_obj_set_size(cont, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0xF0F0F0), 0);
    lv_obj_set_style_border_width(cont, 1, 0);
    
    
    // Кнопки для изменения частоты
    lv_obj_t *frequency_minus = lv_btn_create(cont);
    lv_obj_t *minus_label = lv_label_create(frequency_minus);
    lv_label_set_text(minus_label, LV_SYMBOL_LEFT);
    lv_obj_center(minus_label);
    lv_obj_add_event_cb(frequency_minus, frequency_decrement_handler_cb, LV_EVENT_CLICKED, handle);

    // Label для отображения значения частоты
    frequency_value_label = lv_label_create(cont);
    
    frequency_value_update(handle);
    lv_obj_set_style_text_font(frequency_value_label, &lv_font_montserrat_16, 0);

    // Кнопки для изменения частоты
    lv_obj_t *frequency_plus = lv_btn_create(cont);
    lv_obj_t *plus_label = lv_label_create(frequency_plus);
    lv_label_set_text(plus_label, LV_SYMBOL_RIGHT);
    lv_obj_center(plus_label);
    lv_obj_add_event_cb(frequency_plus, frequency_increment_handler_cb, LV_EVENT_CLICKED, handle);
}

static void create_deadtime_pane(pwm_menu_handle_t *handle) {
    // Контейнер для значения частоты
    lv_obj_t *title_label = lv_label_create(settings_container);
    lv_label_set_text(title_label, "Dead Time");

    lv_obj_t *cont = lv_obj_create(settings_container);
    lv_obj_set_size(cont, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(cont, lv_color_hex(0xF0F0F0), 0);
    lv_obj_set_style_border_width(cont, 1, 0);
    
    
    // Кнопки для изменения частоты
    lv_obj_t *minus_btn = lv_btn_create(cont);
    lv_obj_t *minus_label = lv_label_create(minus_btn);
    lv_label_set_text(minus_label, LV_SYMBOL_LEFT);
    lv_obj_center(minus_label);
    lv_obj_add_event_cb(minus_btn, deadtime_decrement_handler_cb, LV_EVENT_CLICKED, handle);

    // Label для отображения значения частоты
    deadtime_value_label = lv_label_create(cont);    
    deadtime_value_update(handle);
    lv_obj_set_style_text_font(deadtime_value_label, &lv_font_montserrat_16, 0);

    // Кнопки для изменения частоты
    lv_obj_t *plus_btn = lv_btn_create(cont);
    lv_obj_t *plus_label = lv_label_create(plus_btn);
    lv_label_set_text(plus_label, LV_SYMBOL_RIGHT);
    lv_obj_center(plus_label);
    lv_obj_add_event_cb(plus_btn, deadtime_increment_handler_cb, LV_EVENT_CLICKED, handle);
}

static void create_hi_channel_btn(pwm_menu_handle_t *handle) {
    lv_obj_t *btn = lv_btn_create(settings_container);
    lv_obj_set_size(btn, 120, 50);
    lv_obj_add_event_cb(btn, hi_channel_handler_cb, LV_EVENT_CLICKED, (void *)handle);
    
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "Hi Channel");
    lv_obj_center(label);
}

static void create_lo_channel_btn(pwm_menu_handle_t *handle) {
    lv_obj_t *btn = lv_btn_create(settings_container);
    lv_obj_set_size(btn, 120, 50);
    lv_obj_add_event_cb(btn, lo_channel_handler_cb, LV_EVENT_CLICKED, (void *)handle);
    
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "Lo Channel");
    lv_obj_center(label);
}

inline static void frequency_value_update(pwm_menu_handle_t *handle) {
    lv_label_set_text(frequency_value_label, handle->frequency_labels[handle->pwm_config->frequency_index]);
}

inline static void deadtime_value_update(pwm_menu_handle_t *handle) {
    lv_label_set_text(deadtime_value_label, handle->deadtime_labels[handle->pwm_config->deadtime_index]);
}

static void back_handler_cb(lv_event_t *e) {
    pwm_menu_handle_t *handle = lv_event_get_user_data(e);
    lv_scr_load(handle->menu_data->screens.main);
}

static void hi_enable_handler_cb(lv_event_t *e) {
    pwm_menu_handle_t *handle = lv_event_get_user_data(e);
}

static void lo_enable_handler_cb(lv_event_t *e) {
    pwm_menu_handle_t *handle = lv_event_get_user_data(e);
}

static void frequency_decrement_handler_cb(lv_event_t *e) {
    pwm_menu_handle_t *handle = lv_event_get_user_data(e);
    if (handle->pwm_config->frequency_index != 0) {
        handle->pwm_config->frequency_index --;
        frequency_value_update(handle);
    }
}

static void frequency_increment_handler_cb(lv_event_t *e) {
    pwm_menu_handle_t *handle = lv_event_get_user_data(e);
    if (handle->pwm_config->frequency_index != handle->frequency_count - 1) {
        handle->pwm_config->frequency_index ++;
        frequency_value_update(handle);
    }
}

static void deadtime_decrement_handler_cb(lv_event_t *e) {
    pwm_menu_handle_t *handle = lv_event_get_user_data(e);
    if (handle->pwm_config->deadtime_index != 0) {
        handle->pwm_config->deadtime_index --;
        deadtime_value_update(handle);
    }
}

static void deadtime_increment_handler_cb(lv_event_t *e) {
    pwm_menu_handle_t *handle = lv_event_get_user_data(e);
    if (handle->pwm_config->deadtime_index != handle->dead_time_count - 1) {
        handle->pwm_config->deadtime_index ++;
        deadtime_value_update(handle);
    }
}

static void screen_loaded_event_cb(lv_event_t *e) {
    pwm_menu_handle_t *handle = lv_event_get_user_data(e);
    menu_update(handle);
}

static void hi_channel_handler_cb(lv_event_t *e) {
    pwm_menu_handle_t *handle = lv_event_get_user_data(e);
    lv_scr_load(handle->menu_data->screens.hi_channel);
}

static void lo_channel_handler_cb(lv_event_t *e) {
    pwm_menu_handle_t *handle = lv_event_get_user_data(e);
    lv_scr_load(handle->menu_data->screens.lo_channel);
}
