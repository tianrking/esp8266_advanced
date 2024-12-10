#ifndef API_SENSOR_H
#define API_SENSOR_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

class SensorAPI {
public:
    static void handleGetData(ESP8266WebServer* server);
    
    // 传感器数据更新函数
    static void updateIMU(float ax, float ay, float az, float gx, float gy, float gz);
    static void updateMotorStatus(int m1_speed, int m2_speed);
    static void updateBatteryStatus(float voltage, float current);
    static void updateEnvironment(float temp, float humidity);
    
private:
    // IMU数据
    static float imu_ax, imu_ay, imu_az;
    static float imu_gx, imu_gy, imu_gz;
    
    // 电机状态
    static int motor1_speed, motor2_speed;
    
    // 电池状态
    static float battery_voltage;
    static float battery_current;
    
    // 环境数据
    static float temperature;
    static float humidity;
};

#endif // API_SENSOR_H