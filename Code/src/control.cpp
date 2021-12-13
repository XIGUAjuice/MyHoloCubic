#include <control.h>

lv_obj_t *tile_main;
lv_obj_t *page_home;
lv_obj_t *page_weather;
lv_obj_t *page_setting;
lv_obj_t *page_setting_wifi;
lv_obj_t *drop_wifi;
lv_obj_t *text_area_pwd;
lv_obj_t *keyboard;
lv_obj_t *btn_scan;
lv_obj_t *btn_connect;

lv_obj_t *img_wifi;
LV_IMG_DECLARE(base_face);

lv_obj_t *roller_time[5];
lv_obj_t *cont_time;

lv_obj_t *label_time;

lv_obj_t *cont_date;
lv_obj_t *label_date[3];

lv_obj_t *img_weather;
lv_obj_t *img_temp;
lv_obj_t *label_temp;
lv_obj_t *label_humidity;

lv_obj_t *page_smart_config;
lv_obj_t *spinner_smart_config;
lv_obj_t *img_ok;

lv_obj_t *page_alarm;
lv_obj_t *page_alarm_setting;

/* 磁贴回调函数 */
static void tile_event_cb(lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_KEY:
        uint32_t *key = (uint32_t *)lv_event_get_data();
        lv_coord_t x, y;
        static bool wifi_in_group = false;
        static bool smart_config_start = false;
        static bool alarm_in_group = false;

        /*************************** 控制页面切换 ***************************/
        lv_tileview_get_tile_act(tile_main, &x, &y); //获取当前页面坐标
        if (*key == LV_KEY_DOWN)
        {
            /* 向下切换页面 */
            lv_tileview_set_tile_act(tile_main, x, y + 1, LV_ANIM_ON);
        }
        else if (*key == LV_KEY_UP)
        {
            /* 向上切换页面 */
            lv_tileview_set_tile_act(tile_main, x, y - 1, LV_ANIM_ON);
        }
        else if (*key == LV_KEY_RIGHT)
        {
            /* 向右切换页面 */
            lv_tileview_set_tile_act(tile_main, x + 1, y, LV_ANIM_ON);
        }
        else if (*key == LV_KEY_LEFT)
        {
            /* 向左切换页面 */
            lv_tileview_set_tile_act(tile_main, x - 1, y, LV_ANIM_ON);
        }

        /************************** 控制特殊页面的事件 ****************************/

        // 在WiFi设置页面将各个控件加入控制组
        lv_tileview_get_tile_act(tile_main, &x, &y); //获取当前页面坐标

        if (x == 1 && y == 1 && !wifi_in_group)
        {
            Serial.println("将Wifi相关控件加入控制组");
            lv_group_add_obj(group_tile, drop_wifi);
            lv_group_add_obj(group_tile, btn_connect);
            lv_group_add_obj(group_tile, btn_scan);
            lv_group_add_obj(group_tile, text_area_pwd);
            wifi_in_group = true;
        }
        // 退出WiFi设置页面后将各个控件移出控制组
        else if ((x != 1 || y != 1) && wifi_in_group)
        {
            Serial.println("将Wifi相关控件移出控制组");
            lv_group_remove_obj(drop_wifi);
            lv_group_remove_obj(btn_connect);
            lv_group_remove_obj(btn_scan);
            lv_group_remove_obj(text_area_pwd);
            wifi_in_group = false;
        }

        // 在闹钟设置页面将roller加入控制组
        if (x == 1 && y == 2 && !alarm_in_group)
        {
            Serial.println("将闹钟相关控件加入控制组");
            for (int i = 0; i < 5; i++)
            {
                if (i != 2)
                    lv_group_add_obj(group_tile, roller_alarm[i]);
            }
            alarm_in_group = true;

            // 在设置闹钟期间关闭闹钟
            Serial.println("暂时关闭闹钟功能");
            my_alarm.alarmEnable(false);
        }
        // 退出闹钟设置页面后将roller移出控制组
        else if ((x != 1 || y != 2) && alarm_in_group)
        {
            Serial.println("将闹钟相关控件移出控制组");
            for (int i = 0; i < 5; i++)
            {
                if (i != 2)
                    lv_group_add_obj(group_tile, roller_alarm[i]);
            }
            alarm_in_group = false;

            // 重新打开闹钟功能
            Serial.println("打开闹钟功能");
            my_alarm.alarmEnable(true);

            // 将设置的闹钟时间写入
            my_alarm.setAlarm(roller_alarm);
        }

        // 在闹钟响时，任意动作打断闹钟
        if (my_alarm.getAlarmOn())
        {
            my_alarm.stopAlarm();
        }

        // 在扫码配网页面打开SmartConfig以及启用相关task
        if (x == 2 && y == 1 && !smart_config_start)
        {
            Serial.println("开启SmartConfig");
            smart_config_start = true;
            WiFi.beginSmartConfig();
        }
        // 退出扫码配网页面后关闭SmartConfig以及关闭相关task
        else if ((x != 2 || y != 1) && smart_config_start)
        {
            Serial.println("关闭SmartConfig");
            smart_config_start = false;
            WiFi.stopSmartConfig();
        }
        break;
    }
}

