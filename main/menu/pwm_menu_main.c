#include "pwm_menu_main.h"
#include "pwm_config_data.h"

static lv_obj_t *mode_label = NULL;
static lv_obj_t *main_container = NULL;
static lv_obj_t *mode_container = NULL;

static void menu_update(pwm_menu_handle_t *handle);

void main_container_init(pwm_menu_handle_t *handle);
static void mode_switch_init(pwm_menu_handle_t *handle);
static void start_button_create(pwm_menu_handle_t *handle);
static void settings_button_create(pwm_menu_handle_t *handle);
static void show_mode(pwm_menu_handle_t *handle);

static void mode_handler_cb(lv_event_t *e);
static void start_handler_cb(lv_event_t *e);
static void settings_handler_cb(lv_event_t *e);
static void screen_loaded_event_cb(lv_event_t *e);

void pwm_menu_main_init(pwm_menu_handle_t *handle) {
    lv_obj_add_event_cb(handle->menu_data->screens.main, screen_loaded_event_cb, LV_EVENT_SCREEN_LOADED, handle);
    main_container = lv_obj_create(handle->menu_data->screens.main);
    mode_container = lv_obj_create(main_container);
    mode_label = lv_label_create(mode_container);

    main_container_init(handle);
    mode_switch_init(handle);
    start_button_create(handle);
    settings_button_create(handle);
}

void menu_update(pwm_menu_handle_t *pwm) {

}

void main_container_init(pwm_menu_handle_t *handle) {
    lv_obj_set_size(main_container, handle->horz_res, handle->vert_res);    
    // Настраиваем flexbox
    lv_obj_set_flex_flow(main_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(main_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
}

static void mode_switch_init(pwm_menu_handle_t *handle) {
    // Контейнер с flexbox
    lv_obj_set_size(mode_container, 300, 90);
    lv_obj_align(mode_container, LV_ALIGN_TOP_MID, 0, 0);

    // Стиль рамки
    lv_obj_set_style_border_width(mode_container, 2, 0);
    lv_obj_set_style_border_color(mode_container, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_radius(mode_container, 12, 0);
    lv_obj_set_style_pad_all(mode_container, 15, 0);
    lv_obj_set_style_bg_color(mode_container, lv_palette_lighten(LV_PALETTE_BLUE, 4), 0);
    lv_obj_set_style_bg_opa(mode_container, LV_OPA_20, 0);

    // Настраиваем flexbox
    lv_obj_set_flex_flow(mode_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(mode_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Метка режима
    lv_obj_set_style_text_font(mode_label, &lv_font_montserrat_16, 0);

    // Переключатель режимов Тестовый/Рабочий
    lv_obj_t *mode_switch = lv_switch_create(mode_container);
    lv_obj_set_size(mode_switch, 120, 30);
    lv_obj_add_event_cb(mode_switch, mode_handler_cb, LV_EVENT_CLICKED, (void *)handle);

    if (handle->pwm_config->mode == MODE_TEST) {
        lv_obj_remove_state(mode_switch, LV_STATE_CHECKED);
    } else {
        lv_obj_add_state(mode_switch, LV_STATE_CHECKED);
    }

    show_mode(handle);
}

static void start_button_create(pwm_menu_handle_t *handle) {
    lv_obj_t *start_btn = lv_btn_create(main_container);
    lv_obj_set_size(start_btn, 120, 50);
    lv_obj_add_event_cb(start_btn, start_handler_cb, LV_EVENT_CLICKED, (void *)handle);
    
    lv_obj_t *label = lv_label_create(start_btn);
    lv_label_set_text(label, "Start");
    lv_obj_center(label);
}

static void settings_button_create(pwm_menu_handle_t *handle) {
    lv_obj_t *settings_btn = lv_btn_create(main_container);
    lv_obj_set_size(settings_btn, 120, 50);
    lv_obj_add_event_cb(settings_btn, settings_handler_cb, LV_EVENT_CLICKED, (void *)handle);
    
    lv_obj_t *label = lv_label_create(settings_btn);
    lv_label_set_text(label, "Settings");
    lv_obj_center(label);
}

static void mode_handler_cb(lv_event_t *e) {
    pwm_menu_handle_t *handle = (pwm_menu_handle_t *)lv_event_get_user_data(e);
}

static void start_handler_cb(lv_event_t *e) {
    pwm_menu_handle_t *handle = (pwm_menu_handle_t *)lv_event_get_user_data(e);
    lv_scr_load(handle->menu_data->screens.start);
}

static void settings_handler_cb(lv_event_t *e) {
    pwm_menu_handle_t *handle = (pwm_menu_handle_t *)lv_event_get_user_data(e);
    lv_scr_load(handle->menu_data->screens.settings);
}

static void screen_loaded_event_cb(lv_event_t *e) {
    pwm_menu_handle_t *handle = (pwm_menu_handle_t *)lv_event_get_user_data(e);
    menu_update(handle);
}

static void show_mode(pwm_menu_handle_t *handle) {
    lv_label_set_text(mode_label, 
        handle->pwm_config->mode == MODE_OPERATION ? "Mode: Operation" : "Mode: Test");
}
