#ifndef MAIN_CONFIG_H
#define MAIN_CONFIG_H

// The pixel number in horizontal and vertical
#if CONFIG_EXAMPLE_LCD_CONTROLLER_ILI9341
#define LCD_H_RES              240
#define LCD_V_RES              320
#define LCD_BITS_PER_PIXEL     16
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_GC9A01
#define LCD_H_RES              240
#define LCD_V_RES              240
#define LCD_BITS_PER_PIXEL     16
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_ILI9488
#define LCD_H_RES              320
#define LCD_V_RES              480
#define LCD_BITS_PER_PIXEL     24
#endif


#endif /* MAIN_CONFIG_H */