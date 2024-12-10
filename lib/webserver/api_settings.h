#ifndef API_SETTINGS_H
#define API_SETTINGS_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

// 设置结构体
struct Settings {
    // WiFi设置
    char ssid[32];
    char password[64];
    bool staticIP;
    uint8_t ip[4];
    uint8_t gateway[4];
    uint8_t subnet[4];
    
    // 电机设置
    uint8_t motor1_max;
    uint8_t motor2_max;
    uint8_t motor_deadzone;
    uint8_t motor_ramp;
    
    // 传感器设置
    uint16_t update_interval;
    bool auto_update;
    uint8_t filter_level;
    
    // 系统设置
    uint8_t log_level;
    uint16_t max_logs;
    bool watchdog_enabled;
    uint16_t watchdog_timeout;
    
    // 验证标记
    uint32_t valid_flag;
};

class SettingsAPI {
public:
    static void init();
    static void handleLoadSettings(ESP8266WebServer* server);
    static void handleSaveSettings(ESP8266WebServer* server);
    static void handleResetSettings(ESP8266WebServer* server);
    
    // 获取设置值的辅助函数
    static Settings& getSettings() { return settings; }
    static bool isValid() { return settings.valid_flag == VALID_FLAG_VALUE; }
    
private:
    static const uint32_t VALID_FLAG_VALUE = 0xABCD1234;  // 有效标记值
    static const int SETTINGS_ADDR = 0;  // EEPROM中的存储地址
    static Settings settings;  // 当前设置
    
    static void loadDefaults();  // 加载默认设置
    static void readFromEEPROM();  // 从EEPROM读取设置
    static void writeToEEPROM();  // 写入设置到EEPROM
};

#endif // API_SETTINGS_H