#include "api_control.h"

// 静态成员初始化
float ControlAPI::currentV = 0;
float ControlAPI::currentW = 0;
int ControlAPI::motor1Speed = 0;
int ControlAPI::motor2Speed = 0;
bool ControlAPI::ledState = false;
String ControlAPI::buzzerMode = "off";

void ControlAPI::handleSetVW(ESP8266WebServer* server) {
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

    float v = doc["v"];
    float w = doc["w"];
    
    // 计算左右轮速度
    float wheel_separation = 0.2;  // 轮距，单位：米
    float left_speed = (v - w * wheel_separation / 2);
    float right_speed = (v + w * wheel_separation / 2);

    // 将速度转换为PWM值（-255到255）
    int left_pwm = constrain(left_speed * 255, -255, 255);
    int right_pwm = constrain(right_speed * 255, -255, 255);

    // 保存当前状态
    currentV = v;
    currentW = w;
    motor1Speed = left_pwm;
    motor2Speed = right_pwm;

    // TODO: 实际控制电机
    // setMotorSpeed(1, left_pwm);
    // setMotorSpeed(2, right_pwm);

    String response = "{\"status\":\"OK\",\"left_pwm\":" + String(left_pwm) + 
                     ",\"right_pwm\":" + String(right_pwm) + "}";
    server->send(200, "application/json", response);
}

void ControlAPI::handleSetMotor(ESP8266WebServer* server) {
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

    int motorId = doc["motor"];
    int speed = doc["speed"];

    // 验证参数
    if (motorId != 1 && motorId != 2) {
        server->send(400, "application/json", "{\"error\":\"Invalid motor ID\"}");
        return;
    }

    if (speed < -255 || speed > 255) {
        server->send(400, "application/json", "{\"error\":\"Invalid speed value\"}");
        return;
    }

    // 保存电机速度
    if (motorId == 1) motor1Speed = speed;
    else motor2Speed = speed;

    // TODO: 实际控制电机
    // setMotorSpeed(motorId, speed);

    String response = "{\"status\":\"OK\",\"motor\":" + String(motorId) + 
                     ",\"speed\":" + String(speed) + "}";
    server->send(200, "application/json", response);
}

void ControlAPI::handleSetLED(ESP8266WebServer* server) {
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

    String mode = doc["mode"].as<String>();
    
    // 更新LED状态
    if (mode == "on") ledState = true;
    else if (mode == "off") ledState = false;
    // blink模式需要在主循环中处理
    
    // TODO: 实际控制LED
    // digitalWrite(LED_PIN, ledState);

    String response = "{\"status\":\"OK\",\"led_mode\":\"" + mode + "\"}";
    server->send(200, "application/json", response);
}

void ControlAPI::handleSetBuzzer(ESP8266WebServer* server) {
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

    String mode = doc["mode"].as<String>();
    buzzerMode = mode;

    // TODO: 实际控制蜂鸣器
    if (mode == "beep") {
        // 短鸣
        // tone(BUZZER_PIN, 1000, 100);
    } else if (mode == "alarm") {
        // 报警声
        // tone(BUZZER_PIN, 2000, 500);
    } else {
        // 关闭
        // noTone(BUZZER_PIN);
    }

    String response = "{\"status\":\"OK\",\"buzzer_mode\":\"" + mode + "\"}";
    server->send(200, "application/json", response);
}