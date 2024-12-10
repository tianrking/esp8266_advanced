#include "api_settings.h"

Settings SettingsAPI::settings;

void SettingsAPI::init() {
    readFromEEPROM();
    if (!isValid()) {
        loadDefaults();
        writeToEEPROM();
    }
}

void SettingsAPI::handleLoadSettings(ESP8266WebServer* server) {
    DynamicJsonDocument doc(1024);
    
    // WiFi设置
    JsonObject wifi = doc.createNestedObject("wifi");
    wifi["ssid"] = settings.ssid;
    wifi["staticIP"] = settings.staticIP;
    wifi["ip"] = String(settings.ip[0]) + "." + String(settings.ip[1]) + "." + 
                 String(settings.ip[2]) + "." + String(settings.ip[3]);
    wifi["gateway"] = String(settings.gateway[0]) + "." + String(settings.gateway[1]) + "." + 
                     String(settings.gateway[2]) + "." + String(settings.gateway[3]);
    wifi["subnet"] = String(settings.subnet[0]) + "." + String(settings.subnet[1]) + "." + 
                    String(settings.subnet[2]) + "." + String(settings.subnet[3]);
    
    // 电机设置
    JsonObject motor = doc.createNestedObject("motor");
    motor["motor1_max"] = settings.motor1_max;
    motor["motor2_max"] = settings.motor2_max;
    motor["deadzone"] = settings.motor_deadzone;
    motor["ramp"] = settings.motor_ramp;
    
    // 传感器设置
    JsonObject sensor = doc.createNestedObject("sensor");
    sensor["update_interval"] = settings.update_interval;
    sensor["auto_update"] = settings.auto_update;
    sensor["filter_level"] = settings.filter_level;
    
    // 系统设置
    JsonObject system = doc.createNestedObject("system");
    system["log_level"] = settings.log_level;
    system["max_logs"] = settings.max_logs;
    system["watchdog_enabled"] = settings.watchdog_enabled;
    system["watchdog_timeout"] = settings.watchdog_timeout;
    
    String response;
    serializeJson(doc, response);
    server->send(200, "application/json", response);
}

void SettingsAPI::handleSaveSettings(ESP8266WebServer* server) {
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

    // 解析WiFi设置
    if (doc.containsKey("wifi")) {
        JsonObject wifi = doc["wifi"];
        if (wifi.containsKey("ssid")) {
            strncpy(settings.ssid, wifi["ssid"], sizeof(settings.ssid));
        }
        if (wifi.containsKey("password")) {
            strncpy(settings.password, wifi["password"], sizeof(settings.password));
        }
        if (wifi.containsKey("staticIP")) {
            settings.staticIP = wifi["staticIP"];
        }
        if (wifi.containsKey("ip")) {
            String ip = wifi["ip"];
            sscanf(ip.c_str(), "%hhu.%hhu.%hhu.%hhu", 
                   &settings.ip[0], &settings.ip[1], &settings.ip[2], &settings.ip[3]);
        }
        // 同样解析gateway和subnet
    }

    // 解析电机设置
    if (doc.containsKey("motor")) {
        JsonObject motor = doc["motor"];
        if (motor.containsKey("motor1_max")) settings.motor1_max = motor["motor1_max"];
        if (motor.containsKey("motor2_max")) settings.motor2_max = motor["motor2_max"];
        if (motor.containsKey("deadzone")) settings.motor_deadzone = motor["deadzone"];
        if (motor.containsKey("ramp")) settings.motor_ramp = motor["ramp"];
    }

    // 解析传感器设置
    if (doc.containsKey("sensor")) {
        JsonObject sensor = doc["sensor"];
        if (sensor.containsKey("update_interval")) settings.update_interval = sensor["update_interval"];
        if (sensor.containsKey("auto_update")) settings.auto_update = sensor["auto_update"];
        if (sensor.containsKey("filter_level")) settings.filter_level = sensor["filter_level"];
    }

    // 解析系统设置
    if (doc.containsKey("system")) {
        JsonObject system = doc["system"];
        if (system.containsKey("log_level")) settings.log_level = system["log_level"];
        if (system.containsKey("max_logs")) settings.max_logs = system["max_logs"];
        if (system.containsKey("watchdog_enabled")) settings.watchdog_enabled = system["watchdog_enabled"];
        if (system.containsKey("watchdog_timeout")) settings.watchdog_timeout = system["watchdog_timeout"];
    }

    // 保存设置
    writeToEEPROM();

    server->send(200, "application/json", "{\"status\":\"OK\"}");
}

void SettingsAPI::handleResetSettings(ESP8266WebServer* server) {
    loadDefaults();
    writeToEEPROM();
    server->send(200, "application/json", "{\"status\":\"OK\"}");
}

void SettingsAPI::loadDefaults() {
    // WiFi默认设置
    strcpy(settings.ssid, "ESP8266_Robot");
    strcpy(settings.password, "");
    settings.staticIP = false;
    settings.ip[0] = 192; settings.ip[1] = 168; settings.ip[2] = 4; settings.ip[3] = 1;
    settings.gateway[0] = 192; settings.gateway[1] = 168; settings.gateway[2] = 4; settings.gateway[3] = 1;
    settings.subnet[0] = 255; settings.subnet[1] = 255; settings.subnet[2] = 255; settings.subnet[3] = 0;
    
    // 电机默认设置
    settings.motor1_max = 255;
    settings.motor2_max = 255;
    settings.motor_deadzone = 10;
    settings.motor_ramp = 5;
    
    // 传感器默认设置
    settings.update_interval = 100;
    settings.auto_update = true;
    settings.filter_level = 1;
    
    // 系统默认设置
    settings.log_level = 2;  // INFO级别
    settings.max_logs = 100;
    settings.watchdog_enabled = true;
    settings.watchdog_timeout = 1000;
    
    // 设置有效标记
    settings.valid_flag = VALID_FLAG_VALUE;
}

void SettingsAPI::readFromEEPROM() {
    EEPROM.get(SETTINGS_ADDR, settings);
}

void SettingsAPI::writeToEEPROM() {
    EEPROM.put(SETTINGS_ADDR, settings);
    EEPROM.commit();
}