#include "api_sensor.h"

// 静态成员初始化
float SensorAPI::imu_ax = 0;
float SensorAPI::imu_ay = 0;
float SensorAPI::imu_az = 0;
float SensorAPI::imu_gx = 0;
float SensorAPI::imu_gy = 0;
float SensorAPI::imu_gz = 0;

int SensorAPI::motor1_speed = 0;
int SensorAPI::motor2_speed = 0;

float SensorAPI::battery_voltage = 0;
float SensorAPI::battery_current = 0;

float SensorAPI::temperature = 0;
float SensorAPI::humidity = 0;

void SensorAPI::handleGetData(ESP8266WebServer* server) {
    DynamicJsonDocument doc(1024);
    
    // IMU数据
    JsonObject imu = doc.createNestedObject("imu");
    imu["ax"] = imu_ax;
    imu["ay"] = imu_ay;
    imu["az"] = imu_az;
    imu["gx"] = imu_gx;
    imu["gy"] = imu_gy;
    imu["gz"] = imu_gz;
    
    // 电机状态
    JsonObject motors = doc.createNestedObject("motors");
    motors["m1"] = motor1_speed;
    motors["m2"] = motor2_speed;
    
    // 电池状态
    JsonObject battery = doc.createNestedObject("battery");
    battery["voltage"] = battery_voltage;
    battery["current"] = battery_current;
    
    // 环境数据
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    
    String response;
    serializeJson(doc, response);
    server->send(200, "application/json", response);
}

void SensorAPI::updateIMU(float ax, float ay, float az, float gx, float gy, float gz) {
    imu_ax = ax;
    imu_ay = ay;
    imu_az = az;
    imu_gx = gx;
    imu_gy = gy;
    imu_gz = gz;
}

void SensorAPI::updateMotorStatus(int m1_speed, int m2_speed) {
    motor1_speed = m1_speed;
    motor2_speed = m2_speed;
}

void SensorAPI::updateBatteryStatus(float voltage, float current) {
    battery_voltage = voltage;
    battery_current = current;
}

void SensorAPI::updateEnvironment(float temp, float humid) {
    temperature = temp;
    humidity = humid;
}