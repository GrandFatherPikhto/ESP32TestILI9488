#include "pwm_config_data.h"

pwm_config_t *s_config;

// Расчет частот для EP4CE6
const uint32_t frequency_values[] = {
    10000,    // 10 kHz
    20000,    // 20 kHz  
    25000,    // 25 kHz
    40000,    // 40 kHz
    50000,    // 50 kHz
    100000,   // 100 kHz
    200000,   // 200 kHz
    250000,   // 250 kHz
    500000,   // 500 kHz
    1000000,  // 1 MHz
    2000000   // 2 MHz
};

static const char* frequency_labels[] = {
    "10 kHz", "20 kHz", "25 kHz", "40 kHz", "50 kHz", 
    "100 kHz", "200 kHz", "250 kHz", "500 kHz", "1 MHz", "2 MHz"
};

// Dead time значения в наносекундах
static const uint16_t deadtime_values[] = {
    10, 25, 50, 75, 100, 150, 200, 300, 500, 750, 1000
};

static const char* deadtime_labels[] = {
    "10 ns", "25 ns", "50 ns", "75 ns", "100 ns", 
    "150 ns", "200 ns", "300 ns", "500 ns", "750 ns", "1000 ns"
};

void config_data_init(pwm_menu_handle_t *data, pwm_config_t *pwm_config) {
    data->pwm_config = pwm_config;
    data->frequency_values = frequency_values;
    data->frequency_labels = frequency_labels;
    data->frequency_count = (uint8_t)(sizeof(frequency_values) / sizeof(frequency_values[0]));
    data->deadtime_values = deadtime_values;
    data->deadtime_labels = deadtime_labels;
    data->dead_time_count = (uint8_t)(sizeof(deadtime_values) / sizeof(deadtime_values[0]));
}

