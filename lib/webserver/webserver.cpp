#include "webserver.h"

MyWebServer::MyWebServer() {
    server = new ESP8266WebServer(80);
}

MyWebServer::~MyWebServer() {
    delete server;
}

void MyWebServer::init() {
    setupRoutes();
    server->begin();
    Serial.println("HTTP server started");
}

void MyWebServer::handle() {
    server->handleClient();
}

void MyWebServer::setupRoutes() {
    server->on("/", HTTP_GET, [this]() { this->handleRoot(); });
    server->on("/control", HTTP_GET, [this]() { this->handleControl(); });
    server->on("/settings", HTTP_GET, [this]() { this->handleSettings(); });
    
    // API routes
    server->on("/api/setVW", HTTP_POST, [this]() { this->handleSetVW(); });
    server->on("/api/setMotor", HTTP_POST, [this]() { this->handleSetMotor(); });
    server->on("/api/setLED", HTTP_POST, [this]() { this->handleSetLED(); });
    server->on("/api/setBuzzer", HTTP_POST, [this]() { this->handleSetBuzzer(); });
    server->on("/api/status", HTTP_GET, [this]() { this->handleGetStatus(); });
    server->on("/api/version", HTTP_GET, [this]() { this->handleGetVersion(); });
    server->on("/api/settings/save", HTTP_POST, [this]() { this->handleSaveSettings(); });

    // Handle CORS
    server->on("/api/setVW", HTTP_OPTIONS, [this]() {
        server->sendHeader("Access-Control-Allow-Origin", "*");
        server->sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
        server->sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
        server->send(204);
    });
}

// 返回公共CSS样式
String MyWebServer::getCommonStyle() {
    String style = "<style>";
    style += "body{font-family:system-ui,-apple-system,Arial;margin:0;padding:0;background:#e0e7ff;color:#1f2937}";
    style += "nav{background:#1e40af;padding:1rem;color:white}";
    style += "nav a{color:white;text-decoration:none;padding:0.5rem 1rem;margin:0 0.5rem;border-radius:0.5rem}";
    style += "nav a:hover{background:rgba(255,255,255,0.2)}";
    style += "nav a.active{background:rgba(255,255,255,0.3)}";
    style += ".container{padding:20px}";
    style += "h2{margin:0 0 16px 0;font-size:1.5rem;font-weight:600}";
    style += ".card{border-radius:12px;padding:24px;margin:15px 0;transition:all 0.3s;box-shadow:0 2px 4px rgba(0,0,0,0.04)}";
    style += ".card-status{background:linear-gradient(145deg,#fff,#f1f5f9)}";
    style += ".card-speed{background:linear-gradient(145deg,#fff,#f0fdf4)}";
    style += ".card-motor{background:linear-gradient(145deg,#fff,#f0f9ff)}";
    style += ".card-led{background:linear-gradient(145deg,#fff,#fdf4ff)}";
    style += ".card-buzzer{background:linear-gradient(145deg,#fff,#fff7ed)}";
    style += "input{width:80px;padding:8px 12px;margin:5px;border:1px solid #e2e8f0;border-radius:8px;background:white}";
    style += "input:focus{outline:none;border-color:#93c5fd;box-shadow:0 0 0 3px rgba(147,197,253,0.2)}";
    style += "select{padding:8px 12px;margin:5px;border:1px solid #e2e8f0;border-radius:8px;background:white}";
    style += ".btn{color:white;border:none;padding:8px 16px;border-radius:8px;cursor:pointer;margin:5px;font-weight:500;transition:all 0.2s}";
    style += ".btn:hover{transform:translateY(-1px);filter:brightness(110%)}";
    style += ".btn-status{background:#3b82f6}";
    style += ".btn-speed{background:#10b981}";
    style += ".btn-motor{background:#6366f1}";
    style += ".btn-led{background:#8b5cf6}";
    style += ".btn-buzzer{background:#f59e0b}";
    style += ".control-item{margin:10px 0;padding:16px;border:1px solid rgba(255,255,255,0.3);border-radius:8px;background:rgba(255,255,255,0.3)}";
    style += ".status{background:rgba(255,255,255,0.3);padding:16px;margin:10px 0;border-radius:8px;font-family:monospace}";
    style += "</style>";
    return style;
}

