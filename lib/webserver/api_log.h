#ifndef API_LOG_H
#define API_LOG_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

class LogAPI {
public:
    static void handleGetData(ESP8266WebServer* server, String* logs, int logCount);
    static void handleClearLogs(ESP8266WebServer* server);
};

#endif // API_LOG_H