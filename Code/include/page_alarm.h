#ifndef ALARM_H
#define ALARM_H

#include <global.h>
#include <LED.h>
#include <ArduinoJson.h>
#include <sd_card.h>

/* 创建闹钟页面 */
void createPageAlarm();

/* 创建闹钟设置页面 */
void createPageAlarmSetting();

class Alarm
{
private:
    int hour_set;
    int min_set;
    int prev_minute;
    LED led;
    bool alarm_enable;
    bool alarm_on;

public:
    /* 闹钟初始化 */
    void init();

    /* 检查是否到达闹钟设置的时间 */
    void checkAlarm(time_t time_now);

    /* 设置闹钟时间 */
    void setAlarm(lv_obj_t **roller_alarm);

    /* 读取闹钟设置 */
    void readFromSd(lv_obj_t **roller_alarm);

    /* 获取闹钟状态 */
    bool getAlarmOn();

    /* 用呼吸灯亮代替闹钟，该函数需循环调用以改变颜色 */
    void alarmBreathe();

    /* 打断闹钟 */
    void stopAlarm();

    /* 启用/关闭 闹钟功能 */
    void alarmEnable(bool enable);
};

extern Alarm my_alarm;

#endif