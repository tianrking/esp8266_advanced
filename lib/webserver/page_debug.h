#ifndef PAGE_DEBUG_H
#define PAGE_DEBUG_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "html_common.h"

class DebugPage {
public:
    static void handleDebug(ESP8266WebServer* server);
    
private:
    static String getConsoleCard();          // 调试控制台
    static String getSystemTestCard();       // 系统测试面板
    static String getMemoryMonitorCard();    // 内存监视器
    static String getNetworkMonitorCard();   // 网络监视器
    static String getPerformanceCard();      // 性能监视器
    static String getDebugControlCard();     // 调试控制面板
    static String getDebugScript();          // 调试页面脚本
};

#endif // PAGE_DEBUG_H