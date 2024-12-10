#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

// 包含所有页面处理器
#include "page_home.h"
#include "page_control.h"
#include "page_sensor.h"
#include "page_mode.h"
#include "page_log.h"
#include "page_settings.h"
#include "page_advanced.h"
#include "page_debug.h"

// 包含所有API处理器
#include "api_control.h"
#include "api_sensor.h"
#include "api_mode.h"
#include "api_log.h"
#include "api_settings.h"

#define MAX_LOGS 100  // 最大日志条数

class MyWebServer {
private:
    ESP8266WebServer* server;
    String logs[MAX_LOGS];  // 日志数组
    int logCount = 0;       // 当前日志数量
    
    void setupRoutes();
    
public:
    MyWebServer();
    ~MyWebServer();
    void init();
    void handle();
    void log(const String& message);
    void clearLogs();
    String* getLogs() { return logs; }
    int getLogCount() { return logCount; }
};

#endif // WEBSERVER_H