void createTile()
{
    // 设置磁贴样式
    static lv_style_t style_tile;
    lv_style_init(&style_tile);
    lv_style_set_bg_opa(&style_tile, 0, 0); // 取消背景

    // 创建磁贴模块
    tile_main = lv_tileview_create(lv_scr_act(), NULL);

    // 设置可达区域
    static lv_point_t valid_pos[] = {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {1, 1}, {2, 1}, {0, 2}, {1, 2}};
    lv_tileview_set_valid_positions(tile_main, valid_pos, 8);
    // static lv_point_t valid_pos[] = {{0, 0}};
    // lv_tileview_set_valid_positions(tile_main, valid_pos, 1);
    // 设置边缘特效
    lv_tileview_set_edge_flash(tile_main, true);

    // 应用磁贴样式，取消滚动条
    lv_obj_add_style(tile_main, LV_TILEVIEW_PART_SCROLLBAR, &style_tile);

    // 设置磁贴回调函数
    lv_obj_set_event_cb(tile_main, tile_event_cb);

    // 创建主页面
    page_home = lv_obj_create(tile_main, NULL);
    lv_obj_set_size(page_home, LV_HOR_RES, LV_VER_RES);     //调整为整个屏幕大小
    lv_obj_set_style_local_border_side(page_home, 0, 0, 0); // 取消边框
    lv_tileview_add_element(tile_main, page_home);          //将主页面固定至磁贴
    // 设置背景为黑色
    lv_obj_set_style_local_bg_color(page_home, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    // 取消圆角
    lv_obj_set_style_local_radius(page_home, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    // 设置字体为白色
    lv_obj_set_style_local_text_color(page_home, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // 创建设置页面
    page_setting = lv_obj_create(tile_main, page_home);
    lv_obj_set_pos(page_setting, 0, LV_VER_RES);      // 位置为第二行，第一列
    lv_tileview_add_element(tile_main, page_setting); // 将设置页面固定至磁贴

    // 创建WiFi设置页面
    page_setting_wifi = lv_obj_create(tile_main, page_home);
    lv_obj_set_pos(page_setting_wifi, LV_HOR_RES, LV_VER_RES); // 位置为第二行，第二列
    lv_tileview_add_element(tile_main, page_setting_wifi);     // 将WiFi设置页面固定至磁贴

    // 创建扫码连接WiFi页面
    page_smart_config = lv_obj_create(tile_main, page_home);
    lv_obj_set_pos(page_smart_config, 2 * LV_HOR_RES, LV_VER_RES); // 位置为第二行，第三列
    lv_tileview_add_element(tile_main, page_smart_config);         // 将扫码页面固定至磁贴

    // 创建闹钟页面
    page_alarm = lv_obj_create(tile_main, page_home);
    lv_obj_set_pos(page_alarm, 0, 2 * LV_VER_RES);  // 位置为第三行，第一列
    lv_tileview_add_element(tile_main, page_alarm); // 将闹钟页面固定至磁贴

    // 创建闹钟设置页面
    page_alarm_setting = lv_obj_create(tile_main, page_home);
    lv_obj_set_pos(page_alarm_setting, LV_HOR_RES, 2 * LV_VER_RES); // 位置为第三行，第二列
    lv_tileview_add_element(tile_main, page_alarm_setting);         // 将闹钟设置页面固定至磁贴

    // 创建天气预报页面
    page_weather = lv_obj_create(tile_main, page_home);
    lv_obj_set_pos(page_weather, LV_HOR_RES, 0);      // 位置为第一行，第二列
    lv_tileview_add_element(tile_main, page_weather); // 将天气页面固定至磁贴

    // 创建城市天气页面
    page_multi_weather = lv_obj_create(tile_main, page_home);
    lv_obj_set_pos(page_multi_weather, 2 * LV_HOR_RES, 0);
    lv_tileview_add_element(tile_main, page_multi_weather);
}