#ifndef PAGE_LOG_H
#define PAGE_LOG_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "html_common.h"

class LogPage {
public:
    static void handleLog(ESP8266WebServer* server);
    
private:
    static String getLogDisplayCard();
    static String getLogFilterCard();
    static String getLogSettingsCard();
    static String getLogScript();
};

#endif // PAGE_LOG_H