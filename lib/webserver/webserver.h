#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

class MyWebServer {
private:
    ESP8266WebServer* server;
    void setupRoutes();
    String getHTML();
    
    // Route handlers
    void handleRoot();
    void handleSetVW();
    void handleSetMotor();
    void handleSetLED();
    void handleSetBuzzer();
    void handleGetStatus();
    void handleGetVersion();
    
public:
    MyWebServer();
    ~MyWebServer();
    void init();
    void handle();
};

#endif // WEBSERVER_H