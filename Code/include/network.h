#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <ArduinoJson.h>
#include <sd_card.h>

/* https://lbs.amap.com/ 申请API */
#define WEBKEY "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

class Network
{
private:
	static char ntpServerName[];					   // NTP服务器
	static const int NTP_PACKET_SIZE = 48;			   // NTP消息由48字节组成
	static const unsigned int localPort = 8888;		   // UDP监听端口
	static byte packetBuffer[NTP_PACKET_SIZE];		   // 存储时间戳的缓冲区
	static int timeZone;							   // 东八区
	static WiFiUDP Udp;								   // Udp对象
	static wl_status_t wifi_status;					   // WiFi连接状况
	static void sendNtpPacket(IPAddress &ntpServerIp); // 发送NTP请求
	static String city_code;						   // 所在城市代码

public:
	/* 连接指定WiFI */
	void connectWifi(String ssid, String password);
	/* 开机连接上次的WiFi */
	void connectWifiLast();
	/* 检查WiFi连接 */
	wl_status_t checkWifi();
	/* 初始化NTP服务器 */
	void initNtp();

	/* 停止NTP时间同步 */
	void stopNtp();

	unsigned int getBilibiliFans(String url);
	/* NTP同步时钟 */
	static time_t getNtpTime();
	/* 根据ip设置城市代码 */
	void setCityCode();
	/* 获取城市代码 */
	String getCityCode();
	/* 获取实时天气 */
	JsonObject getWeatherLive(String city_code);
	/* 获取天气预报 */
	JsonArray getWeatherForecast(String city_code);
};

extern Network net;

#endif