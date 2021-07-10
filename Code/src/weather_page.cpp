#include <weather_page.h>
#include <fresh_task.h>
#include <global.h>

int highest_DayTemp, lowest_DayTemp;
int highest_NightTemp, lowest_NightTemp;
int DayTemp[3], NightTemp[3];
String DayWeather[3];

lv_obj_t *img_DayWeather_today;
lv_obj_t *img_DayWeather_tomorrow;
lv_obj_t *img_DayWeather_after_tomorrow;
lv_obj_t *chart_DayTemp;
lv_obj_t *chart_NightTemp;
lv_obj_t *label_DayTemp_today;
lv_obj_t *label_DayTemp_tomorrow;
lv_obj_t *label_DayTemp_after_tomorrow;
lv_obj_t *label_NightTemp_today;
lv_obj_t *label_NightTemp_tomorrow;
lv_obj_t *label_NightTemp_after_tomorrow;
lv_obj_t *label_today;
lv_obj_t *label_tomorrow;
lv_obj_t *label_after_tomorrow;
lv_chart_series_t *ser1;
lv_chart_series_t *ser2;

int max_temp(int *temp)
{
	if (temp[0] > temp[1] && temp[0] > temp[2])
		return temp[0];
	if (temp[1] > temp[0] && temp[1] > temp[2])
		return temp[1];
	return temp[2];
}

int min_temp(int *temp)
{
	if (temp[0] < temp[1] && temp[0] < temp[2])
		return temp[0];
	if (temp[1] < temp[0] && temp[1] < temp[2])
		return temp[1];
	return temp[2];
}

/* 显示气温折线图 */
void display_temperature_chart(void)
{
	/* 获取相应城市的天气数据 */
	JsonArray three_day_weather = net.getWeatherForecast("440100");

	/* 获取第一天、第二天、第三天的温度数据 */
	for (int i = 0; i < 3; i++)
	{
		DayTemp[i] = three_day_weather[i]["daytemp"];
		NightTemp[i] = three_day_weather[i]["nighttemp"];
	}

	/* 创建白天温度折线图 */
	chart_DayTemp = lv_chart_create(page_weather, NULL);
	lv_obj_set_size(chart_DayTemp, 180, 50);						//图表的宽和高
	lv_obj_align(chart_DayTemp, NULL, LV_ALIGN_IN_TOP_MID, 0, 110); //位置
	lv_chart_set_type(chart_DayTemp, LV_CHART_TYPE_LINE);

	/* 创建晚上温度折线图 */
	chart_NightTemp = lv_chart_create(page_weather, NULL);
	lv_obj_set_size(chart_NightTemp, 180, 50);						  //图表的宽和高
	lv_obj_align(chart_NightTemp, NULL, LV_ALIGN_IN_TOP_MID, 0, 180); //位置
	lv_chart_set_type(chart_NightTemp, LV_CHART_TYPE_LINE);

	/* 创建chart的样式 */
	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_radius(&style, LV_STATE_DEFAULT, 0);	  //设置圆角半径
	lv_style_set_bg_opa(&style, LV_STATE_DEFAULT, 0);	  //设置背景不透明度
	lv_style_set_border_opa(&style, LV_STATE_DEFAULT, 0); //设置边框不透明度

	lv_obj_add_style(chart_DayTemp, LV_OBJ_PART_MAIN, &style);
	lv_obj_add_style(chart_NightTemp, LV_OBJ_PART_MAIN, &style);

	ser1 = lv_chart_add_series(chart_DayTemp, LV_COLOR_MAKE(0xf0, 0x7c, 0x82));	  //香叶红
	ser2 = lv_chart_add_series(chart_NightTemp, LV_COLOR_MAKE(0x29, 0xb7, 0xcb)); //蔚蓝

	lv_chart_set_div_line_count(chart_DayTemp, 0, 0);	//去掉网格线
	lv_chart_set_div_line_count(chart_NightTemp, 0, 0); //去掉网格线
	lv_chart_set_point_count(chart_DayTemp, 3);			//设置点数
	lv_chart_set_point_count(chart_NightTemp, 3);		//设置点数

	/* 白天温度的最大值最小值 */
	highest_DayTemp = max_temp(DayTemp);
	lowest_DayTemp = min_temp(DayTemp);

	/* 晚上温度的最大值最小值 */
	highest_NightTemp = max_temp(NightTemp);
	lowest_NightTemp = min_temp(NightTemp);

	lv_chart_set_range(chart_DayTemp, lowest_DayTemp - 1, highest_DayTemp + 1);		  // 设置图纵坐标的上下限
	lv_chart_set_range(chart_NightTemp, lowest_NightTemp - 1, highest_NightTemp + 1); // 设置图纵坐标的上下限

	lv_chart_set_point_id(chart_DayTemp, ser1, DayTemp[0], 0);
	lv_chart_set_point_id(chart_DayTemp, ser1, DayTemp[1], 1);
	lv_chart_set_point_id(chart_DayTemp, ser1, DayTemp[2], 2);

	lv_chart_set_point_id(chart_NightTemp, ser2, NightTemp[0], 0);
	lv_chart_set_point_id(chart_NightTemp, ser2, NightTemp[1], 1);
	lv_chart_set_point_id(chart_NightTemp, ser2, NightTemp[2], 2);

	Serial.println("白天温度与晚上温度：");
	Serial.printf("第一天白天温度 %d \n", DayTemp[0]);
	Serial.printf("第二天白天温度 %d \n", DayTemp[1]);
	Serial.printf("第三天白天温度 %d \n", DayTemp[2]);

	Serial.printf("第一天晚上温度 %d \n", NightTemp[0]);
	Serial.printf("第二天晚上温度 %d \n", NightTemp[1]);
	Serial.printf("第三天晚上温度 %d \n", NightTemp[2]);

	Serial.printf("最高温度为 %d \n", highest_DayTemp);
	Serial.printf("最低温度为 %d \n", lowest_DayTemp);
}

