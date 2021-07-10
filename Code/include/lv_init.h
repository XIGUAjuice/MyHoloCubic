#ifndef LV_INIT_H
#define LV_INIT_H

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Arduino.h>
#include <lv_port_fatfs.h>

#define LCD_BL_PIN 5
#define LCD_BL_PWM_CHANNEL 0

extern TFT_eSPI tft;

/* 屏幕刷新回调函数 */
void my_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);

/* 设置屏幕背光亮度 */
void setBackLight(float duty);

/* LVGL初始化 */
void initLvgl();

#endif