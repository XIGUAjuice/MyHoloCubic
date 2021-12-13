#include <page_alarm.h>

lv_obj_t *roller_alarm[5];
lv_obj_t *cont_alarm;
Alarm my_alarm;

void createPageAlarm()
{
    // 创建背景
    lv_obj_t *img_base = lv_img_create(page_alarm, NULL);
    lv_img_set_src(img_base, &base_face);

    // 创建闹钟图标
    lv_obj_t *img_alarm = lv_img_create(page_alarm, NULL);
    lv_img_set_src(img_alarm, "S:/bin/alarm.bin");               // 设置图标路径
    lv_obj_align(img_alarm, page_alarm, LV_ALIGN_CENTER, 0, 10); // 调整位置

    // 创建闹钟页面标题
    lv_obj_t *label_alarm = lv_label_create(page_alarm, NULL);
    lv_obj_set_style_local_text_font(label_alarm, 0, 0, &hanzi_25);     // 设置字体
    lv_label_set_text(label_alarm, "闹 钟");                            // 设置文本
    lv_obj_align(label_alarm, img_alarm, LV_ALIGN_OUT_TOP_MID, 0, -10); // 调整位置
}

/* roller的回调函数，当第一位为2时，第二位只能是0~3 (24小时格式) */
static void roller_alarm_cb(lv_obj_t *roller, lv_event_t event)
{
    if (event == LV_EVENT_KEY)
    {
        if (lv_roller_get_selected(roller) == 2)
        {
            lv_roller_set_options(roller_alarm[1], "0\n1\n2\n3", LV_ROLLER_MODE_INFINITE);
        }
        else
        {
            lv_roller_set_options(roller_alarm[1], "0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_INFINITE);
        }
    }
}

void createPageAlarmSetting()
{
    static lv_style_t style_roller_alarm;
    lv_style_init(&style_roller_alarm);
    lv_style_set_shadow_color(&style_roller_alarm, LV_STATE_FOCUSED, LV_COLOR_RED);
    lv_style_set_shadow_width(&style_roller_alarm, LV_STATE_FOCUSED, 10);
    lv_style_set_shadow_spread(&style_roller_alarm, LV_STATE_FOCUSED, 3);

    // 创建roller的容器，从cont_time复制
    cont_alarm = lv_cont_create(page_alarm_setting, cont_time);

    // 创建5个roller显示数字，从roller_time复制
    for (int i = 0; i < 5; i++)
    {
        roller_alarm[i] = lv_roller_create(cont_alarm, roller_time[i]);

        // 应用roller样式
        lv_obj_add_style(roller_alarm[i], LV_ROLLER_PART_BG, &style_roller_alarm);

        // 修改选项
        if (i == 0)
        {
            // 第一位只有0~2
            lv_roller_set_options(roller_alarm[i], "0\n1\n2", LV_ROLLER_MODE_INFINITE);
            // 第二位可能是0~9，也可能是0~3
            lv_obj_set_event_cb(roller_alarm[i], roller_alarm_cb);
        }
        else if (i == 2)
        {
            // 第三位是冒号
            lv_roller_set_options(roller_alarm[i], ":", LV_ROLLER_MODE_INFINITE);
        }
        else if (i == 3)
        {
            // 第四位只有0~6
            lv_roller_set_options(roller_alarm[i], "0\n1\n2\n3\n4\n5", LV_ROLLER_MODE_INFINITE);
        }
        else
        {
            // 其他位为0~9
            lv_roller_set_options(roller_alarm[i], "0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_INFINITE);
        }
    }
    lv_obj_align(cont_alarm, page_alarm_setting, LV_ALIGN_CENTER, 0, 0);

    my_alarm.init();                   // 初始化闹钟
    my_alarm.readFromSd(roller_alarm); // 读取闹钟设置
}

void Alarm::init()
{
    Serial.println("初始化Alarm");
    led.init();
    led.setBrightness(1);
    led.setColorBreath(50, 100, 50, 1, 2, 3);
}

void Alarm::checkAlarm(time_t time_now)
{
    int m = minute(time_now);
    if (m != prev_minute)
    {
        prev_minute = m;
        int h = hour(time_now);
        if (h == hour_set && m == min_set && alarm_enable)
        {
            Serial.println("闹钟设置时间到！");
            alarm_on = true;
        }
    }
}

void Alarm::setAlarm(lv_obj_t **roller_alarm)
{
    hour_set = lv_roller_get_selected(roller_alarm[0]) * 10 + lv_roller_get_selected(roller_alarm[1]);
    min_set = lv_roller_get_selected(roller_alarm[3]) * 10 + lv_roller_get_selected(roller_alarm[4]);

    // 将闹钟设置写入sd卡
    DynamicJsonDocument doc(50);
    doc["hour"] = hour_set;
    doc["minute"] = min_set;
    sd.writeJson("/alarm.json", doc);

    Serial.printf("闹钟设置为: %d : %d \n", hour_set, min_set);
}

/* 读取闹钟设置 */
void Alarm::readFromSd(lv_obj_t **roller_alarm)
{
    Serial.println("读取闹钟设置");
    DynamicJsonDocument doc(50);
    sd.readJson("/alarm.json", doc);
    int hour_set = doc["hour"];
    int min_set = doc["minute"];
    Serial.printf("读取闹钟设置为: %d : %d \n", hour_set, min_set);
    lv_roller_set_selected(roller_alarm[0], hour_set / 10, LV_ANIM_OFF);
    lv_roller_set_selected(roller_alarm[1], hour_set % 10, LV_ANIM_OFF);
    lv_roller_set_selected(roller_alarm[3], min_set / 10, LV_ANIM_OFF);
    lv_roller_set_selected(roller_alarm[4], min_set % 10, LV_ANIM_OFF);
}

bool Alarm::getAlarmOn()
{
    return alarm_on;
}

void Alarm::alarmBreathe()
{
    led.colorBreath();
}

void Alarm::stopAlarm()
{
    Serial.println("闹钟已关闭");
    led.setBrightness(0);
    alarm_on = false;
}

void Alarm::alarmEnable(bool enable)
{
    alarm_enable = enable;
}