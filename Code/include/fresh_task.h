#ifndef FRESH_TASK_H
#define FRESH_TASK_H

#include <home_page.h>
#include <network.h>
#include <global.h>
#include <sd_card.h>
#include <esp_wifi.h>
#include <page_alarm.h>
#include <weather_page.h>
#include <Arduino.h>

/* 获取天气图标 */
void getWeatherIcon(String weather, int h, char *icon);

/* 刷新日期 */
void freshDate(time_t time_now);

/* 刷新时间 */
void freshTime(time_t time_now);

/* 刷新天气 */
void freshWeather(time_t time_now);

/* 刷新天气预报 */
void fresh_weather_forecast();

/* 刷新城市天气 */
void freshCityWeather(time_t time_now);

/* 刷新WiFi连接情况 */
void freshWifiConnect();

/* 刷新Smart Config状态 */
void freshSmartConfig();

/* lvgl刷新任务 */
void freshLvgl(void *pvParameters);

/* 间隔较长的任务 */
void freshLongInterval(void *pvParameters);

/* 间隔较短的任务 */
void freshShortInterval(void *pvParameters);

/* 创建LVGL任务 */
void createTasks();

extern TaskHandle_t task_fresh__short_interval;
extern TaskHandle_t task_fresh_long_interval;
extern TaskHandle_t task_lvgl;

#endif