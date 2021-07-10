#include <fresh_task.h>

lv_task_t *task_fresh_weather;
lv_task_t *task_fresh_info;
lv_task_t *task_fresh_smart_config;
lv_task_t *task_fresh_alarm;
lv_task_t *task_fresh_city_weather;
lv_task_t *task_fresh_weather_forecast;

void getWeatherIcon(String weather, int h, char *icon)
{
    String icon_path;
    if (weather == "晴" && h <= 18 && h >= 6)
    {
        icon_path = "S:/bin/baitianqing.bin";
    }
    else if (weather == "晴")
    {
        icon_path = "S:/bin/yejianqing.bin";
    }
    else if (weather.indexOf("云") != -1 && h <= 18 && h >= 6)
    {
        icon_path = "S:/bin/baitianduoyun.bin";
    }
    else if (weather.indexOf("云") != -1)
    {
        icon_path = "S:/bin/yejianduoyun.bin";
    }
    else if (weather == "阴")
    {
        icon_path = "S:/bin/yin.bin";
    }
    else if (weather.indexOf("雷阵雨") != -1)
    {
        icon_path = "S:/bin/leizhenyu.bin";
    }
    else if (weather.indexOf("阵雨") != -1 && h <= 18 && h >= 6)
    {
        icon_path = "S:/bin/baitianzhenyu.bin";
    }
    else if (weather.indexOf("阵雨") != -1)
    {
        icon_path = "S:/bin/yejianzhenyu.bin";
    }
    else if (weather.indexOf("小雨") != -1)
    {
        icon_path = "S:/bin/xiaoyu.bin";
    }
    else if (weather.indexOf("中雨") != -1)
    {
        icon_path = "S:/bin/zhongyu.bin";
    }
    else if (weather.indexOf("大雨") != -1)
    {
        icon_path = "S:/bin/dayu.bin";
    }
    else if (weather.indexOf("大暴雨") != -1)
    {
        icon_path = "S:/bin/tedabaoyu.bin";
    }
    else if (weather.indexOf("暴雨") != -1)
    {
        icon_path = "S:/bin/dabaoyu.bin";
    }
    else if (weather.indexOf("雨夹雪") != -1)
    {
        icon_path = "S:/bin/yujiaxue.bin";
    }
    else if (weather.indexOf("暴雪") != -1)
    {
        icon_path = "S:/bin/baoxue.bin";
    }
    else if (weather.indexOf("大雪") != -1)
    {
        icon_path = "S:/bin/daxue.bin";
    }
    else if (weather.indexOf("暴雪") != -1)
    {
        icon_path = "S:/bin/baoxue.bin";
    }
    else if (weather.indexOf("雪") != -1 && h <= 18 && h >= 6)
    {
        icon_path = "S:/bin/baitianzhenxue.bin";
    }
    else if (weather.indexOf("雪") != -1)
    {
        icon_path = "S:/bin/yejianzhenxue.bin";
    }
    else if (weather.indexOf("雾") != -1 && h <= 18 && h >= 6)
    {
        icon_path = "S:/bin/baitianwu.bin";
    }
    else if (weather.indexOf("雾") != -1)
    {
        icon_path = "S:/bin/yejianwu.bin";
    }
    else if (weather.indexOf("霾") != -1)
    {
        icon_path = "S:/bin/wumai.bin";
    }
    icon_path.toCharArray(icon, 30);
}

/* 刷新日期 */
void freshDate(time_t time_now, int &prev_day)
{
    // 每天刷新日期
    int day_now = day(time_now);

    if (day_now != prev_day)
    {
        prev_day = day_now;
        int month_now = month(time_now);
        int weekday_now = weekday(time_now);

        lv_label_set_text(label_date[0], monthShortStr(month_now));
        lv_label_set_text(label_date[1], String(day_now).c_str());
        lv_label_set_text(label_date[2], dayShortStr(weekday_now));
    }
}

/* 刷新时间 */
void freshTime(time_t time_now, int &prev_second)
{
    // 每秒刷新时间
    int s = second(time_now);
    if (s != prev_second)
    {
        prev_second = s;
        int m = minute(time_now);
        int h = hour(time_now);

        static uint8_t time_bit_old[5] = {};

        int time_bit[] = {h / 10, h % 10, ':', m / 10, m % 10}; // hour_high, hour_low, 冒号, minute_high, minute_low
        int roller_index[] = {0, 1, 3, 4};

        lv_label_set_text_fmt(label_time, "%02d", s);
        for (uint8_t i : roller_index)
        {
            if (time_bit[i] != time_bit_old[i])
            {
                lv_roller_set_selected(roller_time[i], time_bit[i], LV_ANIM_ON);
                time_bit_old[i] = time_bit[i];
            }
        }
    }
}

/* 刷新天气 */
void freshWeather(lv_task_t *task)
{
    // 每五分钟刷新天气
    time_t time_now = now(); //获取时间
    int h = hour(time_now);  // 区分白天和晚上

    // 获取实时天气
    JsonObject weather_live = net.getWeatherLive(net.getCityCode());
    if (!weather_live.isNull())
    {
        Serial.println("更新天气成功");

        int temp = weather_live["temperature"];
        if (temp > 25)
        {
            lv_img_set_src(img_temp, "S:/bin/gaowen.bin");
        }
        if (temp < 10)
        {
            lv_img_set_src(img_temp, "S:/bin/diwen.bin");
        }
        else
        {
            lv_img_set_src(img_temp, "S:/bin/changwen.bin");
        }
        lv_label_set_text(label_temp, (String(temp) + "℃").c_str());
        lv_label_set_text(label_humidity, weather_live["humidity"]);

        String weather = weather_live["weather"];
        char weather_c_str[30];
        getWeatherIcon(weather, h, weather_c_str);
        lv_img_set_src(img_weather, weather_c_str);
    }
    else
    {
        Serial.println("更新天气失败");
    }
}