/* 显示气温数据标签 */
void display_temperature_label(void)
{
	// 创建数据标签样式
	static lv_style_t style_temp;
	lv_style_init(&style_temp);
	lv_style_set_text_font(&style_temp, 0, &temp_font); //设置字体

	// 创建汉字标签样式
	static lv_style_t style_word;
	lv_style_init(&style_word);
	lv_style_set_text_font(&style_word, 0, &hanzi_25); //设置字体

	// 创建数据标签
	label_DayTemp_today = lv_label_create(page_weather, NULL);
	label_DayTemp_tomorrow = lv_label_create(page_weather, NULL);
	label_DayTemp_after_tomorrow = lv_label_create(page_weather, NULL);
	label_NightTemp_today = lv_label_create(page_weather, NULL);
	label_NightTemp_tomorrow = lv_label_create(page_weather, NULL);
	label_NightTemp_after_tomorrow = lv_label_create(page_weather, NULL);

	// 创建日期标签
	label_today = lv_label_create(page_weather, NULL);
	label_tomorrow = lv_label_create(page_weather, NULL);
	label_after_tomorrow = lv_label_create(page_weather, NULL);

	// 设置默认值
	lv_label_set_text(label_DayTemp_today, (String(DayTemp[0]) + "℃").c_str());
	lv_label_set_text(label_DayTemp_tomorrow, (String(DayTemp[1]) + "℃").c_str());
	lv_label_set_text(label_DayTemp_after_tomorrow, (String(DayTemp[2]) + "℃").c_str());
	lv_label_set_text(label_NightTemp_today, (String(NightTemp[0]) + "℃").c_str());
	lv_label_set_text(label_NightTemp_tomorrow, (String(NightTemp[1]) + "℃").c_str());
	lv_label_set_text(label_NightTemp_after_tomorrow, (String(NightTemp[2]) + "℃").c_str());
	lv_label_set_text(label_today, "今天");
	lv_label_set_text(label_tomorrow, "明天");
	lv_label_set_text(label_after_tomorrow, "后天");

	//调整位置
	lv_obj_align(label_today, NULL, LV_ALIGN_IN_TOP_MID, -95, 10);
	lv_obj_align(label_tomorrow, NULL, LV_ALIGN_IN_TOP_MID, -15, 10);
	lv_obj_align(label_after_tomorrow, NULL, LV_ALIGN_IN_TOP_MID, 65, 10);

	lv_obj_align(label_DayTemp_today, NULL, LV_ALIGN_IN_TOP_MID, -85, 95);
	lv_obj_align(label_DayTemp_tomorrow, NULL, LV_ALIGN_IN_TOP_MID, -5, 95);
	lv_obj_align(label_DayTemp_after_tomorrow, NULL, LV_ALIGN_IN_TOP_MID, 75, 95);

	lv_obj_align(label_NightTemp_today, NULL, LV_ALIGN_IN_TOP_MID, -85, 165);
	lv_obj_align(label_NightTemp_tomorrow, NULL, LV_ALIGN_IN_TOP_MID, -5, 165);
	lv_obj_align(label_NightTemp_after_tomorrow, NULL, LV_ALIGN_IN_TOP_MID, 75, 165);

	//应用数据标签样式
	lv_obj_add_style(label_DayTemp_today, 0, &style_temp);
	lv_obj_add_style(label_DayTemp_tomorrow, 0, &style_temp);
	lv_obj_add_style(label_DayTemp_after_tomorrow, 0, &style_temp);
	lv_obj_add_style(label_NightTemp_today, 0, &style_temp);
	lv_obj_add_style(label_NightTemp_tomorrow, 0, &style_temp);
	lv_obj_add_style(label_NightTemp_after_tomorrow, 0, &style_temp);
	lv_obj_add_style(label_today, 0, &style_word);
	lv_obj_add_style(label_tomorrow, 0, &style_word);
	lv_obj_add_style(label_after_tomorrow, 0, &style_word);
}

