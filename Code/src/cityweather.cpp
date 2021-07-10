#include "cityweather.h"


lv_obj_t *label_temp_city1;
lv_obj_t *label_temp_city2;
lv_obj_t *label_temp_city3;
lv_obj_t *img_weather1;
lv_obj_t *img_weather2;
lv_obj_t *img_weather3;
lv_obj_t *label_text_city1;
lv_obj_t *label_text_city2;
lv_obj_t *label_text_city3;
lv_obj_t *page_multi_weather;
void createPageWeather()
{
  lv_obj_t * kuang1;
  kuang1 = lv_obj_create(page_multi_weather, NULL);
  lv_obj_set_size(kuang1, 230, 60);
  lv_obj_align(kuang1, page_multi_weather, LV_ALIGN_CENTER, 0 , 0);

  lv_obj_t * kuang2;
  kuang2 = lv_obj_create(page_multi_weather, NULL);
  lv_obj_set_size(kuang2, 230, 60);
  lv_obj_align(kuang2, kuang1, LV_ALIGN_OUT_BOTTOM_MID, 0 , 5);

  lv_obj_t * kuang3;
  kuang3 = lv_obj_create(page_multi_weather, NULL);
  lv_obj_set_size(kuang3, 230, 60);
  lv_obj_align(kuang3, kuang1, LV_ALIGN_OUT_TOP_MID, 0 , -5);



  img_weather1 = lv_img_create(kuang1, NULL);

  lv_obj_align_mid(img_weather1, kuang1, LV_ALIGN_CENTER, 0, 0);

  label_temp_city1 = lv_label_create(kuang1, NULL);
  lv_label_set_text(label_temp_city1, "30℃");
  // lv_label_set_align(label_temp, LV_LABEL_ALIGN_CENTER);
  lv_obj_set_style_local_text_font(label_temp_city1, 0, 0, &temp_font);
  lv_obj_align_mid(label_temp_city1, kuang1, LV_ALIGN_CENTER, 80, 0);

  label_text_city1 = lv_label_create(kuang1, NULL);
  lv_label_set_text(label_text_city1, "GZ");
  lv_obj_align_mid(label_text_city1, kuang1, LV_ALIGN_CENTER, -80, 0);



  img_weather2 = lv_img_create(kuang2, NULL);

  lv_obj_align_mid(img_weather2, kuang2,  LV_ALIGN_CENTER, 0, 0);

  label_temp_city2 = lv_label_create(kuang2, NULL);
  lv_label_set_text(label_temp_city2, "28℃");
  // lv_label_set_align(label_temp, LV_LABEL_ALIGN_CENTER);
  lv_obj_set_style_local_text_font(label_temp_city2, 0, 0, &temp_font);
  lv_obj_align_mid(label_temp_city2, kuang2, LV_ALIGN_CENTER, 80, 0);

  label_text_city2 = lv_label_create(kuang2, NULL);
  lv_label_set_text(label_text_city2, "SZ");
  lv_obj_align_mid(label_text_city2, kuang2, LV_ALIGN_CENTER, -80, 0);

  img_weather3 = lv_img_create(kuang3, NULL);

  lv_obj_align_mid(img_weather3, kuang3,  LV_ALIGN_CENTER, 0, 0);

  label_temp_city3 = lv_label_create(kuang3, NULL);
  lv_label_set_text(label_temp_city3, "28℃");
  // lv_label_set_align(label_temp, LV_LABEL_ALIGN_CENTER);
  lv_obj_set_style_local_text_font(label_temp_city3, 0, 0, &temp_font);
  lv_obj_align_mid(label_temp_city3, kuang3, LV_ALIGN_CENTER, 80, 0);

  label_text_city3 = lv_label_create(kuang3, NULL);
  lv_label_set_text(label_text_city3, "ZJ");
  lv_obj_align_mid(label_text_city3, kuang3, LV_ALIGN_CENTER, -80, 0);

}

