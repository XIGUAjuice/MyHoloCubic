#ifndef FRESH_TASK_H
#define FRESH_TASK_H

#include <home_page.h>
#include <network.h>
#include <global.h>
#include <sd_card.h>
#include <esp_wifi.h>
#include <page_alarm.h>
#include <weather_page.h>

extern lv_task_t *task_fresh_weather;
extern lv_task_t *task_fresh_info;
extern lv_task_t *task_fresh_smart_config;
extern lv_task_t *task_fresh_city_weather;
extern lv_task_t *task_fresh_weather_forecast;

/* 获取天气图标 */
void getWeatherIcon(String weather, int h, char *icon);

/* 刷新日期 */
void freshDate(time_t time_now, int &prev_day);

/* 刷新时间 */
void freshTime(time_t time_now, int &prev_second);

/* 刷新天气 */
void freshWeather(lv_task_t *task);

/* 刷新天气预报 */
void fresh_weather_forecast(lv_task_t *task);

/* 刷新城市天气 */
void freshCityWeather(lv_task_t *task);

/* 刷新WiFi连接情况 */
void freshWifiConnect();

/* 刷新日期时间 */
void freshInfo(lv_task_t *task);

/* 刷新Smart Config状态 */
void freshSmartConfig(lv_task_t *task);

/* 创建LVGL任务 */
void createTasks();

#endif