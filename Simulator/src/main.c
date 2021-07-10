/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "lvgl.h"
#include "app_hal.h"
#include "png_decoder.h"

lv_obj_t *label_time;
lv_obj_t *label_temp;
lv_obj_t *label_humidity;
lv_obj_t *cont_time;
lv_obj_t *cont_alarm;
lv_obj_t *cont_date;
lv_obj_t *roller_time[5];
lv_obj_t *roller_alarm[5];
lv_obj_t *label_date[3];

lv_obj_t *tile_main;
lv_obj_t *page_home;
lv_obj_t *page_setting;
lv_obj_t *page_setting_wifi;
lv_obj_t *page_smart_config;
lv_obj_t *drop_wifi;

lv_obj_t *img_ok;

lv_obj_t *page_alarm;
lv_obj_t *page_alarm_setting;

extern lv_font_t digital;
extern lv_font_t digital_medium;
extern lv_font_t alphabet;
extern lv_font_t temp_font;
extern lv_font_t hanzi_25;

lv_style_t style_cont_time;
lv_style_t style_roller_time;

void createRollerTime()
{
  // 设置容器样式
  static lv_style_t style_cont_time;
  lv_style_init(&style_cont_time);
  lv_style_set_text_font(&style_cont_time, 0, &digital); // 设置字体
  lv_style_set_border_opa(&style_cont_time, 0, 0);       // 取消边框
  lv_style_set_bg_opa(&style_cont_time, 0, 0);           // 取消背景
  lv_style_set_pad_inner(&style_cont_time, 0, 5);        // 设置数字之间的间距

  // 设置滚轮样式
  static lv_style_t style_roller_time;
  lv_style_init(&style_roller_time);
  lv_style_set_border_opa(&style_roller_time, 0, 0); // 取消边框
  lv_style_set_bg_opa(&style_roller_time, 0, 0);     // 取消背景
  lv_style_set_pad_all(&style_roller_time, 0, 0);    //设置内边距

  // 创建容器
  cont_time = lv_cont_create(page_home, NULL);
  lv_cont_set_layout(cont_time, LV_LAYOUT_ROW_MID);           // 排列成一行
  lv_cont_set_fit(cont_time, LV_FIT_TIGHT);                   // 紧凑布局
  lv_obj_align_mid(cont_time, NULL, LV_ALIGN_CENTER, -70, 0); //调整位置
  lv_obj_add_style(cont_time, 0, &style_cont_time);           // 应用容器样式

  // 创建滚轮
  for (int i = 0; i < 5; i++)
  {
    roller_time[i] = lv_roller_create(cont_time, NULL);
    if (i == 0)
    {
      // 第一位只有0~2
      lv_roller_set_options(roller_time[i], "0\n1\n2", LV_ROLLER_MODE_NORMAL);
    }
    else if (i == 2)
    {
      // 第三位是冒号
      lv_roller_set_options(roller_time[i], ":", LV_ROLLER_MODE_NORMAL);
    }
    else if (i == 3)
    {
      // 第四位只有0~6
      lv_roller_set_options(roller_time[i], "0\n1\n2\n3\n4\n5", LV_ROLLER_MODE_NORMAL);
    }
    else
    {
      // 其他位为0~9
      lv_roller_set_options(roller_time[i], "0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_NORMAL);
    }

    lv_roller_set_visible_row_count(roller_time[i], 1); // 仅选中的行可见
    lv_roller_set_anim_time(roller_time[i], 1000);      // 设置动画切换时间
    lv_obj_set_height(roller_time[i], 65);              // 设置滚轮高度
    lv_roller_set_fix_width(roller_time[i], 30);        //设置滚轮宽度

    // 应用滚轮样式
    lv_obj_add_style(roller_time[i], LV_ROLLER_PART_BG, &style_roller_time);
    lv_obj_add_style(roller_time[i], LV_ROLLER_PART_SELECTED, &style_roller_time);
  }

  lv_roller_set_fix_width(roller_time[2], 10); // 冒号窄一点
}

void createLabelTime()
{
  static lv_style_t style_time;
  lv_style_init(&style_time);
  lv_style_set_text_font(&style_time, 0, &digital_medium);
  lv_style_set_text_color(&style_time, 0, LV_COLOR_WHITE);

  label_time = lv_label_create(page_home, NULL);
  lv_label_set_text(label_time, "00");
  lv_obj_add_style(label_time, 0, &style_time);
  // lv_label_set_align(label_time, LV_LABEL_ALIGN_CENTER);
  lv_obj_align_mid(label_time, cont_time, LV_ALIGN_IN_RIGHT_MID, 8, 14);
}

void createLabelDate()
{
  static lv_style_t style_cont_date;
  lv_style_init(&style_cont_date);
  lv_style_set_text_font(&style_cont_date, 0, &alphabet);
  lv_style_set_border_opa(&style_cont_date, 0, 0);
  lv_style_set_bg_opa(&style_cont_date, 0, 0);
  lv_style_set_pad_inner(&style_cont_date, 0, 25);

  cont_date = lv_cont_create(page_home, NULL);
  lv_cont_set_layout(cont_date, LV_LAYOUT_ROW_MID);
  lv_cont_set_fit(cont_date, LV_FIT_TIGHT);
  lv_obj_add_style(cont_date, 0, &style_cont_date);
  lv_obj_align_mid(cont_date, NULL, LV_ALIGN_CENTER, -30, 45);

  for (int i = 0; i < 3; i++)
  {
    label_date[i] = lv_label_create(cont_date, NULL);
    // lv_label_set_align(label_date[i], LV_LABEL_ALIGN_CENTER);
  }

  lv_label_set_text(label_date[0], "JAN");
  lv_label_set_text(label_date[1], "13");
  lv_label_set_text(label_date[2], "SUN");
}

void createWatchFace()
{
  lv_obj_t *img_base = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(img_base, "base_face.png");

  lv_obj_t *img_wifi = lv_img_create(page_home, NULL);
  lv_img_set_src(img_wifi, "wifi1.png");
  lv_obj_align_mid(img_wifi, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 70, -40);
}

void createWeatherLive()
{
  lv_obj_t *img_weather = lv_img_create(page_home, NULL);
  lv_img_set_src(img_weather, "weather/16tedabaoyu.png");
  lv_obj_align_mid(img_weather, NULL, LV_ALIGN_OUT_BOTTOM_MID, 3, -35);

  lv_obj_t *img_temp = lv_img_create(page_home, NULL);
  lv_img_set_src(img_temp, "weather/qiwen-gaowen.png");
  lv_obj_align_mid(img_temp, NULL, LV_ALIGN_IN_TOP_RIGHT, -70, 40);
  lv_obj_t *img_huminity = lv_img_create(page_home, NULL);
  lv_img_set_src(img_huminity, "weather/shidu.png");
  lv_obj_align_mid(img_huminity, NULL, LV_ALIGN_IN_TOP_LEFT, 80, 35);

  label_temp = lv_label_create(page_home, NULL);
  lv_label_set_text(label_temp, "30℃");
  // lv_label_set_align(label_temp, LV_LABEL_ALIGN_CENTER);
  lv_obj_set_style_local_text_font(label_temp, 0, 0, &temp_font);
  lv_obj_align_mid(label_temp, img_temp, LV_ALIGN_CENTER, -7, 25);

  label_humidity = lv_label_create(page_home, NULL);
  lv_label_set_text(label_humidity, "97");
  // lv_label_set_align(label_humidity, LV_LABEL_ALIGN_CENTER);
  lv_obj_set_style_local_text_font(label_humidity, 0, 0, &temp_font);
  lv_obj_align_mid(label_humidity, label_temp, LV_ALIGN_CENTER, -83, 0);
}

void createTile()
{
  static lv_point_t valid_pos[] = {{0, 0}, {0, 1}, {0, 2}, {1, 1}, {2, 1}, {1, 2}};
  tile_main = lv_tileview_create(lv_scr_act(), NULL);
  lv_tileview_set_valid_positions(tile_main, valid_pos, 6);
  lv_tileview_set_edge_flash(tile_main, true);

  static lv_style_t style_tile;
  lv_style_init(&style_tile);
  lv_style_set_bg_opa(&style_tile, 0, 0);
  lv_obj_add_style(tile_main, LV_TILEVIEW_PART_SCROLLBAR, &style_tile);

  page_home = lv_obj_create(tile_main, NULL);
  lv_obj_set_size(page_home, LV_HOR_RES, LV_VER_RES);
  lv_tileview_add_element(tile_main, page_home);
  lv_obj_set_style_local_border_side(page_home, 0, 0, 0);
  // 设置背景为黑色
  lv_obj_set_style_local_bg_color(page_home, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  // 取消圆角
  lv_obj_set_style_local_radius(page_home, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
  // 设置字体为白色
  lv_obj_set_style_local_text_color(page_home, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
}

lv_obj_t *kb;

void createKeyboard(lv_obj_t *ta)
{
  kb = lv_keyboard_create(lv_scr_act(), NULL);
  lv_obj_set_size(kb, LV_HOR_RES, LV_VER_RES / 2.5);
  lv_keyboard_set_cursor_manage(kb, true);
  lv_keyboard_set_textarea(kb, ta);
  lv_obj_align(kb, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
}

void ta_cb(lv_obj_t *ta, lv_event_t event)
{
  if (event == LV_EVENT_FOCUSED && kb == NULL)
  {
    createKeyboard(ta);
  }
  if (event == LV_EVENT_DEFOCUSED && kb != NULL)
  {
    lv_keyboard_set_textarea(kb, NULL);
    lv_obj_del(kb);
    kb = NULL;
  }
}

void createPageSetting()
{
  page_setting = lv_obj_create(tile_main, page_home);
  lv_obj_set_pos(page_setting, 0, LV_VER_RES);
  lv_tileview_add_element(tile_main, page_setting);
  lv_obj_t *img_setting = lv_img_create(page_setting, NULL);
  lv_img_set_src(img_setting, "setting.png");
  lv_obj_align(img_setting, page_setting, LV_ALIGN_CENTER, 0, 10);

  lv_obj_t *label_setting = lv_label_create(page_setting, NULL);
  lv_obj_set_style_local_text_font(label_setting, 0, 0, &hanzi_25);
  lv_label_set_text(label_setting, "设 置");
  lv_obj_align(label_setting, img_setting, LV_ALIGN_OUT_TOP_MID, 0, -10);

  page_setting_wifi = lv_obj_create(tile_main, page_home);
  lv_obj_set_size(page_setting_wifi, LV_HOR_RES, LV_VER_RES);
  lv_obj_set_pos(page_setting_wifi, LV_HOR_RES, LV_VER_RES);
  lv_obj_set_style_local_border_side(page_setting_wifi, 0, 0, 0);
  lv_tileview_add_element(tile_main, page_setting_wifi);

  drop_wifi = lv_dropdown_create(page_setting_wifi, NULL);
  // lv_dropdown_clear_options(drop_wifi); // 清空下拉选项
  lv_obj_set_size(drop_wifi, 100, 30);
  lv_obj_align(drop_wifi, page_setting_wifi, LV_ALIGN_CENTER, 0, -20);

  lv_obj_t *label_ssid = lv_label_create(page_setting_wifi, NULL);
  lv_obj_set_style_local_text_font(label_ssid, 0, 0, &hanzi_25);
  lv_label_set_text(label_ssid, "WiFi: ");
  lv_obj_align(label_ssid, drop_wifi, LV_ALIGN_OUT_LEFT_MID, 0, 0);
  lv_obj_t *btn_scan = lv_btn_create(page_setting_wifi, NULL);
  lv_obj_t *label_scan = lv_label_create(btn_scan, NULL);
  lv_obj_set_style_local_text_font(label_scan, 0, 0, &hanzi_25);
  lv_label_set_text(label_scan, "扫描");
  lv_obj_set_size(btn_scan, 50, 30);
  lv_obj_align(btn_scan, drop_wifi, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

  lv_obj_t *ta_pwd = lv_textarea_create(page_setting_wifi, NULL);
  lv_obj_set_event_cb(ta_pwd, ta_cb);
  lv_textarea_set_text(ta_pwd, "");
  lv_textarea_set_pwd_mode(ta_pwd, true);
  lv_textarea_set_one_line(ta_pwd, true);
  lv_textarea_set_cursor_hidden(ta_pwd, true);
  lv_obj_set_size(ta_pwd, 100, 30);
  lv_obj_align(ta_pwd, drop_wifi, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_obj_t *label_pwd = lv_label_create(page_setting_wifi, NULL);
  lv_obj_set_style_local_text_font(label_pwd, 0, 0, &hanzi_25);
  lv_label_set_text(label_pwd, "密码: ");
  lv_obj_align(label_pwd, ta_pwd, LV_ALIGN_OUT_LEFT_MID, 0, 0);
  lv_obj_t *btn_connect = lv_btn_create(page_setting_wifi, NULL);
  lv_obj_t *label_connect = lv_label_create(btn_connect, NULL);
  lv_obj_set_style_local_text_font(label_connect, 0, 0, &hanzi_25);
  lv_label_set_text(label_connect, "连接");
  lv_obj_set_size(btn_connect, 50, 30);
  lv_obj_align(btn_connect, ta_pwd, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

  lv_obj_t *label_wifi = lv_label_create(page_setting_wifi, NULL);
  lv_obj_set_style_local_text_font(label_wifi, 0, 0, &hanzi_25);
  lv_label_set_text(label_wifi, "连接WiFi");
  lv_obj_align(label_wifi, drop_wifi, LV_ALIGN_OUT_TOP_MID, 0, -10);
}

void createPageSmartConfig()
{
  page_smart_config = lv_obj_create(tile_main, page_home);
  lv_obj_set_pos(page_smart_config, 2 * LV_HOR_RES, LV_VER_RES);
  lv_tileview_add_element(tile_main, page_smart_config);

  lv_obj_t *img_smart_config = lv_img_create(page_smart_config, NULL);
  lv_img_set_src(img_smart_config, "smart_config.png");
  lv_obj_align(img_smart_config, page_smart_config, LV_ALIGN_CENTER, 0, 10);

  lv_obj_t *label_smart_config = lv_label_create(page_smart_config, NULL);
  lv_obj_set_style_local_text_font(label_smart_config, 0, 0, &hanzi_25);
  lv_label_set_text(label_smart_config, "扫码连接WiFi");
  lv_obj_align(label_smart_config, img_smart_config, LV_ALIGN_OUT_TOP_MID, 0, 0);

  lv_obj_t *spinner_smart_config = lv_spinner_create(page_smart_config, NULL);
  lv_obj_set_size(spinner_smart_config, 40, 40);
  lv_spinner_set_type(spinner_smart_config, LV_SPINNER_TYPE_FILLSPIN_ARC);
  lv_obj_align(spinner_smart_config, img_smart_config, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
  lv_obj_set_style_local_line_width(spinner_smart_config, LV_SPINNER_PART_BG, LV_STATE_DEFAULT, 8);
  lv_obj_set_style_local_line_width(spinner_smart_config, LV_SPINNER_PART_INDIC, LV_STATE_DEFAULT, 8);

  img_ok = lv_img_create(page_smart_config, NULL);
  lv_img_set_src(img_ok, "ok.png");
  lv_obj_align(img_ok, img_smart_config, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
}

void createPageAlarm()
{
  page_alarm = lv_obj_create(tile_main, page_home);
  lv_obj_set_pos(page_alarm, 0, 2 * LV_VER_RES);
  lv_tileview_add_element(tile_main, page_alarm);
  lv_obj_t *img_alarm = lv_img_create(page_alarm, NULL);
  lv_img_set_src(img_alarm, "alarm.png");
  lv_obj_align(img_alarm, page_alarm, LV_ALIGN_CENTER, 0, 10);

  lv_obj_t *label_alarm = lv_label_create(page_alarm, NULL);
  lv_obj_set_style_local_text_font(label_alarm, 0, 0, &hanzi_25);
  lv_label_set_text(label_alarm, "闹 钟");
  lv_obj_align(label_alarm, img_alarm, LV_ALIGN_OUT_TOP_MID, 0, -10);
}

static void roller_alarm_cb(lv_obj_t *roller, lv_event_t event)
{
  if (event == LV_EVENT_VALUE_CHANGED)
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
  page_alarm_setting = lv_obj_create(tile_main, page_home);
  lv_obj_set_pos(page_alarm_setting, LV_HOR_RES, 2 * LV_VER_RES);
  lv_tileview_add_element(tile_main, page_alarm_setting);
  cont_alarm = lv_cont_create(page_alarm_setting, cont_time);
  for (int i = 0; i < 5; i++)
  {
    roller_alarm[i] = lv_roller_create(cont_alarm, roller_time[i]);
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
}

int main(void)
{
  lv_init();
  png_decoder_init();
  hal_setup();
  createTile();
  createWatchFace();
  createRollerTime();
  createLabelTime();
  createWeatherLive();
  createLabelDate();
  createPageSetting();
  createPageSmartConfig();
  createPageAlarm();
  createPageAlarmSetting();
  hal_loop();
}
