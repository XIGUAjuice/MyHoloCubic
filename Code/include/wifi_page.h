#ifndef WIFI_PAGE
#define WIFI_PAGE

#include <control.h>
#include <network.h>

/* 创建键盘 */
void createKeyboard(lv_obj_t *text_area);

/* 输入框回调函数 */
// static void text_area_cb(lv_obj_t *text_area, lv_event_t event);

/* 扫描按钮回调函数 */
// static void btn_sacn_cb(lv_obj_t *obj, lv_event_t event);

/* 创建WiFi设置页面 */
void createPageWifiSetting();

#endif