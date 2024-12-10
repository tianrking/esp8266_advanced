#ifndef API_CONTROL_H
#define API_CONTROL_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

class ControlAPI {
public:
    static void handleSetVW(ESP8266WebServer* server);
    static void handleSetMotor(ESP8266WebServer* server);
    static void handleSetLED(ESP8266WebServer* server);
    static void handleSetBuzzer(ESP8266WebServer* server);
    
private:
    // 存储当前的控制状态
    static float currentV;   // 当前线速度
    static float currentW;   // 当前角速度
    static int motor1Speed;  // 电机1速度
    static int motor2Speed;  // 电机2速度
    static bool ledState;    // LED状态
    static String buzzerMode;// 蜂鸣器模式
};

#endif // API_CONTROL_H