#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

class MyWebServer {
private:
    ESP8266WebServer* server;
    
    void setupRoutes();
    String getHomeHTML();      // 首页(导航页)
    String getControlHTML();   // 控制页面
    String getSettingsHTML();  // 设置页面
    String getCommonStyle();   // 公共CSS样式
    String getCommonScript();  // 公共JavaScript代码
    
    // Route handlers
    void handleRoot();
    void handleControl();
    void handleSettings();
    void handleSetVW();
    void handleSetMotor();
    void handleSetLED();
    void handleSetBuzzer();
    void handleGetStatus();
    void handleGetVersion();
    void handleSaveSettings();
    
public:
    MyWebServer();
    ~MyWebServer();
    void init();
    void handle();
};

#endif // WEBSERVER_H