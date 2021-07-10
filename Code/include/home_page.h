#ifndef HOME_PAGE_H
#define HOME_PAGE_H

#include <lvgl.h>
#include <control.h>

/* 创建表盘 */
void createWatchFace();
/* 创建时间模块（用来显示小时与分钟） */
void createRollerTime();
/* 创建时间标签（用来显示秒） */
void createLabelTime();
/* 创建日期标签 */
void createLabelDate();
/* 创建实时天气图标 */
void createWeatherLive();
/* 创建主页面 */
void createPageHome();

#endif