/* 显示天气图标 */
void display_weather_icon(void)
{
	/* 获取相应城市的天气情况 */
	JsonArray three_day_weather = net.getWeatherForecast("440100");

	/* 获取第一天、第二天、第三天的天气情况 */
	for (int i = 0; i < 3; i++)
	{
		String temp_weather = three_day_weather[i]["dayweather"];
		Serial.printf("第 %d 天的天气为：\n", i + 1);
		Serial.println(temp_weather);
		DayWeather[i] = temp_weather;
	}

	time_t time_now = now(); //获取时间
	int h = hour(time_now);	 // 区分白天和晚上

	Serial.printf("h=%d \n", h);

	/* 今天白天天气 */
	img_DayWeather_today = lv_img_create(page_weather, NULL);
	char day_weather0[30];
	getWeatherIcon(DayWeather[0], h, day_weather0);
	lv_img_set_src(img_DayWeather_today, day_weather0);
	//lv_img_set_src(img_DayWeather_today, "S:/bin/leizhenyu.bin");
	lv_obj_align(img_DayWeather_today, label_DayTemp_today, LV_ALIGN_OUT_TOP_MID, 0, -4);
	Serial.println("今天的天气为：");
	Serial.println(day_weather0);

	/* 明天白天天气 */
	img_DayWeather_tomorrow = lv_img_create(page_weather, NULL);
	char day_weather1[30];
	getWeatherIcon(DayWeather[1], h, day_weather1);
	lv_img_set_src(img_DayWeather_tomorrow, day_weather1);
	//lv_img_set_src(img_DayWeather_tomorrow, "S:/bin/leizhenyu.bin");
	lv_obj_align(img_DayWeather_tomorrow, label_DayTemp_tomorrow, LV_ALIGN_OUT_TOP_MID, 0, -4);
	Serial.println("明天的天气为：");
	Serial.println(day_weather1);

	/* 后天白天天气 */
	img_DayWeather_after_tomorrow = lv_img_create(page_weather, NULL);
	char day_weather2[30];
	getWeatherIcon(DayWeather[2], h, day_weather2);
	lv_img_set_src(img_DayWeather_after_tomorrow, day_weather2);
	//lv_img_set_src(img_DayWeather_after_tomorrow, "S:/bin/yejianduoyun.bin");
	lv_obj_align(img_DayWeather_after_tomorrow, label_DayTemp_after_tomorrow, LV_ALIGN_OUT_TOP_MID, 0, -4);
	Serial.println("后天的天气为：");
	Serial.println(day_weather2);
}

