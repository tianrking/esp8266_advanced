#include "webserver.h"

MyWebServer::MyWebServer() {
    server = new ESP8266WebServer(80);
}

MyWebServer::~MyWebServer() {
    if (server) {
        delete server;
        server = nullptr;
    }
}

void MyWebServer::init() {
    if (!server) {
        Serial.println("Error: Web server not initialized!");
        return;
    }

    // 主页路由
    server->on("/", HTTP_GET, [this]() {
        String html = "<!DOCTYPE html><html><head>";
        html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
        html += "<title>ESP8266 Robot Control</title>";
        html += "<style>";
        html += "body{font-family:system-ui,-apple-system,Arial;margin:0;padding:20px;background:#e0e7ff;color:#1f2937}";
        html += ".card{border-radius:12px;padding:24px;margin:15px 0;background:white;box-shadow:0 2px 4px rgba(0,0,0,0.1)}";
        html += ".btn{color:white;border:none;padding:8px 16px;border-radius:8px;cursor:pointer;margin:5px;font-weight:500}";
        html += ".btn-blue{background:#3b82f6}";
        html += ".btn-green{background:#10b981}";
        html += ".control-item{margin:10px 0;padding:16px;border:1px solid #e5e7eb;border-radius:8px}";
        html += "input{width:80px;padding:8px 12px;margin:5px;border:1px solid #e2e8f0;border-radius:8px}";
        html += "select{padding:8px 12px;margin:5px;border:1px solid #e2e8f0;border-radius:8px}";
        html += "</style></head><body>";

        // 状态卡片
        html += "<div class='card'>";
        html += "<h2>System Status</h2>";
        html += "<div class='control-item'>";
        html += "<p>Memory: " + String(ESP.getFreeHeap()) + " bytes free</p>";
        html += "<p>WiFi: " + String(WiFi.SSID()) + " (" + String(WiFi.RSSI()) + " dBm)</p>";
        html += "<p>IP: " + WiFi.localIP().toString() + "</p>";
        html += "<button class='btn btn-blue' onclick='fetch(\"/api/status\").then(r=>r.json()).then(console.log)'>Refresh Status</button>";
        html += "</div></div>";

        // 控制卡片
        html += "<div class='card'>";
        html += "<h2>Robot Control</h2>";
        html += "<div class='control-item'>";
        html += "<h3>Speed Control</h3>";
        html += "Linear (m/s): <input type='number' id='v_speed' step='0.1' value='0'>";
        html += "Angular (rad/s): <input type='number' id='w_speed' step='0.1' value='0'><br>";
        html += "<button class='btn btn-green' onclick='setSpeed()'>Set Speed</button>";
        html += "</div>";
        
        html += "<div class='control-item'>";
        html += "<h3>Motor Control</h3>";
        html += "Motor: <select id='motor_select'>";
        html += "<option value='1'>Motor 1</option>";
        html += "<option value='2'>Motor 2</option>";
        html += "</select>";
        html += "Speed: <input type='number' id='motor_speed' min='-255' max='255' value='0'><br>";
        html += "<button class='btn btn-green' onclick='setMotor()'>Set Motor</button>";
        html += "</div></div>";

        // JavaScript
        html += "<script>";
        html += "function setSpeed() {";
        html += "  const v = parseFloat(document.getElementById('v_speed').value);";
        html += "  const w = parseFloat(document.getElementById('w_speed').value);";
        html += "  fetch('/api/setVW', {";
        html += "    method: 'POST',";
        html += "    headers: {'Content-Type': 'application/json'},";
        html += "    body: JSON.stringify({v, w})";
        html += "  }).then(r => r.json()).then(console.log);";
        html += "}";
        
        html += "function setMotor() {";
        html += "  const motor = parseInt(document.getElementById('motor_select').value);";
        html += "  const speed = parseInt(document.getElementById('motor_speed').value);";
        html += "  fetch('/api/setMotor', {";
        html += "    method: 'POST',";
        html += "    headers: {'Content-Type': 'application/json'},";
        html += "    body: JSON.stringify({motor, speed})";
        html += "  }).then(r => r.json()).then(console.log);";
        html += "}";
        html += "</script></body></html>";

        server->send(200, "text/html", html);
    });

    // API路由 - 状态
    server->on("/api/status", HTTP_GET, [this]() {
        DynamicJsonDocument doc(1024);
        doc["heap"] = ESP.getFreeHeap();
        doc["wifi_ssid"] = WiFi.SSID();
        doc["wifi_strength"] = WiFi.RSSI();
        doc["ip"] = WiFi.localIP().toString();
        doc["uptime"] = millis() / 1000;
        
        String response;
        serializeJson(doc, response);
        server->send(200, "application/json", response);
    });

    // API路由 - 设置速度
    server->on("/api/setVW", HTTP_POST, [this]() {
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

        // TODO: 实际的电机控制代码
        String response = "{\"status\":\"OK\",\"v\":" + String(v) + ",\"w\":" + String(w) + "}";
        server->send(200, "application/json", response);
    });

    // API路由 - 直接控制电机
    server->on("/api/setMotor", HTTP_POST, [this]() {
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

        int motor = doc["motor"];
        int speed = doc["speed"];

        if (motor != 1 && motor != 2) {
            server->send(400, "application/json", "{\"error\":\"Invalid motor\"}");
            return;
        }

        if (speed < -255 || speed > 255) {
            server->send(400, "application/json", "{\"error\":\"Invalid speed\"}");
            return;
        }

        // TODO: 实际的电机控制代码
        String response = "{\"status\":\"OK\",\"motor\":" + String(motor) + 
                         ",\"speed\":" + String(speed) + "}";
        server->send(200, "application/json", response);
    });

    // 处理CORS
    server->on("/api/setVW", HTTP_OPTIONS, [this]() {
        server->sendHeader("Access-Control-Allow-Origin", "*");
        server->sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
        server->sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
        server->send(204);
    });

    // 处理404
    server->onNotFound([this]() {
        server->send(404, "text/plain", "Not Found");
    });

    server->begin();
    Serial.println("HTTP server started");
}

void MyWebServer::handle() {
    if (server) {
        server->handleClient();
        yield();
    }
}