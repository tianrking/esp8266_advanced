#ifndef PAGE_CONTROL_H
#define PAGE_CONTROL_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "html_common.h"

class ControlPage {
public:
    static void handleControl(ESP8266WebServer* server);
    
private:
    static String getStatusCard();
    static String getSpeedControlCard();
    static String getMotorControlCard();
    static String getLEDControlCard();
    static String getBuzzerControlCard();
    static String getControlScript();
};

#endif // PAGE_CONTROL_H