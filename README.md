| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-C61 | ESP32-H2 | ESP32-H21 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | --------- | -------- | --------- | -------- | -------- | -------- |

# SPI LCD Touch Example with ILI9488

Этот проект демонстрирует работу с SPI LCD дисплеем с сенсорным экраном на чипе ESP32-S3, используя драйвер ILI9488 с 24-битным цветом (RGB888).

## Особенности

- Поддержка LCD контроллеров: ILI9341, GC9A01, ILI9488
- Поддержка сенсорных контроллеров: STMPE610, XPT2046
- Использование LVGL для графического интерфейса
- Аппаратное ускорение SPI с DMA
- Многозадачная обработка LVGL

## Требования

### Оборудование

- Плата разработки ESP32-S3
- SPI LCD дисплей с контроллером ILI9488
- Сенсорный контроллер (опционально)
- Соединительные провода

### Программное обеспечение

- [ESP-IDF](https://github.com/espressif/esp-idf) версии 5.0 или выше
- [Драйвер ILI9488](https://github.com/atanisoft/esp_lcd_ili9488)

## Подключение

| ESP32-S3 Pin | LCD Pin | Функция |
|--------------|---------|---------|
| GPIO18       | SCLK    | SPI Clock |
| GPIO19       | MOSI    | SPI Data Out |
| GPIO21       | MISO    | SPI Data In (опционально) |
| GPIO5        | DC      | Data/Command |
| GPIO3        | RST     | Reset |
| GPIO4        | CS      | LCD Chip Select |
| GPIO2        | BL      | Backlight Control |
| GPIO15       | T_CS    | Touch Chip Select (если используется) |

## Конфигурация

Перед сборкой настройте проект с помощью `idf.py menuconfig`:

### Основные настройки LCD

1. Перейдите в `Example Configuration` → `LCD controller selection` и выберите `ILI9488`
2. Установите разрешение дисплея: 320x480 пикселей
3. Настройте пины в соответствии с вашей схемой подключения

### Настройки сенсорного экрана (опционально)

1. Включите сенсорный экран в `Example Configuration` → `Touch controller enabled`
2. Выберите сенсорный контроллер (STMPE610 или XPT2046)
3. Настройте соответствующие пины

### Настройки LVGL

1. Настройте размер буфера отрисовки (рекомендуется 1/10 размера экрана)
2. При необходимости измените приоритет задачи LVGL

## Сборка и запуск

### В корне проекта в файле idf_componen.yml
Добавьте
atanisoft/esp_lcd_ili9488: 1.0.11

### В файле Kconfig.projbuild добавить опцию для ILI9488:
```kconfig
    choice EXAMPLE_LCD_CONTROLLER
        prompt "LCD controller model"
        default EXAMPLE_LCD_CONTROLLER_ILI9341
        help
            Select LCD controller model

        config EXAMPLE_LCD_CONTROLLER_ILI9341
            bool "ILI9341"

        config EXAMPLE_LCD_CONTROLLER_ILI9488
            bool "ILI9488"

        config EXAMPLE_LCD_CONTROLLER_GC9A01
            bool "GC9A01"
    endchoice
```
### В начале файла spi_lcd_touch_example_main.c

дописать настройку для включения заголовочного файла
```c
#if CONFIG_EXAMPLE_LCD_CONTROLLER_ILI9341
#include "esp_lcd_ili9341.h"
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_GC9A01
#include "esp_lcd_gc9a01.h"
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_ILI9488
#include "esp_lcd_ili9488.h"
#endif
```

### Дописать конфигурацию для дисплея ILI9488, например:
```c
// The pixel number in horizontal and vertical
#if CONFIG_EXAMPLE_LCD_CONTROLLER_ILI9341
#define EXAMPLE_LCD_H_RES              240
#define EXAMPLE_LCD_V_RES              320
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_GC9A01
#define EXAMPLE_LCD_H_RES              240
#define EXAMPLE_LCD_V_RES              240
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_ILI9488
#define EXAMPLE_LCD_H_RES              320
#define EXAMPLE_LCD_V_RES              480
#endif
```

### Добавить функцию инициализации дисплея ILI9488

```c
#if CONFIG_EXAMPLE_LCD_CONTROLLER_ILI9341
    ESP_LOGI(TAG, "Install ILI9341 panel driver");
    ESP_ERROR_CHECK(esp_lcd_new_panel_ili9341(io_handle, &panel_config, &panel_handle));
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_GC9A01
    ESP_LOGI(TAG, "Install GC9A01 panel driver");
    ESP_ERROR_CHECK(esp_lcd_new_panel_gc9a01(io_handle, &panel_config, &panel_handle));
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_ILI9488
    ESP_LOGI(TAG, "Install 9488 panel driver");
    ESP_ERROR_CHECK(esp_lcd_new_panel_ili9488(io_handle, &panel_config, draw_buffer_sz, &panel_handle));
#endif
```

### Настроить на передачу 24 бит на один пиксел
```c
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = EXAMPLE_PIN_NUM_LCD_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
#if CONFIG_EXAMPLE_LCD_CONTROLLER_ILI9488
        .bits_per_pixel = 24,
#else
        .bits_per_pixel = 16,
#endif
    };

```

### Наконец, в меню конфигурации установить цвет
(SDK Configuration)

Color depth:

24:RGB888

## Особенности реализации

### Цветовой формат

Проект использует 24-битный цветовой формат (RGB888) для ILI9488:
- `bits_per_pixel = 24`
- `rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR`

### Буферизация

- Используется двойная буферизация для плавной анимации
- Буферы выделяются в DMA-памяти для эффективной передачи
- Размер буфера: `EXAMPLE_LCD_H_RES * 20 * sizeof(uint16_t)`

### Обработка сенсорного ввода

При использовании сенсорного экрана:
- Координаты автоматически преобразуются в соответствии с ориентацией дисплея
- Поддержка многопальцевого ввода (до 1 касания в данной конфигурации)


## Дополнительные ресурсы

- [Исходный пример](https://github.com/espressif/esp-idf/tree/master/examples/peripherals/lcd/spi_lcd_touch)
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/index.html)
- [LVGL Documentation](https://docs.lvgl.io/master/)
- [ILI9488 Atanisoft Driver](https://github.com/atanisoft/esp_lcd_ili9488)

### Hardware Connection

Для экрана и тачскрина используется один SPI:

```
       ESP Board                       GC9A01/ILI9341 Panel + TOUCH
┌──────────────────────┐              ┌────────────────────┐
│             GND      ├─────────────►│ GND                │
│                      │              │                    │
│             3V3      ├─────────────►│ VCC                │
│                      │              │                    │
│             PCLK     ├─────────────►│ SCL                │
│                      │              │                    │
│             MOSI     ├─────────────►│ MOSI               │
│                      │              │                    │
│             MISO     |◄─────────────┤ MISO               │
│                      │              │                    │
│             RST      ├─────────────►│ RES                │
│                      │              │                    │
│             DC       ├─────────────►│ DC                 │
│                      │              │                    │
│             LCD CS   ├─────────────►│ LCD CS             │
│                      │              │                    │
│             TOUCH CS ├─────────────►│ TOUCH CS           │
│                      │              │                    │
│             BK_LIGHT ├─────────────►│ BLK                │
└──────────────────────┘              └────────────────────┘
```

Таким образом, 
- SDI объединён с TDI (MISO -- Mater In Slave Out)
- SCK объединён c TCK (Serial Clock)
- SDO объединён с TDO (MOSI -- Master Out Slave In)
- CS -- управляет дисплеем
- TCS -- управляет touch screen
- PEN -- можно включать, можно не включать.
