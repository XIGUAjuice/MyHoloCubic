#ifndef WEATHER_PAGE_H
#define WEATHER_PAGE_H

#include <lvgl.h>
#include <control.h>
#include <network.h>

int max_temp(int *temp);
int min_temp(int *temp);
void display_weather_icon(void);
void display_temperature_chart(void);
void display_temperature_label(void);
void fresh_weather_icon(void);
void fresh_temperature_chart(void);
void fresh_temperature_label(void);

#endif