// 返回公共JavaScript代码
String MyWebServer::getCommonScript() {
    String script = "<script>";
    script += "function updateStatus(text){document.getElementById('status').innerText=JSON.stringify(text,null,2)}";
    script += "function getStatus(){fetch('/api/status')";
    script += ".then(response=>response.json())";
    script += ".then(data=>updateStatus(data))";
    script += ".catch(error=>updateStatus('Error: '+error));}";
    script += "function getVersion(){fetch('/api/version')";
    script += ".then(response=>response.json())";
    script += ".then(data=>updateStatus(data))";
    script += ".catch(error=>updateStatus('Error: '+error));}";
    script += "</script>";
    return script;
}

// 首页 HTML
void MyWebServer::handleRoot() {
    server->send(200, "text/html", getHomeHTML());
}

String MyWebServer::getHomeHTML() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>ESP8266 Robot</title>";
    html += getCommonStyle();
    html += "</head><body>";
    
    html += "<nav>";
    html += "<a href='/' class='active'>Home</a>";
    html += "<a href='/control'>Control</a>";
    html += "<a href='/settings'>Settings</a>";
    html += "</nav>";
    
    html += "<div class='container'>";
    html += "<div class='card card-status'>";
    html += "<h2>ESP8266 Robot Control System</h2>";
    html += "<p>Welcome to the robot control system. Please select an option:</p>";
    html += "<a href='/control' class='btn btn-status'>Robot Control</a> ";
    html += "<a href='/settings' class='btn btn-motor'>System Settings</a>";
    html += "</div>";
    
    html += "<div class='card'>";
    html += "<h2>System Information</h2>";
    html += "<p>IP Address: " + WiFi.localIP().toString() + "</p>";
    html += "<p>MAC Address: " + WiFi.macAddress() + "</p>";
    html += "<p>Free Memory: " + String(ESP.getFreeHeap()) + " bytes</p>";
    html += "</div>";
    html += "</div>";
    
    html += "</body></html>";
    return html;
}

// 控制页面 HTML
void MyWebServer::handleControl() {
    server->send(200, "text/html", getControlHTML());
}

