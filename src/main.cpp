#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "webserver.h"

const char* ssid = "KUNIU";     // 替换为你的WiFi名称
const char* password = "kuniu666";  // 替换为你的WiFi密码

MyWebServer webServer;

void setup() {
    Serial.begin(921600);
    
    // 连接WiFi
    WiFi.begin(ssid, password);
    Serial.println("");
    Serial.print("Connecting to WiFi");
    
    // 等待WiFi连接
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // 初始化Web服务器
    webServer.init();
}

void loop() {
    webServer.handle();
    delay(1);  // 给系统一些处理时间
}