#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

#define MAX_LOGS 100  // 最大日志条数

class MyWebServer {
private:
    ESP8266WebServer* server;
    String logs[MAX_LOGS];  // 日志数组
    int logIndex = 0;       // 当前日志索引
    int logCount = 0;       // 当前日志数量
    
    void setupRoutes();
    // 页面HTML生成函数
    String getHomeHTML();       // 首页(导航页)
    String getControlHTML();    // 基本控制页面
    String getSensorHTML();     // 传感器监控页面
    String getModeHTML();       // 运动模式配置页面
    String getLogHTML();        // 日志查看页面
    String getSettingsHTML();   // 系统设置页面
    String getAdvancedHTML();   // 高级设置页面
    String getDebugHTML();      // 调试页面
    
    String getCommonStyle();    // 公共CSS样式
    String getCommonScript();   // 公共JavaScript代码
    String getNavBar(const String& currentPage);  // 导航栏
    
    // Route handlers
    void handleRoot();
    void handleControl();
    void handleSensor();
    void handleMode();
    void handleLog();
    void handleSettings();
    void handleAdvanced();
    void handleDebug();

    // API handlers
    void handleSetVW();
    void handleSetMotor();
    void handleSetLED();
    void handleSetBuzzer();
    void handleGetStatus();
    void handleGetVersion();
    void handleSaveSettings();
    void handleSaveMode();
    void handleGetSensorData();
    void handleGetLogData();
    void handleClearLog();
    void handleSaveAdvanced();
    void handleSystemCommand();
    
    // 工具函数
    void addLog(const String& message);  // 添加日志
    String getSystemInfo();    // 获取系统信息JSON
    String getSensorInfo();    // 获取传感器信息JSON
    void clearLogs();         // 清除所有日志
    
public:
    MyWebServer();
    ~MyWebServer();
    void init();
    void handle();
    void log(const String& message) { addLog(message); }
};

#endif // WEBSERVER_H