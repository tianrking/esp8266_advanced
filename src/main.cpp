// #include <Arduino.h>
// #include <ESP8266WiFi.h>
// #include "webserver.h"

// const char* ssid = "POCO";     // 替换为你的WiFi名称
// const char* password = "22222222";  // 替换为你的WiFi密码

// MyWebServer webServer;

// void setup() {
//     Serial.begin(921600);
    
//     // 连接WiFi
//     WiFi.begin(ssid, password);
//     Serial.println("");
//     Serial.print("Connecting to WiFi");
    
//     // 等待WiFi连接
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500);
//         Serial.print(".");
//     }
    
//     Serial.println("");
//     Serial.print("Connected to ");
//     Serial.println(ssid);
//     Serial.print("IP address: ");
//     Serial.println(WiFi.localIP());

//     // 初始化Web服务器
//     webServer.init();
// }

// void loop() {
//     webServer.handle();
//     delay(1);  // 给系统一些处理时间
// }

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "webserver.h"

const char* ssid = "KUNIU";       // WiFi名称
const char* password = "kuniu666"; // WiFi密码

// 静态IP配置
IPAddress staticIP(192, 168, 110, 102); // 设置静态IP
IPAddress gateway(192, 168, 110, 1);    // 网关地址
IPAddress subnet(255, 255, 255, 0);     // 子网掩码
// IPAddress dns(192, 168, 110, 1);        // DNS服务器，通常和网关相同

MyWebServer webServer;

void setup() {
    Serial.begin(921600);
    
    // 配置静态IP
    WiFi.config(staticIP, gateway, subnet);
    
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