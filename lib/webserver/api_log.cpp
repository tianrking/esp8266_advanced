#include "api_log.h"

void LogAPI::handleGetData(ESP8266WebServer* server, String* logs, int logCount) {
    DynamicJsonDocument doc(4096);
    JsonArray array = doc.to<JsonArray>();
    
    for(int i = 0; i < logCount; i++) {
        array.add(logs[i]);
    }
    
    String response;
    serializeJson(doc, response);
    server->send(200, "application/json", response);
}

void LogAPI::handleClearLogs(ESP8266WebServer* server) {
    server->send(200, "application/json", "{\"status\":\"OK\"}");
}