#ifndef PAGE_SETTINGS_H
#define PAGE_SETTINGS_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "html_common.h"
#include "api_settings.h"

class SettingsPage {
public:
    static void handleSettings(ESP8266WebServer* server);
    
private:
    static String getWiFiSettingsCard();
    static String getMotorSettingsCard();
    static String getSensorSettingsCard();
    static String getSystemSettingsCard();
    static String getSettingsScript();
};

#endif // PAGE_SETTINGS_H