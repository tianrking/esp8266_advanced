#include "api_mode.h"

// 静态成员初始化
String ModeAPI::currentMode = "manual";
String ModeAPI::modeStatus = "stopped";
float ModeAPI::posX = 0;
float ModeAPI::posY = 0;
float ModeAPI::posTheta = 0;
float ModeAPI::velX = 0;
float ModeAPI::velY = 0;
float ModeAPI::velW = 0;
StaticJsonDocument<1024> ModeAPI::modeParams;

void ModeAPI::handleModeCommand(ESP8266WebServer* server) {
    if (!server->hasArg("plain")) {
        server->send(400, "application/json", "{\"error\":\"No body\"}");
        return;
    }

    String body = server->arg("plain");
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, body);

    if (error) {
        server->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
        return;
    }

    if (doc.containsKey("mode")) {
        String mode = doc["mode"].as<String>();
        setMode(mode);
        
        if (doc.containsKey("params")) {
            JsonObject params = doc["params"].as<JsonObject>();
            setModeParams(mode, params);
        }
    }

    String response = "{\"status\":\"OK\",\"mode\":\"" + currentMode + "\"}";
    server->send(200, "application/json", response);
}

void ModeAPI::handleModeStatus(ESP8266WebServer* server) {
    DynamicJsonDocument doc(1024);
    
    doc["mode"] = currentMode;
    doc["status"] = modeStatus;
    doc["x"] = posX;
    doc["y"] = posY;
    doc["theta"] = posTheta;
    doc["vx"] = velX;
    doc["vy"] = velY;
    doc["angular"] = velW;
    doc["speed"] = sqrt(velX * velX + velY * velY);
    
    String response;
    serializeJson(doc, response);
    server->send(200, "application/json", response);
}

void ModeAPI::setMode(const String& mode) {
    if (mode != currentMode) {
        currentMode = mode;
        modeStatus = "initializing";
        
        // 重置位置和速度
        velX = 0;
        velY = 0;
        velW = 0;
        
        // 根据模式执行相应的初始化
        JsonObject params = modeParams.as<JsonObject>();
        if (mode == "manual") {
            executeManualMode(params);
        } else if (mode == "auto") {
            executeAutoMode(params);
        } else if (mode == "follow") {
            executeFollowMode(params);
        } else if (mode == "avoid") {
            executeAvoidMode(params);
        }
    }
}

void ModeAPI::setModeParams(const String& mode, const JsonObject& params) {
    // 保存参数
    modeParams.clear();
    modeParams.set(params);
    
    // 根据模式更新参数
    if (mode == "manual") {
        executeManualMode(params);
    } else if (mode == "auto") {
        executeAutoMode(params);
    } else if (mode == "follow") {
        executeFollowMode(params);
    } else if (mode == "avoid") {
        executeAvoidMode(params);
    }
}

void ModeAPI::updatePosition(float x, float y, float theta) {
    posX = x;
    posY = y;
    posTheta = theta;
}

void ModeAPI::updateVelocity(float vx, float vy, float w) {
    velX = vx;
    velY = vy;
    velW = w;
}

// 模式执行函数
void ModeAPI::executeManualMode(const JsonObject& params) {
    if (params.containsKey("maxSpeed")) {
        int maxSpeed = params["maxSpeed"];
        // TODO: 设置最大速度
    }
    if (params.containsKey("ramp")) {
        int ramp = params["ramp"];
        // TODO: 设置加速斜率
    }
    if (params.containsKey("deadzone")) {
        int deadzone = params["deadzone"];
        // TODO: 设置死区
    }
    modeStatus = "ready";
}

void ModeAPI::executeAutoMode(const JsonObject& params) {
    if (params.containsKey("navMode")) {
        String navMode = params["navMode"].as<String>();
        // TODO: 设置导航算法
    }
    if (params.containsKey("targetX") && params.containsKey("targetY")) {
        int targetX = params["targetX"];
        int targetY = params["targetY"];
        // TODO: 设置目标位置
    }
    if (params.containsKey("safeDist")) {
        int safeDist = params["safeDist"];
        // TODO: 设置安全距离
    }
    modeStatus = "ready";
}

void ModeAPI::executeFollowMode(const JsonObject& params) {
    if (params.containsKey("baseSpeed")) {
        int baseSpeed = params["baseSpeed"];
        // TODO: 设置基础速度
    }
    if (params.containsKey("kp") && params.containsKey("ki") && params.containsKey("kd")) {
        float kp = params["kp"];
        float ki = params["ki"];
        float kd = params["kd"];
        // TODO: 设置PID参数
    }
    modeStatus = "ready";
}

void ModeAPI::executeAvoidMode(const JsonObject& params) {
    if (params.containsKey("range")) {
        int range = params["range"];
        // TODO: 设置检测范围
    }

    if (params.containsKey("turnSpeed")) {
        int turnSpeed = params["turnSpeed"];
        // TODO: 设置转向速度
    }
    if (params.containsKey("safeDist")) {
        int safeDist = params["safeDist"];
        // TODO: 设置安全距离
    }
    if (params.containsKey("strategy")) {
        String strategy = params["strategy"].as<String>();
        // TODO: 设置避障策略
    }
    modeStatus = "ready";
}