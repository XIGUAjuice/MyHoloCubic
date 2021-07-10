#include <smartconfig_page.h>

void createPageSmartConfig()
{
    // 创建二维码图像
    lv_obj_t *img_smart_config = lv_img_create(page_smart_config, NULL);
    lv_img_set_src(img_smart_config, &smart_config);                           // 设置图片路径
    lv_obj_align(img_smart_config, page_smart_config, LV_ALIGN_CENTER, 0, 10); // 调整位置

    // 创建文本标签
    lv_obj_t *label_smart_config = lv_label_create(page_smart_config, NULL);
    lv_obj_set_style_local_text_font(label_smart_config, 0, 0, &hanzi_25);          // 设置字体
    lv_label_set_text(label_smart_config, "扫码连接WiFi");                          // 设置标签文本
    lv_obj_align(label_smart_config, img_smart_config, LV_ALIGN_OUT_TOP_MID, 0, 0); // 调整位置

    // 创建加载圈
    spinner_smart_config = lv_spinner_create(page_smart_config, NULL);
    lv_obj_set_size(spinner_smart_config, 40, 40);                                                       // 设置加载圈大小
    lv_spinner_set_type(spinner_smart_config, LV_SPINNER_TYPE_FILLSPIN_ARC);                             // 设置加载圈的样式(弧长会长短变化)
    lv_obj_set_style_local_line_width(spinner_smart_config, LV_SPINNER_PART_BG, LV_STATE_DEFAULT, 8);    // 调整背景线圈粗细
    lv_obj_set_style_local_line_width(spinner_smart_config, LV_SPINNER_PART_INDIC, LV_STATE_DEFAULT, 8); // 调整内部指示线圈粗细
    lv_obj_align(spinner_smart_config, img_smart_config, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);                 // 调整加载圈位置
    lv_obj_set_hidden(spinner_smart_config, true);                                                       // 默认不可见

    // 创建连接完成图标
    img_ok = lv_img_create(page_smart_config, NULL);
    lv_img_set_src(img_ok, "S:/bin/ok.bin");                               // 设置图片路径
    lv_obj_align(img_ok, img_smart_config, LV_ALIGN_OUT_BOTTOM_MID, 0, 0); // 调整位置
    lv_obj_set_hidden(img_ok, true);                                       // 默认不可见
}
