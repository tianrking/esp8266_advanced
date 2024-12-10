#ifndef API_MODE_H
#define API_MODE_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

class ModeAPI {
public:
    static void handleModeCommand(ESP8266WebServer* server);
    static void handleModeStatus(ESP8266WebServer* server);
    
    // 运动模式管理
    static void setMode(const String& mode);
    static void setModeParams(const String& mode, const JsonObject& params);
    static String getCurrentMode() { return currentMode; }
    
    // 运动状态管理
    static void updatePosition(float x, float y, float theta);
    static void updateVelocity(float vx, float vy, float w);
    
private:
    static String currentMode;
    static String modeStatus;
    static float posX, posY, posTheta;
    static float velX, velY, velW;
    static StaticJsonDocument<1024> modeParams;
    
    static void executeManualMode(const JsonObject& params);
    static void executeAutoMode(const JsonObject& params);
    static void executeFollowMode(const JsonObject& params);
    static void executeAvoidMode(const JsonObject& params);
};

#endif // API_MODE_H