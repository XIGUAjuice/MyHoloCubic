#include <wifi_page.h>
#include <global.h>

/* 创建键盘 */
void createKeyboard(lv_obj_t *text_area)
{
    // 创建键盘
    keyboard = lv_keyboard_create(lv_scr_act(), NULL);
    lv_keyboard_set_cursor_manage(keyboard, true);              //自动设置光标闪烁
    lv_keyboard_set_textarea(keyboard, text_area);              //附加到输入框
    lv_obj_set_size(keyboard, LV_HOR_RES, LV_VER_RES / 2.5);    //改变大小
    lv_obj_align(keyboard, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0); // 调整位置
    lv_group_add_obj(group_tile, keyboard);                     // 将键盘加入组
    lv_group_focus_obj(keyboard);                               // 聚焦键盘
}

/* 输入框回调函数 */
static void text_area_cb(lv_obj_t *text_area, lv_event_t event)
{
    if (event == LV_EVENT_FOCUSED && keyboard == NULL)
    {
        /* 聚焦则创建键盘 */
        createKeyboard(text_area);
    }
    // if (event == LV_EVENT_DEFOCUSED && keyboard != NULL)
    // {
    //     /* 失去焦点则销毁键盘 */
    //     lv_group_remove_obj(keyboard);
    //     lv_keyboard_set_textarea(keyboard, NULL);
    //     lv_obj_del(keyboard);
    //     keyboard = NULL;
    // }
}

/* 扫描按钮回调函数 */
static void btn_sacn_cb(lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_PRESSED:
        Serial.println("scan start");
        int n = WiFi.scanNetworks();
        Serial.println("scan done");
        if (lv_dropdown_get_option_cnt(drop_wifi) != 0)
        {
            // lv_dropdown_clear_options(drop_wifi);
        }
        for (int i = 0; i < n; i++)
        {
            lv_dropdown_add_option(drop_wifi, WiFi.SSID(i).c_str(), i);
        }
        break;
    }
}

/* 连接按钮回调函数 */
static void btn_connect_cb(lv_obj_t *obj, lv_event_t event)
{
    switch (event)
    {
    case LV_EVENT_PRESSED:
        int wifi_selected = lv_dropdown_get_selected(drop_wifi);
        net.connectWifi(WiFi.SSID(wifi_selected), lv_textarea_get_text(text_area_pwd));
        break;
    }
}

void createPageWifiSetting()
{
    // 按钮样式，选中发光
    static lv_style_t style_focused;
    lv_style_init(&style_focused);
    lv_style_set_shadow_color(&style_focused, LV_STATE_FOCUSED, LV_COLOR_RED);
    lv_style_set_shadow_width(&style_focused, LV_STATE_FOCUSED, 10);
    lv_style_set_shadow_spread(&style_focused, LV_STATE_FOCUSED, 3);

    // 创建WiFi下拉列表
    drop_wifi = lv_dropdown_create(page_setting_wifi, NULL);
    lv_obj_add_style(drop_wifi, LV_OBJ_PART_MAIN, &style_focused); // 添加样式
    lv_obj_set_size(drop_wifi, 100, 30);                           // 设置大小
    lv_dropdown_add_option(drop_wifi, "default", 0);
    // lv_dropdown_clear_options(drop_wifi);
    lv_obj_align(drop_wifi, page_setting_wifi, LV_ALIGN_CENTER, 0, -30); // 调整位置

    // 创建标题标签
    lv_obj_t *label_wifi = lv_label_create(page_setting_wifi, NULL);
    lv_obj_set_style_local_text_font(label_wifi, 0, 0, &hanzi_25);     // 设置字体
    lv_label_set_text(label_wifi, "连接WiFi");                         // 设置文本
    lv_obj_align(label_wifi, drop_wifi, LV_ALIGN_OUT_TOP_MID, 0, -10); // 调整位置

    // 创建文本标签
    lv_obj_t *label_ssid = lv_label_create(page_setting_wifi, NULL);
    lv_obj_set_style_local_text_font(label_ssid, 0, 0, &hanzi_25);
    lv_label_set_text(label_ssid, "WiFi: ");                          // 设置文本
    lv_obj_align(label_ssid, drop_wifi, LV_ALIGN_OUT_LEFT_MID, 0, 0); //调整位置

    // 创建扫描按钮
    btn_scan = lv_btn_create(page_setting_wifi, NULL);
    lv_obj_add_style(btn_scan, LV_OBJ_PART_MAIN, &style_focused);    // 添加样式
    lv_obj_set_size(btn_scan, 50, 30);                               //设置大小
    lv_obj_t *label_scan = lv_label_create(btn_scan, label_ssid);    //按钮文本标签
    lv_label_set_text(label_scan, "扫描");                           //设置文本
    lv_obj_align(btn_scan, drop_wifi, LV_ALIGN_OUT_RIGHT_MID, 0, 0); //调整位置
    // 设置扫描按钮回调
    lv_obj_set_event_cb(btn_scan, btn_sacn_cb);

    //创建密码输入框
    text_area_pwd = lv_textarea_create(page_setting_wifi, NULL);
    lv_obj_add_style(text_area_pwd, LV_OBJ_PART_MAIN, &style_focused);      // 添加样式
    lv_textarea_set_pwd_mode(text_area_pwd, true);                          // 密码输入模式
    lv_textarea_set_one_line(text_area_pwd, true);                          // 单行输入
    lv_textarea_set_cursor_hidden(text_area_pwd, true);                     // 隐藏光标
    lv_textarea_set_text(text_area_pwd, "");                                //设置默认值
    lv_obj_set_size(text_area_pwd, 100, 30);                                //设置大小
    lv_obj_align(text_area_pwd, drop_wifi, LV_ALIGN_OUT_BOTTOM_MID, 0, 10); // 调整位置
    lv_obj_set_event_cb(text_area_pwd, text_area_cb);                       //设置回调函数

    // 创建文本标签
    lv_obj_t *label_pwd = lv_label_create(page_setting_wifi, label_ssid);
    lv_label_set_text(label_pwd, "密码: ");                              // 设置文本
    lv_obj_align(label_pwd, text_area_pwd, LV_ALIGN_OUT_LEFT_MID, 0, 0); // 调整位置

    // 创建连接按钮
    btn_connect = lv_btn_create(page_setting_wifi, NULL);
    lv_obj_add_style(btn_connect, LV_OBJ_PART_MAIN, &style_focused);        // 添加样式
    lv_obj_t *label_connect = lv_label_create(btn_connect, label_ssid);     // 按钮文本标签
    lv_label_set_text(label_connect, "连接");                               // 设置文本
    lv_obj_set_size(btn_connect, 50, 30);                                   // 调整按钮大小
    lv_obj_align(btn_connect, text_area_pwd, LV_ALIGN_OUT_RIGHT_MID, 0, 0); // 调整位置
    lv_obj_set_event_cb(btn_connect, btn_connect_cb);                       //设置回调函数
}