String MyWebServer::getControlHTML() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>Robot Control</title>";
    html += getCommonStyle();
    html += "</head><body>";
    
    html += "<nav>";
    html += "<a href='/'>Home</a>";
    html += "<a href='/control' class='active'>Control</a>";
    html += "<a href='/settings'>Settings</a>";
    html += "</nav>";
    
    html += "<div class='container'>";
    
    // Status Card
    html += "<div class='card card-status'>";
    html += "<h2 style='color:#2563eb'>System Status</h2>";
    html += "<button class='btn btn-status' onclick='getStatus()'>Get Status</button>";
    html += "<button class='btn btn-status' onclick='getVersion()'>Get Version</button>";
    html += "<pre id='status' class='status'>Status will appear here...</pre>";
    html += "</div>";

    // Speed Control Card
    html += "<div class='card card-speed'>";
    html += "<h2 style='color:#059669'>Speed Control (V,W)</h2>";
    html += "<div class='control-item'>";
    html += "Linear (m/s): <input type='number' id='v_speed' step='0.1' value='0'>";
    html += "Angular (rad/s): <input type='number' id='w_speed' step='0.1' value='0'>";
    html += "<button class='btn btn-speed' onclick='setVW()'>Set Speed</button>";
    html += "</div></div>";

    // Motor Control Card
    html += "<div class='card card-motor'>";
    html += "<h2 style='color:#4f46e5'>Direct Motor Control</h2>";
    html += "<div class='control-item'>";
    html += "Motor: <select id='motor_select'>";
    html += "<option value='1'>Motor 1</option>";
    html += "<option value='2'>Motor 2</option>";
    html += "</select>";
    html += "Speed: <input type='number' id='motor_speed' min='-255' max='255' value='0'>";
    html += "<button class='btn btn-motor' onclick='setMotor()'>Set Motor</button>";
    html += "</div></div>";

    // LED Control Card
    html += "<div class='card card-led'>";
    html += "<h2 style='color:#7c3aed'>LED Control</h2>";
    html += "<div class='control-item'>";
    html += "Mode: <select id='led_mode'>";
    html += "<option value='off'>OFF</option>";
    html += "<option value='on'>ON</option>";
    html += "<option value='blink'>BLINK</option>";
    html += "</select>";
    html += "<button class='btn btn-led' onclick='setLED()'>Set LED</button>";
    html += "</div></div>";

    // Buzzer Control Card
    html += "<div class='card card-buzzer'>";
    html += "<h2 style='color:#d97706'>Buzzer Control</h2>";
    html += "<div class='control-item'>";
    html += "Mode: <select id='buzzer_mode'>";
    html += "<option value='off'>OFF</option>";
    html += "<option value='beep'>BEEP</option>";
    html += "<option value='alarm'>ALARM</option>";
    html += "</select>";
    html += "<button class='btn btn-buzzer' onclick='setBuzzer()'>Set Buzzer</button>";
    html += "</div></div>";
    
    html += "</div>";
    
    // JavaScript
    html += "<script>";
    html += "function setVW(){";
    html += "const v=parseFloat(document.getElementById('v_speed').value);";
    html += "const w=parseFloat(document.getElementById('w_speed').value);";
    html += "fetch('/api/setVW',{method:'POST',headers:{'Content-Type':'application/json'},";
    html += "body:JSON.stringify({v:v,w:w})}).then(response=>response.json())";
    html += ".then(data=>updateStatus(data)).catch(error=>updateStatus('Error: '+error));}";
    
    html += "function setMotor(){";
    html += "const motor=parseInt(document.getElementById('motor_select').value);";
    html += "const speed=parseInt(document.getElementById('motor_speed').value);";
    html += "fetch('/api/setMotor',{method:'POST',headers:{'Content-Type':'application/json'},";
    html += "body:JSON.stringify({motor:motor,speed:speed})}).then(response=>response.json())";
    html += ".then(data=>updateStatus(data)).catch(error=>updateStatus('Error: '+error));}";
    
    html += "function setLED(){";
    html += "const mode=document.getElementById('led_mode').value;";
    html += "fetch('/api/setLED',{method:'POST',headers:{'Content-Type':'application/json'},";
    html += "body:JSON.stringify({mode:mode})}).then(response=>response.json())";
    html += ".then(data=>updateStatus(data)).catch(error=>updateStatus('Error: '+error));}";
    
    html += "function setBuzzer(){";
    html += "const mode=document.getElementById('buzzer_mode').value;";
    html += "fetch('/api/setBuzzer',{method:'POST',headers:{'Content-Type':'application/json'},";
    html += "body:JSON.stringify({mode:mode})}).then(response=>response.json())";
    html += ".then(data=>updateStatus(data)).catch(error=>updateStatus('Error: '+error));}";
    
    html += getCommonScript();
    html += "getStatus();"; // 页面加载时获取初始状态
    html += "</script></body></html>";
    
    return html;
}

// 设置页面 HTML
void MyWebServer::handleSettings() {
    server->send(200, "text/html", getSettingsHTML());
}

