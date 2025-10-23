#ifndef LVGL_PORT_H
#define LVGL_PORT_H

#include "lvgl.h"

void lvgl_port_init(void);
void lvgl_port_start(void);
void lvgl_port_lock(void);
void lvgl_port_unlock(void);

#endif /* LVGL_PORT_H */
