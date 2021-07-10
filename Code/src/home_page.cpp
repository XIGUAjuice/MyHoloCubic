#include <home_page.h>
#include <global.h>

/* 创建表盘 */
void createWatchFace()
{
    // 创建背景
    lv_obj_t *img_base = lv_img_create(page_home, NULL);
    lv_img_set_src(img_base, &base_face);

    // 创建Wifi连接图标
    img_wifi = lv_img_create(page_home, NULL);
    lv_img_set_src(img_wifi, "S:/bin/wifi1.bin");
    lv_obj_align_mid(img_wifi, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 70, -40);
}

/* 创建时间模块（用来显示小时与分钟） */
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

/* 创建时间标签（用来显示秒） */
void createLabelTime()
{
    // 创建时间标签样式
    static lv_style_t style_time;
    lv_style_init(&style_time);
    lv_style_set_text_font(&style_time, 0, &digital_medium); //设置字体

    // 创建时间标签
    label_time = lv_label_create(page_home, NULL);
    lv_label_set_text(label_time, "00");                              // 设置默认值
    lv_obj_align(label_time, cont_time, LV_ALIGN_IN_RIGHT_MID, 8, 5); //调整位置

    //应用时间标签样式
    lv_obj_add_style(label_time, 0, &style_time);
}

/* 创建日期标签 */
void createLabelDate()
{
    // 设置日期容器样式
    static lv_style_t style_cont_date;
    lv_style_init(&style_cont_date);
    lv_style_set_text_font(&style_cont_date, 0, &alphabet); // 设置字体
    lv_style_set_border_opa(&style_cont_date, 0, 0);        // 取消边框
    lv_style_set_bg_opa(&style_cont_date, 0, 0);            // 取消背景
    lv_style_set_pad_inner(&style_cont_date, 0, 30);        //设置标签之间的间距

    // 创建日期容器
    cont_date = lv_cont_create(page_home, NULL);
    lv_cont_set_layout(cont_date, LV_LAYOUT_ROW_MID);            // 排成一行
    lv_cont_set_fit(cont_date, LV_FIT_TIGHT);                    //紧凑布局
    lv_obj_align_mid(cont_date, NULL, LV_ALIGN_CENTER, -30, 45); // 调整位置

    //应用日期容器样式
    lv_obj_add_style(cont_date, 0, &style_cont_date);

    //创建日期标签
    for (int i = 0; i < 3; i++)
    {
        label_date[i] = lv_label_create(cont_date, NULL);
    }

    // 设置默认值
    lv_label_set_text(label_date[0], "JAN");
    lv_label_set_text(label_date[1], "01");
    lv_label_set_text(label_date[2], "SUN");
}

/* 创建实时天气图标 */
void createWeatherLive()
{
    // 创建天气图标
    img_weather = lv_img_create(page_home, NULL);
    lv_img_set_src(img_weather, "S:/bin/baitianqing.bin");
    lv_obj_align_mid(img_weather, NULL, LV_ALIGN_OUT_BOTTOM_MID, 3, -35);

    // 创建温度图标
    img_temp = lv_img_create(page_home, NULL);
    lv_img_set_src(img_temp, "S:/bin/gaowen.bin");
    lv_obj_align_mid(img_temp, NULL, LV_ALIGN_IN_TOP_RIGHT, -70, 40);

    // 创建湿度图标
    lv_obj_t *img_huminity = lv_img_create(page_home, NULL);
    lv_img_set_src(img_huminity, "S:/bin/shidu.bin");
    lv_obj_align_mid(img_huminity, NULL, LV_ALIGN_IN_TOP_LEFT, 80, 35);

    //创建温度标签
    label_temp = lv_label_create(page_home, NULL);
    lv_label_set_text(label_temp, "00℃");
    lv_obj_set_style_local_text_font(label_temp, 0, 0, &temp_font);
    lv_obj_set_style_local_text_color(label_temp, 0, 0, LV_COLOR_WHITE);
    lv_obj_align_mid(label_temp, img_temp, LV_ALIGN_CENTER, -7, 25);

    // 创建湿度标签
    label_humidity = lv_label_create(page_home, NULL);
    lv_label_set_text(label_humidity, "00");
    lv_obj_set_style_local_text_font(label_humidity, 0, 0, &temp_font);
    lv_obj_set_style_local_text_color(label_humidity, 0, 0, LV_COLOR_WHITE);
    lv_obj_align_mid(label_humidity, label_temp, LV_ALIGN_CENTER, -83, 0);
}

/* 创建主页面 */
void createPageHome()
{
    createWatchFace();
    createRollerTime();
    createLabelTime();
    createWeatherLive();
    createLabelDate();
}