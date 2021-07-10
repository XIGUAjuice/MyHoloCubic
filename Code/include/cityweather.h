#ifndef CITYWEATHER_H
#define CITYWEATHER_H


#include <network.h>
#include <global.h>
#include <sd_card.h>
#include <esp_wifi.h>
#include<fresh_task.h>

void createPageWeather();

/* 刷新天气 */
void freshCityWeather();

#endif