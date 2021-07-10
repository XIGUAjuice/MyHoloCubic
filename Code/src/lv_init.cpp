#include <lv_init.h>

TFT_eSPI tft = TFT_eSPI();

/* 屏幕刷新回调函数 */
void my_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    // tft.pushPixelsDMA(&color_p->full, w * h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp_drv);
}

/* 设置屏幕背光亮度 */
void setBackLight(float duty)
{
    duty = constrain(duty, 0, 1);
    duty = 1 - duty;
    ledcWrite(LCD_BL_PWM_CHANNEL, (int)(duty * 255));
}

/* LVGL初始化 */
void initLvgl()
{
    lv_init();            //初始化LVGL
    lv_fs_if_init();      // 初始化LVGL文件系统
    tft.begin();          // 初始化屏幕驱动
    tft.setRotation(4);
    // // 启用DMA
    // if (tft.initDMA())
    // {
    //     Serial.println("DMA启用成功");
    // }
    // else
    // {
    //     Serial.println("DMA启用失败");
    // }

    ledcSetup(LCD_BL_PWM_CHANNEL, 5000, 8);        // 设置PWM发生器
    ledcAttachPin(LCD_BL_PIN, LCD_BL_PWM_CHANNEL); // 指定PWM引脚
    setBackLight(1);                               //设置最大亮度

    // 设置双缓冲区
    static lv_disp_buf_t disp_buf;
    static lv_color_t buf1[120 * 120];
    // static lv_color_t buf2[240 * 10];
    // lv_disp_buf_init(&disp_buf, buf1, buf2, 240 * 10);
    lv_disp_buf_init(&disp_buf, buf1, NULL, 120 * 120);
    Serial.println("lv_buf initialisation done.");

    // 初始化LVGL驱动
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    Serial.println("lv_drv initialisation done.");

    // 设置LVGL驱动
    disp_drv.hor_res = 240;
    disp_drv.ver_res = 240;
    disp_drv.flush_cb = my_flush_cb;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);
    Serial.println("lv_drv register done.");
}