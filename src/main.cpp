#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "webserver.h"

const char* ssid = "POCO";     // 替换为你的WiFi名称
const char* password = "22222222";  // 替换为你的WiFi密码

MyWebServer webServer;

void setup() {
    // 初始化串口
    Serial.begin(921600);
    delay(10);

    // 设置WiFi模式
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();  // 断开之前的连接
    delay(100);

    // 连接WiFi
    Serial.printf("\nConnecting to %s ", ssid);
    WiFi.begin(ssid, password);

    // 等待WiFi连接
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) { // 设置最大尝试次数
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected!");
        Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
        
        // 初始化Web服务器
        webServer.init();
    } else {
        Serial.println("\nFailed to connect!");
    }
}

void loop() {
    // 检查WiFi连接状态
    if (WiFi.status() == WL_CONNECTED) {
        webServer.handle();
    } else {
        // WiFi断开，尝试重新连接
        Serial.println("WiFi disconnected. Attempting to reconnect...");
        WiFi.begin(ssid, password);
        delay(5000);  // 等待5秒后再次尝试
    }
    
    // 给系统一些处理时间
    delay(1);
    yield();
}