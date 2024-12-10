#ifndef PAGE_ADVANCED_H
#define PAGE_ADVANCED_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "html_common.h"

class AdvancedPage {
public:
    static void handleAdvanced(ESP8266WebServer* server);
    
private:
    static String getSystemInfoCard();
    static String getNetworkConfigCard();
    static String getFirmwareUpdateCard();
    static String getCalibrationCard();
    static String getCustomCommandCard();
    static String getDataVisualizationCard();
    static String getAdvancedScript();
};

#endif // PAGE_ADVANCED_H