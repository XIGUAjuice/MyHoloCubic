#ifndef GLOBAL_H
#define GLOBAL_H

#include <lvgl.h>

extern lv_obj_t *tile_main;
extern lv_obj_t *page_home;
extern lv_obj_t *page_setting;
extern lv_obj_t *page_setting_wifi;
extern lv_obj_t *page_smart_config;
extern lv_font_t hanzi_25;

extern lv_obj_t *drop_wifi;
extern lv_obj_t *text_area_pwd;
extern lv_obj_t *keyboard;
extern lv_obj_t *btn_scan;
extern lv_obj_t *btn_connect;

extern lv_obj_t *img_wifi;
LV_IMG_DECLARE(base_face);

extern lv_font_t digital;
extern lv_obj_t *roller_time[5];
extern lv_obj_t *cont_time;

extern lv_obj_t *label_time;
extern lv_font_t digital_medium;

extern lv_obj_t *cont_date;
extern lv_obj_t *label_date[3];
extern lv_font_t alphabet;

extern lv_obj_t *img_weather;
extern lv_obj_t *img_temp;
extern lv_obj_t *label_temp;
extern lv_obj_t *label_humidity;
extern lv_font_t temp_font;

extern lv_indev_t *my_indev;
extern lv_indev_drv_t indev_drv;
extern lv_group_t *group_tile;

extern lv_obj_t *spinner_smart_config;
extern lv_obj_t *img_ok;
LV_IMG_DECLARE(smart_config);

extern lv_obj_t *page_alarm;
extern lv_obj_t *roller_alarm[5];
extern lv_obj_t *page_alarm_setting;
extern lv_obj_t *cont_alarm;

extern lv_obj_t *page_weather;
extern lv_obj_t *img_DayWeather_today;
extern lv_obj_t *img_DayWeather_tomorrow;
extern lv_obj_t *img_DayWeather_after_tomorrow;
extern lv_obj_t *label_DayTemp_today;
extern lv_obj_t *label_DayTemp_tomorrow;
extern lv_obj_t *label_DayTemp_after_tomorrow;
extern lv_obj_t *label_NightTemp_today;
extern lv_obj_t *label_NightTemp_tomorrow;
extern lv_obj_t *label_NightTemp_after_tomorrow;
extern lv_chart_series_t* ser1;
extern lv_chart_series_t* ser2;

extern lv_obj_t *label_temp_city1;
extern lv_obj_t *label_temp_city2;
extern lv_obj_t *label_temp_city3;
extern lv_obj_t *img_weather1;
extern lv_obj_t *img_weather2;
extern lv_obj_t *img_weather3;
extern lv_obj_t *label_text_city1;
extern lv_obj_t *label_text_city2;
extern lv_obj_t *label_text_city3;
extern lv_obj_t *page_multi_weather;

#endif