String MyWebServer::getSettingsHTML() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>System Settings</title>";
    html += getCommonStyle();
    html += "</head><body>";
    
    html += "<nav>";
    html += "<a href='/'>Home</a>";
    html += "<a href='/control'>Control</a>";
    html += "<a href='/settings' class='active'>Settings</a>";
    html += "</nav>";
    
    html += "<div class='container'>";
    
    // 系统信息卡片
    html += "<div class='card card-status'>";
    html += "<h2>System Information</h2>";
    html += "<div class='control-item'>";
    html += "<p>Device ID: ESP8266_Robot</p>";
    html += "<p>Firmware Version: 1.0.0</p>";
    html += "<p>Build Date: " + String(__DATE__) + "</p>";
    html += "<p>Build Time: " + String(__TIME__) + "</p>";
    html += "<p>Free Heap: " + String(ESP.getFreeHeap()) + " bytes</p>";
    html += "<p>CPU Frequency: " + String(ESP.getCpuFreqMHz()) + " MHz</p>";
    html += "</div></div>";
    
    // 网络设置卡片
    html += "<div class='card card-motor'>";
    html += "<h2>Network Settings</h2>";
    html += "<div class='control-item'>";
    html += "<p>MAC Address: " + WiFi.macAddress() + "</p>";
    html += "<p>IP Address: " + WiFi.localIP().toString() + "</p>";
    html += "<p>Subnet Mask: " + WiFi.subnetMask().toString() + "</p>";
    html += "<p>Gateway IP: " + WiFi.gatewayIP().toString() + "</p>";
    html += "<p>RSSI: " + String(WiFi.RSSI()) + " dBm</p>";
    html += "</div></div>";
    
    // 电机设置卡片
    html += "<div class='card card-speed'>";
    html += "<h2>Motor Settings</h2>";
    html += "<div class='control-item'>";
    html += "Motor 1 - Max Speed: <input type='number' id='motor1_max' value='255'><br>";
    html += "Motor 2 - Max Speed: <input type='number' id='motor2_max' value='255'><br>";
    html += "<button class='btn btn-speed' onclick='saveMotorSettings()'>Save Motor Settings</button>";
    html += "</div></div>";
    
    // 安全设置卡片
    html += "<div class='card card-led'>";
    html += "<h2>Safety Settings</h2>";
    html += "<div class='control-item'>";
    html += "Timeout (ms): <input type='number' id='timeout' value='1000'><br>";
    html += "Enable Watchdog: <input type='checkbox' id='watchdog' checked><br>";
    html += "<button class='btn btn-led' onclick='saveSafetySettings()'>Save Safety Settings</button>";
    html += "</div></div>";
    
    // 固件更新卡片
    html += "<div class='card card-buzzer'>";
    html += "<h2>Firmware Update</h2>";
    html += "<div class='control-item'>";
    html += "<form method='POST' action='/update' enctype='multipart/form-data'>";
    html += "Firmware file: <input type='file' name='update'><br>";
    html += "<button class='btn btn-buzzer' type='submit'>Update Firmware</button>";
    html += "</form></div></div>";
    
    html += "</div>";
    
    // JavaScript
    html += "<script>";
    // 保存电机设置
    html += "function saveMotorSettings(){";
    html += "const motor1_max=parseInt(document.getElementById('motor1_max').value);";
    html += "const motor2_max=parseInt(document.getElementById('motor2_max').value);";
    html += "fetch('/api/settings/save',{method:'POST',headers:{'Content-Type':'application/json'},";
    html += "body:JSON.stringify({type:'motor',motor1_max:motor1_max,motor2_max:motor2_max})})";
    html += ".then(response=>response.json())";
    html += ".then(data=>alert('Settings saved'))";
    html += ".catch(error=>alert('Error: '+error));}";
    
    // 保存安全设置
    html += "function saveSafetySettings(){";
    html += "const timeout=parseInt(document.getElementById('timeout').value);";
    html += "const watchdog=document.getElementById('watchdog').checked;";
    html += "fetch('/api/settings/save',{method:'POST',headers:{'Content-Type':'application/json'},";
    html += "body:JSON.stringify({type:'safety',timeout:timeout,watchdog:watchdog})})";
    html += ".then(response=>response.json())";
    html += ".then(data=>alert('Settings saved'))";
    html += ".catch(error=>alert('Error: '+error));}";
    
    html += getCommonScript();
    html += "</script></body></html>";
    
    return html;
}

