#ifndef CONTROL_H
#define CONTROL_H

#include <lvgl.h>
#include <global.h>
#include <Arduino.h>
#include <fresh_task.h>
#include <cityweather.h>

/* 磁贴回调函数 */
// static void tile_event_cb(lv_obj_t *obj, lv_event_t event);

/* 创建磁贴 */
void createTile();

#endif