#ifndef PAGE_SENSOR_H
#define PAGE_SENSOR_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "html_common.h"

class SensorPage {
public:
    static void handleSensor(ESP8266WebServer* server);
    
private:
    static String getSensorDataCard();
    static String getMotorStatusCard();
    static String getBatteryStatusCard();
    static String getIMUDataCard();
    static String getSensorScript();
};

#endif // PAGE_SENSOR_H