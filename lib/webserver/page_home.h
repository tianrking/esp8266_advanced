#ifndef PAGE_HOME_H
#define PAGE_HOME_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "html_common.h"

class HomePage {
public:
    static void handleRoot(ESP8266WebServer* server);
private:
    static String getWelcomeCard();
    static String getQuickAccessCard();
    static String getUptimeScript();
};

#endif // PAGE_HOME_H