void fresh_temperature_chart(void)
{
	/* 获取相应城市的天气数据 */
	JsonArray three_day_weather = net.getWeatherForecast("440100");

	/* 获取第一天、第二天、第三天的温度数据 */
	for (int i = 0; i < 3; i++)
	{
		DayTemp[i] = three_day_weather[i]["daytemp"];
		NightTemp[i] = three_day_weather[i]["nighttemp"];
	}

	/* 白天温度的最大值最小值 */
	highest_DayTemp = max_temp(DayTemp);
	lowest_DayTemp = min_temp(DayTemp);

	/* 晚上温度的最大值最小值 */
	highest_NightTemp = max_temp(NightTemp);
	lowest_NightTemp = min_temp(NightTemp);

	lv_chart_set_range(chart_DayTemp, lowest_DayTemp - 1, highest_DayTemp + 1);		  // 设置图纵坐标的上下限
	lv_chart_set_range(chart_NightTemp, lowest_NightTemp - 1, highest_NightTemp + 1); // 设置图纵坐标的上下限

	lv_chart_set_point_id(chart_DayTemp, ser1, DayTemp[0], 0);
	lv_chart_set_point_id(chart_DayTemp, ser1, DayTemp[1], 1);
	lv_chart_set_point_id(chart_DayTemp, ser1, DayTemp[2], 2);

	lv_chart_set_point_id(chart_NightTemp, ser2, NightTemp[0], 0);
	lv_chart_set_point_id(chart_NightTemp, ser2, NightTemp[1], 1);
	lv_chart_set_point_id(chart_NightTemp, ser2, NightTemp[2], 2);
}

void fresh_temperature_label(void)
{
	// 更新温度数据
	lv_label_set_text(label_DayTemp_today, (String(DayTemp[0]) + "℃").c_str());
	lv_label_set_text(label_DayTemp_tomorrow, (String(DayTemp[1]) + "℃").c_str());
	lv_label_set_text(label_DayTemp_after_tomorrow, (String(DayTemp[2]) + "℃").c_str());
	lv_label_set_text(label_NightTemp_today, (String(NightTemp[0]) + "℃").c_str());
	lv_label_set_text(label_NightTemp_tomorrow, (String(NightTemp[1]) + "℃").c_str());
	lv_label_set_text(label_NightTemp_after_tomorrow, (String(NightTemp[2]) + "℃").c_str());
}

void fresh_weather_icon(void)
{
	JsonArray three_day_weather = net.getWeatherForecast("440100");

	/* 获取第一天、第二天、第三天的天气情况 */
	for (int i = 0; i < 3; i++)
	{
		String temp_weather = three_day_weather[i]["dayweather"];
		Serial.printf("第 %d 天的天气为：\n", i + 1);
		Serial.println(temp_weather);
		DayWeather[i] = temp_weather;
	}

	time_t time_now = now(); //获取时间
	int h = hour(time_now);	 // 区分白天和晚上

	Serial.printf("h=%d \n", h);

	/* 今天白天天气 */
	char day_weather0[30];
	getWeatherIcon(DayWeather[0], h, day_weather0);
	lv_img_set_src(img_DayWeather_today, day_weather0);

	/* 明天白天天气 */
	char day_weather1[30];
	getWeatherIcon(DayWeather[1], h, day_weather1);
	lv_img_set_src(img_DayWeather_tomorrow, day_weather1);

	/* 后天白天天气 */
	char day_weather2[30];
	getWeatherIcon(DayWeather[2], h, day_weather2);
	lv_img_set_src(img_DayWeather_after_tomorrow, day_weather2);
}

