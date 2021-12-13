#include <TFT_eSPI.h>
#include <lvgl.h>
#include "lv_port_fatfs.h"
#include "sd_card.h"
#include "network.h"
#include "home_page.h"
#include "setting_page.h"
#include "wifi_page.h"
#include "input_device_mpu.h"
#include "fresh_task.h"
#include "lv_init.h"
#include "smartconfig_page.h"
#include "page_alarm.h"
#include "weather_page.h"
#include "cityweather.h"

SdCard sd;
Network net;
InputDevice indev;
lv_obj_t * bar1;

void setup()
{
    /* 打开串口 */
    Serial.begin(115200);

    /* LVGL初始化 */
    initLvgl();

    /* SD卡初始化 */
    Serial.println("初始化SD卡...");
    sd.init();

    /* 显示Logo */
    lv_obj_t *img_logo = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img_logo, "S:/bin/logo.bin");

    /* 启动lvgl的线程 */
    xTaskCreate(freshLvgl, "lvgl", 20480, NULL, 24, &task_lvgl);

    /* 显示进度条 */
    bar1 = lv_bar_create(lv_scr_act(), NULL);
    lv_obj_set_size(bar1, 200, 20);
    lv_obj_align(bar1, NULL, LV_ALIGN_CENTER, 0, 100);
    lv_bar_set_anim_time(bar1, 2000);
    lv_bar_set_range(bar1, 0, 3);
    lv_bar_set_value(bar1, 1, LV_ANIM_ON);

    /* 自动连接上次的WiFi */
    net.connectWifiLast();
    lv_bar_set_value(bar1, 2, LV_ANIM_ON);

    /* 初始化陀螺仪 */
    indev.mpuInit();
    lv_bar_set_value(bar1, 3, LV_ANIM_ON);
    delay(2000);
    vTaskSuspend(task_lvgl);
    delay(1500);
    
    /* 创建UI */
    Serial.println("创建UI...");
    createTile();             //创建磁贴
    createPageHome();         //创建主页面
    createPageSetting();      // 创建设置页面
    createPageWifiSetting();  //创建WiFi设置界面
    createPageSmartConfig();  // 创建扫码页面
    createPageAlarm();        // 创建闹钟页面
    createPageAlarmSetting(); // 创建闹钟设置页面
    createPageWeather();      // 创建城市天气页面
    // 创建天气预报页面
    display_temperature_chart();
    display_temperature_label();
    display_weather_icon();

    vTaskResume(task_lvgl);

    /* 初始化输入设备 */
    Serial.println("初始化输入设备...");
    indev.inputDriveInit();

    /* 创建任务 */
    createTasks(); // 创建LVGL任务
}

void loop()
{
}