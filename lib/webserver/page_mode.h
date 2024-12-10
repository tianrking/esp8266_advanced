#ifndef PAGE_MODE_H
#define PAGE_MODE_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "html_common.h"

class ModePage {
public:
    static void handleMode(ESP8266WebServer* server);
    
private:
    static String getModeSelectionCard();
    static String getParameterCard();
    static String getTrajectoryCard();
    static String getStatusCard();
    static String getModeScript();
};

#endif // PAGE_MODE_H