// API处理函数
void MyWebServer::handleSetVW() {
    server->sendHeader("Access-Control-Allow-Origin", "*");
    
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

    // TODO: 实现电机控制逻辑

    String response = "{\"status\":\"OK\",\"v\":" + String(v) + ",\"w\":" + String(w) + "}";
    server->send(200, "application/json", response);
}

void MyWebServer::handleSetMotor() {
    server->sendHeader("Access-Control-Allow-Origin", "*");
    
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

    // TODO: 实现电机控制逻辑

    String response = "{\"status\":\"OK\",\"motor\":" + String(motorId) + 
                     ",\"speed\":" + String(speed) + "}";
    server->send(200, "application/json", response);
}

void MyWebServer::handleSetLED() {
    server->sendHeader("Access-Control-Allow-Origin", "*");
    
    if (!server->hasArg("plain")) {
        server->send(400, "application/json", "{\"error\":\"No body\"}");
        return;
    }

    String body = server->arg("plain");
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, body);
    String mode = doc["mode"];

    // TODO: 实现LED控制逻辑

    String response = "{\"status\":\"OK\",\"led_mode\":\"" + mode + "\"}";
    server->send(200, "application/json", response);
}

void MyWebServer::handleSetBuzzer() {
    server->sendHeader("Access-Control-Allow-Origin", "*");
    
    if (!server->hasArg("plain")) {
        server->send(400, "application/json", "{\"error\":\"No body\"}");
        return;
    }

    String body = server->arg("plain");
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, body);
    String mode = doc["mode"];

    // TODO: 实现蜂鸣器控制逻辑

    String response = "{\"status\":\"OK\",\"buzzer_mode\":\"" + mode + "\"}";
    server->send(200, "application/json", response);
}

void MyWebServer::handleGetStatus() {
    server->sendHeader("Access-Control-Allow-Origin", "*");
    
    DynamicJsonDocument doc(1024);
    doc["uptime"] = millis() / 1000;
    doc["free_heap"] = ESP.getFreeHeap();
    doc["cpu_freq"] = ESP.getCpuFreqMHz();
    doc["ip"] = WiFi.localIP().toString();
    doc["rssi"] = WiFi.RSSI();
    // TODO: 添加更多状态数据

    String status;
    serializeJson(doc, status);
    server->send(200, "application/json", status);
}

void MyWebServer::handleGetVersion() {
    server->sendHeader("Access-Control-Allow-Origin", "*");
    
    String version = "{\"version\":\"1.0.0\",\"build_date\":\"" + 
                    String(__DATE__) + "\",\"build_time\":\"" + 
                    String(__TIME__) + "\"}";
    server->send(200, "application/json", version);
}

void MyWebServer::handleSaveSettings() {
    server->sendHeader("Access-Control-Allow-Origin", "*");
    
    if (!server->hasArg("plain")) {
        server->send(400, "application/json", "{\"error\":\"No body\"}");
        return;
    }

    String body = server->arg("plain");
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, body);
    
    const char* type = doc["type"];
    String response = "{\"status\":\"OK\"}";

    if (String(type) == "motor") {
        // 保存电机设置
        int motor1_max = doc["motor1_max"];
        int motor2_max = doc["motor2_max"];
        // TODO: 实现保存逻辑
    }
    else if (String(type) == "safety") {
        // 保存安全设置
        int timeout = doc["timeout"];
        bool watchdog = doc["watchdog"];
        // TODO: 实现保存逻辑
    }
    
    server->send(200, "application/json", response);
}