/* 刷新WiFi连接情况 */
void freshWifiConnect()
{
    wl_status_t status = net.checkWifi();
    if (status != WL_IDLE_STATUS)
    {
        if (status == WL_CONNECTED)
        {
            lv_img_set_src(img_wifi, "S:bin/wifi1.bin");
            net.initNtp();
            Serial.println("打开天气更新");
            lv_task_set_prio(task_fresh_weather, LV_TASK_PRIO_MID);
            lv_task_ready(task_fresh_weather);
            Serial.println("关闭SmartConfig更新");
            lv_task_set_prio(task_fresh_smart_config, LV_TASK_PRIO_OFF);
        }
        else
        {
            lv_img_set_src(img_wifi, "S:bin/wifi0.bin");
            net.stopNtp();
            Serial.println("关闭天气更新");
            lv_task_set_prio(task_fresh_weather, LV_TASK_PRIO_OFF);
        }
    }
}

void freshSmartConfig(lv_task_t *task)
{
    // 显示加载圈
    if (!WiFi.smartConfigDone())
    {
        lv_obj_set_hidden(spinner_smart_config, false);
        lv_obj_set_hidden(img_ok, true);
    }
    // 关闭加载圈
    else if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WiFi连接成功");
        lv_obj_set_hidden(spinner_smart_config, true);
        lv_obj_set_hidden(img_ok, false);

        // 读取WiFi配置信息
        wifi_config_t conf;
        esp_wifi_get_config(WIFI_IF_STA, &conf);

        // 将WiFi配置信息写入Json
        DynamicJsonDocument wifi_doc(100);
        wifi_doc["ssid"] = conf.sta.ssid;
        wifi_doc["password"] = conf.sta.password;

        // 将Json写入sd卡中
        sd.writeJson("/wifi.json", wifi_doc);

        lv_task_ready(task_fresh_city_weather);
        lv_task_ready(task_fresh_weather_forecast);
        net.setCityCode();
    }
}

/* 刷新日期时间 */
void freshInfo(lv_task_t *task)
{
    static int prev_second = -1;
    static int prev_day = -1;
    time_t time_now = now();

    freshWifiConnect();
    freshDate(time_now, prev_day);
    freshTime(time_now, prev_second);
}

/* 刷新闹钟 */
void freshAlarm(lv_task_t *task)
{
    time_t time_now = now();
    my_alarm.checkAlarm(time_now);
    if (my_alarm.getAlarmOn())
    {
        my_alarm.alarmBreathe();
    }
}

/* 刷新天气预报 */
void fresh_weather_forecast(lv_task_t *task)
{
    fresh_weather_icon();
    fresh_temperature_chart();
    fresh_temperature_label();
}

/* 刷新城市天气 */
void freshCityWeather(lv_task_t *task)
{
    // 每五分钟刷新天气
    time_t time_now = now(); //获取时间
    int h = hour(time_now);  // 区分白天和晚上

    // 获取实时天气
    JsonObject weather_live = net.getWeatherLive("440106");
    if (!weather_live.isNull())
    {
        Serial.println("更新天气成功");

        int temp = weather_live["temperature"];

        lv_label_set_text(label_temp_city1, (String(temp) + "℃").c_str());

        String weather = weather_live["weather"];
        char icon[30];
        getWeatherIcon(weather, h, icon);
        lv_img_set_src(img_weather1, icon);
    }
    else
    {
        Serial.println("更新天气失败");
    }

    // 获取实时天气
    weather_live = net.getWeatherLive("440300");
    if (!weather_live.isNull())
    {
        Serial.println("更新天气成功");

        int temp = weather_live["temperature"];

        lv_label_set_text(label_temp_city2, (String(temp) + "℃").c_str());

        String weather = weather_live["weather"];
        char icon[30];
        getWeatherIcon(weather, h, icon);
        lv_img_set_src(img_weather2, icon);
    }
    else
    {
        Serial.println("更新天气失败");
    }

    // 获取实时天气
    weather_live = net.getWeatherLive("330000");
    if (!weather_live.isNull())
    {
        Serial.println("更新天气成功");

        int temp = weather_live["temperature"];

        lv_label_set_text(label_temp_city3, (String(temp) + "℃").c_str());

        String weather = weather_live["weather"];
        char icon[30];
        getWeatherIcon(weather, h, icon);
        lv_img_set_src(img_weather3, icon);
    }
    else
    {
        Serial.println("更新天气失败");
    }
}

/* 创建任务 */
void createTasks()
{
    // 每5分钟更新天气
    task_fresh_weather = lv_task_create(freshWeather, 1000 * 60 * 5, LV_TASK_PRIO_MID, NULL);

    // 每20ms刷新时间
    task_fresh_info = lv_task_create(freshInfo, 20, LV_TASK_PRIO_HIGH, NULL);

    // 每20ms检测SmartConfig状态，默认关闭
    task_fresh_smart_config = lv_task_create(freshSmartConfig, 20, LV_TASK_PRIO_OFF, NULL);

    // 每20ms检测闹钟状态
    task_fresh_alarm = lv_task_create(freshAlarm, 20, LV_TASK_PRIO_MID, NULL);

    // 每10分钟更新城市天气
    task_fresh_city_weather = lv_task_create(freshCityWeather, 1000 * 60 * 10, LV_TASK_PRIO_MID, NULL);
    lv_task_ready(task_fresh_city_weather);

    // 每6分钟刷新天气预报
    task_fresh_weather_forecast = lv_task_create(fresh_weather_forecast, 1000 * 60 * 6, LV_TASK_PRIO_MID, NULL);
}
