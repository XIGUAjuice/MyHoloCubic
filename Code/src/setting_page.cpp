#include <setting_page.h>
#include <global.h>

void createPageSetting()
{
    // 创建背景
    lv_obj_t *img_base = lv_img_create(page_setting, NULL);
    lv_img_set_src(img_base, &base_face);

    // 创建设置图标
    lv_obj_t *img_setting = lv_img_create(page_setting, NULL);
    lv_img_set_src(img_setting, "S:/bin/setting.bin");
    lv_obj_align(img_setting, page_setting, LV_ALIGN_CENTER, 0, 10); // 调整位置

    // 创建设置标签
    lv_obj_t *label_setting = lv_label_create(page_setting, NULL);
    lv_label_set_text(label_setting, "设 置");                              // 设置文本
    lv_obj_set_style_local_text_font(label_setting, 0, 0, &hanzi_25);       // 设置字体
    lv_obj_align(label_setting, img_setting, LV_ALIGN_OUT_TOP_MID, 0, -10); // 调整位置
}
