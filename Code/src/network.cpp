#include "network.h"

char Network::ntpServerName[] = "ntp.aliyun.com";
int Network::timeZone = 8;
String Network::city_code;
byte Network::packetBuffer[NTP_PACKET_SIZE] = {};
WiFiUDP Network::Udp;
wl_status_t Network::wifi_status = WL_IDLE_STATUS;

/* 连接指定WiFi */
void Network::connectWifi(String ssid, String password)
{
	int count = 0;
	Serial.print("Connecting: ");
	Serial.print(ssid.c_str());
	Serial.print(" @");
	Serial.println(password.c_str());

	WiFi.begin(ssid.c_str(), password.c_str());
	while (1)
	{
		delay(500);
		wl_status_t status = WiFi.status();
		Serial.print(".");
		count++;
		if (status == WL_CONNECTED)
		{
			Serial.println("");
			Serial.println("WiFi connected");
			Serial.println("IP address: ");
			Serial.println(WiFi.localIP());
			break;
		}
		else if (count > 15)
		{
			Serial.println("");
			Serial.println("WiFi connect failed!");
			break;
		}
	}
	setCityCode();
}

/* 开机连接上次的WiFi */
void Network::connectWifiLast()
{	
	Serial.println("读取WiFi设置");
	DynamicJsonDocument wifi_doc(100);
	sd.readJson("/wifi.json", wifi_doc);
	String ssid = wifi_doc["ssid"];
	String password = wifi_doc["password"];
	connectWifi(ssid, password);
}

/* 检查WiFi连接 */
wl_status_t Network::checkWifi()
{
	wl_status_t status = WiFi.status();
	if (status != wifi_status)
	{
		/* 若WiFi状态发生变化 */
		wifi_status = status;
		return status;
	}
	else
	{
		/* WiFi状态不变 */
		return WL_IDLE_STATUS;
	}
}

/* 初始化NTP服务器 */
void Network::initNtp()
{
	Serial.println("时间同步开");
	setSyncProvider(getNtpTime);
	setSyncInterval(600);
}

void Network::stopNtp()
{
	Serial.println("时间同步关");
	setSyncProvider(NULL);
}

unsigned int Network::getBilibiliFans(String uid)
{
	String fansCount = "";
	HTTPClient http;
	http.begin("http://api.bilibili.com/x/relation/stat?vmid=" + uid);

	// start connection and send HTTP headerFFF
	int httpCode = http.GET();

	// httpCode will be negative on error
	if (httpCode > 0)
	{
		// file found at server
		if (httpCode == HTTP_CODE_OK)
		{
			String payload = http.getString();
			int pos = (payload.indexOf("follower"));
			fansCount = payload.substring(pos + 10, payload.length() - 2);
		}
	}
	else
	{
		Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
	}
	http.end();

	return atol(fansCount.c_str());
}

/* NTP同步时钟 */
time_t Network::getNtpTime()
{
	IPAddress ntpServerIp; // NTP服务器IP地址

	while (Udp.parsePacket() > 0)
		; // 丢弃之前收到的UDP报文
	Serial.println("发送NTP请求…………");
	WiFi.hostByName(ntpServerName, ntpServerIp);
	sendNtpPacket(ntpServerIp);

	// 等待接收报文
	uint32_t beginWait = millis();
	while (millis() - beginWait < 1500)
	{
		int size = Udp.parsePacket();
		if (size >= NTP_PACKET_SIZE)
		{
			Serial.println("Receive NTP Response");
			Udp.read(packetBuffer, NTP_PACKET_SIZE); // read packet into the buffer
			unsigned long secsSince1900;
			// 将32位时间戳数据转换为1970.1.1的时间戳
			secsSince1900 = (unsigned long)packetBuffer[40] << 24;
			secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
			secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
			secsSince1900 |= (unsigned long)packetBuffer[43];
			return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
		}
	}
	Serial.println("No NTP Response :-(");
	return 0; // UDP服务器无响应
}

/* 发送NTP请求报文 */
void Network::sendNtpPacket(IPAddress &address)
{
	// 清空时间戳缓冲区
	memset(packetBuffer, 0, NTP_PACKET_SIZE);

	// 初始化NTP请求报文
	// https://labs.apnic.net/?p=462
	packetBuffer[0] = 0b11100011; // LI(不指定), Version(版本4), Mode(客户端模式)
	packetBuffer[1] = 0;		  // Stratum(不指定)
	packetBuffer[2] = 6;		  // Polling Interval(6秒)
	packetBuffer[3] = 0xEC;		  // Peer Clock Precision(2^-20)
	// Root Delay & Root Dispersion(8字节, 不重要, 为空)
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;
	// NTP请求报文填写完毕

	// 发送请求报文
	Udp.beginPacket(address, 123); //NTP 服务端口为123
	Udp.write(packetBuffer, NTP_PACKET_SIZE);
	Udp.endPacket();
}

/* 根据ip设置城市代码 */
void Network::setCityCode()
{
	HTTPClient http;
	String url = String("https://restapi.amap.com/v3/ip?output=json&key=") + WEBKEY;
	http.begin(url);
	int http_code = http.GET();
	Serial.println("发起请求：" + url);
	if (http_code == HTTP_CODE_OK)
	{
		Stream &response = http.getStream();
		DynamicJsonDocument doc(1024);
		deserializeJson(doc, response);
		String city_code = doc["adcode"];
		String city = doc["city"];
		Serial.printf("当前所在城市为: %s, 城市代码为: %s \n", city.c_str(), city_code.c_str());
		this->city_code = city_code;
	}
	else
	{
		Serial.println("获取城市代码失败");
	}
}

/* 获取城市代码 */
String Network::getCityCode()
{
	return city_code;
}

/* 获取实时天气 */
JsonObject Network::getWeatherLive(String city_code)
{
	HTTPClient http;
	String url = "https://restapi.amap.com/v3/weather/weatherInfo?city=" + city_code + "&key=" + WEBKEY + "&extensions=base";
	http.begin(url);
	int http_code = http.GET();
	Serial.println("获取实时天气中：" + url);
	if (http_code == HTTP_CODE_OK)
	{
		Serial.println("网络请求成功");
		Stream &response = http.getStream();
		DynamicJsonDocument doc(1024);
		deserializeJson(doc, response);
		JsonObject weather_live = doc["lives"][0];
		return weather_live;
	}
	else
	{
		Serial.println("网络请求失败");
		return JsonObject();
	}
}

/* 获取天气预报 */
JsonArray Network::getWeatherForecast(String city_code)
{
	HTTPClient http;
	String url = "https://restapi.amap.com/v3/weather/weatherInfo?city=" + city_code + "&key=" + WEBKEY + "&extensions=all";
	http.begin(url);
	int http_code = http.GET();
	Serial.println("获取天气预报中：" + url);
	if (http_code == HTTP_CODE_OK)
	{
		Serial.println("网络请求成功");
		Stream &response = http.getStream();
		DynamicJsonDocument doc(1024);
		deserializeJson(doc, response);
		JsonArray weather_forecast = doc["forecasts"][0]["casts"];
		return weather_forecast;
	}
	else
	{
		Serial.println("网络请求失败");
		return JsonArray();
	}
}