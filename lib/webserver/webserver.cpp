#include "webserver.h"

MyWebServer::MyWebServer()
{
    server = new ESP8266WebServer(80);
}

MyWebServer::~MyWebServer()
{
    delete server;
}

void MyWebServer::init()
{
    setupRoutes();
    server->begin();
    Serial.println("HTTP server started");
    log("System started");
}

void MyWebServer::handle()
{
    server->handleClient();
}

void MyWebServer::setupRoutes()
{
    server->on("/", HTTP_GET, [this]()
               { this->handleRoot(); });
    server->on("/control", HTTP_GET, [this]()
               { this->handleControl(); });
    server->on("/sensor", HTTP_GET, [this]()
               { this->handleSensor(); });
    server->on("/mode", HTTP_GET, [this]()
               { this->handleMode(); });
    server->on("/log", HTTP_GET, [this]()
               { this->handleLog(); });
    server->on("/settings", HTTP_GET, [this]()
               { this->handleSettings(); });
    server->on("/advanced", HTTP_GET, [this]()
               { this->handleAdvanced(); });
    server->on("/debug", HTTP_GET, [this]()
               { this->handleDebug(); });

    // API routes
    server->on("/api/setVW", HTTP_POST, [this]()
               { this->handleSetVW(); });
    server->on("/api/setMotor", HTTP_POST, [this]()
               { this->handleSetMotor(); });
    server->on("/api/setLED", HTTP_POST, [this]()
               { this->handleSetLED(); });
    server->on("/api/setBuzzer", HTTP_POST, [this]()
               { this->handleSetBuzzer(); });
    server->on("/api/status", HTTP_GET, [this]()
               { this->handleGetStatus(); });
    server->on("/api/version", HTTP_GET, [this]()
               { this->handleGetVersion(); });
    server->on("/api/settings/save", HTTP_POST, [this]()
               { this->handleSaveSettings(); });
    server->on("/api/mode/save", HTTP_POST, [this]()
               { this->handleSaveMode(); });
    server->on("/api/sensor/data", HTTP_GET, [this]()
               { this->handleGetSensorData(); });
    server->on("/api/log/data", HTTP_GET, [this]()
               { this->handleGetLogData(); });
    server->on("/api/log/clear", HTTP_POST, [this]()
               { this->handleClearLog(); });

    // Handle CORS
    server->on("/api/setVW", HTTP_OPTIONS, [this]()
               {
        server->sendHeader("Access-Control-Allow-Origin", "*");
        server->sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
        server->sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
        server->send(204); });
}

// 日志相关函数
void MyWebServer::addLog(const String &message)
{
    if (logCount < MAX_LOGS)
    {
        logs[logCount++] = String(millis() / 1000) + ": " + message;
    }
    else
    {
        // 数组已满，移动所有日志
        for (int i = 0; i < MAX_LOGS - 1; i++)
        {
            logs[i] = logs[i + 1];
        }
        logs[MAX_LOGS - 1] = String(millis() / 1000) + ": " + message;
    }
}

void MyWebServer::clearLogs()
{
    logCount = 0;
}

String MyWebServer::getNavBar(const String &currentPage)
{
    String html = "<nav>";
    html += "<a href='/' " + String(currentPage == "home" ? "class='active'" : "") + ">Home</a>";
    html += "<a href='/control' " + String(currentPage == "control" ? "class='active'" : "") + ">Control</a>";
    html += "<a href='/sensor' " + String(currentPage == "sensor" ? "class='active'" : "") + ">Sensors</a>";
    html += "<a href='/mode' " + String(currentPage == "mode" ? "class='active'" : "") + ">Modes</a>";
    html += "<a href='/log' " + String(currentPage == "log" ? "class='active'" : "") + ">Logs</a>";
    html += "<a href='/settings' " + String(currentPage == "settings" ? "class='active'" : "") + ">Settings</a>";
    html += "<a href='/debug' " + String(currentPage == "debug" ? "class='active'" : "") + ">Debug</a>";
    html += "</nav>";
    return html;
}

String MyWebServer::getCommonStyle()
{
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
    style += "textarea{width:100%;padding:8px;border-radius:8px;border:1px solid #e2e8f0}";
    style += "</style>";
    return style;
}

String MyWebServer::getCommonScript()
{
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

// 主页面 Handler
void MyWebServer::handleRoot()
{
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>ESP8266 Robot</title>";
    html += getCommonStyle();
    html += "</head><body>";
    html += getNavBar("home");

    html += "<div class='container'>";
    // 欢迎卡片
    html += "<div class='card card-status'>";
    html += "<h2>Welcome to ESP8266 Robot Control</h2>";
    html += "<div class='control-item'>";
    html += "<p>IP Address: " + WiFi.localIP().toString() + "</p>";
    html += "<p>MAC Address: " + WiFi.macAddress() + "</p>";
    html += "<p>Uptime: <span id='uptime'>0</span> seconds</p>";
    html += "</div></div>";

    // 快速访问卡片
    html += "<div class='card card-motor'>";
    html += "<h2>Quick Access</h2>";
    html += "<div class='control-item'>";
    html += "<a href='/control' class='btn btn-motor'>Control Panel</a> ";
    html += "<a href='/sensor' class='btn btn-speed'>Sensor Monitor</a> ";
    html += "<a href='/mode' class='btn btn-led'>Motion Modes</a>";
    html += "</div></div>";

    html += "</div>";

    // 更新时间的JavaScript
    html += "<script>";
    html += "setInterval(()=>{";
    html += "document.getElementById('uptime').textContent=Math.floor(Date.now()/1000);";
    html += "},1000);</script>";

    html += "</body></html>";
    server->send(200, "text/html", html);
}

// API 处理函数
void MyWebServer::handleGetStatus()
{
    server->sendHeader("Access-Control-Allow-Origin", "*");

    DynamicJsonDocument doc(1024);
    doc["uptime"] = millis() / 1000;
    doc["free_heap"] = ESP.getFreeHeap();
    doc["cpu_freq"] = ESP.getCpuFreqMHz();
    doc["ip"] = WiFi.localIP().toString();
    doc["rssi"] = WiFi.RSSI();

    String status;
    serializeJson(doc, status);
    server->send(200, "application/json", status);
}

void MyWebServer::handleGetVersion()
{
    server->sendHeader("Access-Control-Allow-Origin", "*");

    String version = "{\"version\":\"1.0.0\",\"build_date\":\"" +
                     String(__DATE__) + "\",\"build_time\":\"" +
                     String(__TIME__) + "\"}";
    server->send(200, "application/json", version);
}

void MyWebServer::handleSetVW()
{
    server->sendHeader("Access-Control-Allow-Origin", "*");

    if (!server->hasArg("plain"))
    {
        server->send(400, "application/json", "{\"error\":\"No body\"}");
        return;
    }

    String body = server->arg("plain");
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, body);

    if (error)
    {
        server->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
        return;
    }

    float v = doc["v"];
    float w = doc["w"];

    // TODO: 实现电机控制逻辑
    log("Set velocity: v=" + String(v) + ", w=" + String(w));

    String response = "{\"status\":\"OK\",\"v\":" + String(v) + ",\"w\":" + String(w) + "}";
    server->send(200, "application/json", response);
}

void MyWebServer::handleSetMotor()
{
    server->sendHeader("Access-Control-Allow-Origin", "*");

    if (!server->hasArg("plain"))
    {
        server->send(400, "application/json", "{\"error\":\"No body\"}");
        return;
    }

    String body = server->arg("plain");
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, body);

    if (error)
    {
        server->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
        return;
    }

    int motorId = doc["motor"];
    int speed = doc["speed"];

    // TODO: 实现电机控制逻辑
    log("Set motor " + String(motorId) + " speed: " + String(speed));

    String response = "{\"status\":\"OK\",\"motor\":" + String(motorId) +
                      ",\"speed\":" + String(speed) + "}";
    server->send(200, "application/json", response);
}

void MyWebServer::handleSetLED()
{
    server->sendHeader("Access-Control-Allow-Origin", "*");

    if (!server->hasArg("plain"))
    {
        server->send(400, "application/json", "{\"error\":\"No body\"}");
        return;
    }

    String body = server->arg("plain");
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, body);
    String mode = doc["mode"];

    // TODO: 实现LED控制逻辑
    log("Set LED mode: " + mode);

    String response = "{\"status\":\"OK\",\"led_mode\":\"" + mode + "\"}";
    server->send(200, "application/json", response);
}

void MyWebServer::handleSetBuzzer()
{
    server->sendHeader("Access-Control-Allow-Origin", "*");

    if (!server->hasArg("plain"))
    {
        server->send(400, "application/json", "{\"error\":\"No body\"}");
        return;
    }

    String body = server->arg("plain");
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, body);
    String mode = doc["mode"];

    // TODO: 实现蜂鸣器控制逻辑
    log("Set buzzer mode: " + mode);

    String response = "{\"status\":\"OK\",\"buzzer_mode\":\"" + mode + "\"}";
    server->send(200, "application/json", response);
}

// 控制页面处理函数
void MyWebServer::handleControl()
{
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>Robot Control</title>";
    html += getCommonStyle();
    html += "</head><body>";
    html += getNavBar("control");

    html += "<div class='container'>";

    // Status Card
    html += "<div class='card card-status'>";
    html += "<h2>System Status</h2>";
    html += "<button class='btn btn-status' onclick='getStatus()'>Get Status</button>";
    html += "<button class='btn btn-status' onclick='getVersion()'>Get Version</button>";
    html += "<pre id='status' class='status'>Status will appear here...</pre>";
    html += "</div>";

    // Speed Control Card
    html += "<div class='card card-speed'>";
    html += "<h2>Speed Control (V,W)</h2>";
    html += "<div class='control-item'>";
    html += "Linear (m/s): <input type='number' id='v_speed' step='0.1' value='0'>";
    html += "Angular (rad/s): <input type='number' id='w_speed' step='0.1' value='0'>";
    html += "<button class='btn btn-speed' onclick='setVW()'>Set Speed</button>";
    html += "</div></div>";

    // Motor Control Card
    html += "<div class='card card-motor'>";
    html += "<h2>Direct Motor Control</h2>";
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
    html += "<h2>LED Control</h2>";
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
    html += "<h2>Buzzer Control</h2>";
    html += "<div class='control-item'>";
    html += "Mode: <select id='buzzer_mode'>";
    html += "<option value='off'>OFF</option>";
    html += "<option value='beep'>BEEP</option>";
    html += "<option value='alarm'>ALARM</option>";
    html += "</select>";
    html += "<button class='btn btn-buzzer' onclick='setBuzzer()'>Set Buzzer</button>";
    html += "</div></div>";

    html += "</div>";

    // JavaScript functions
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
    html += "</script></body></html>";

    server->send(200, "text/html", html);
}

// 设置页面处理函数
void MyWebServer::handleSettings()
{
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>System Settings</title>";
    html += getCommonStyle();
    html += "</head><body>";
    html += getNavBar("settings");

    html += "<div class='container'>";

    // System Info Card
    html += "<div class='card card-status'>";
    html += "<h2>System Information</h2>";
    html += "<div class='control-item'>";
    html += "<p>Device Name: ESP8266_Robot</p>";
    html += "<p>Version: 1.0.0</p>";
    html += "<p>Build Date: " + String(__DATE__) + "</p>";
    html += "<p>Build Time: " + String(__TIME__) + "</p>";
    html += "<p>Free Heap: " + String(ESP.getFreeHeap()) + " bytes</p>";
    html += "</div></div>";

    // WiFi Settings Card
    html += "<div class='card card-motor'>";
    html += "<h2>WiFi Settings</h2>";
    html += "<div class='control-item'>";
    html += "SSID: <input type='text' id='wifi_ssid' value='" + WiFi.SSID() + "'><br>";
    html += "Password: <input type='password' id='wifi_password'><br>";
    html += "<button class='btn btn-motor' onclick='saveWiFiSettings()'>Save WiFi Settings</button>";
    html += "</div></div>";

    // Motor Settings Card
    html += "<div class='card card-speed'>";
    html += "<h2>Motor Settings</h2>";
    html += "<div class='control-item'>";
    html += "Max Speed: <input type='number' id='max_speed' value='255'><br>";
    html += "Dead Zone: <input type='number' id='dead_zone' value='10'><br>";
    html += "<button class='btn btn-speed' onclick='saveMotorSettings()'>Save Motor Settings</button>";
    html += "</div></div>";

    // System Functions Card
    html += "<div class='card card-led'>";
    html += "<h2>System Functions</h2>";
    html += "<div class='control-item'>";
    html += "<button class='btn btn-led' onclick='if(confirm(\"Reset settings?\"))resetSettings()'>Reset Settings</button> ";
    html += "<button class='btn btn-led' onclick='if(confirm(\"Reboot device?\"))rebootDevice()'>Reboot Device</button>";
    html += "</div></div>";

    html += "</div>";

    // JavaScript
    html += "<script>";
    html += "function saveWiFiSettings(){";
    html += "const settings={";
    html += "ssid:document.getElementById('wifi_ssid').value,";
    html += "password:document.getElementById('wifi_password').value";
    html += "};";
    html += "fetch('/api/settings/save',{method:'POST',headers:{'Content-Type':'application/json'},";
    html += "body:JSON.stringify({type:'wifi',...settings})})";
    html += ".then(response=>response.json())";
    html += ".then(data=>alert('WiFi settings saved'))";
    html += ".catch(error=>alert('Error: '+error));}";

    html += "function saveMotorSettings(){";
    html += "const settings={";
    html += "max_speed:parseInt(document.getElementById('max_speed').value),";
    html += "dead_zone:parseInt(document.getElementById('dead_zone').value)";
    html += "};";
    html += "fetch('/api/settings/save',{method:'POST',headers:{'Content-Type':'application/json'},";
    html += "body:JSON.stringify({type:'motor',...settings})})";
    html += ".then(response=>response.json())";
    html += ".then(data=>alert('Motor settings saved'))";
    html += ".catch(error=>alert('Error: '+error));}";

    html += "function resetSettings(){";
    html += "fetch('/api/settings/save',{method:'POST',headers:{'Content-Type':'application/json'},";
    html += "body:JSON.stringify({type:'reset'})})";
    html += ".then(response=>response.json())";
    html += ".then(data=>alert('Settings reset'))";
    html += ".catch(error=>alert('Error: '+error));}";

    html += "function rebootDevice(){";
    html += "fetch('/api/settings/save',{method:'POST',headers:{'Content-Type':'application/json'},";
    html += "body:JSON.stringify({type:'reboot'})})";
    html += ".then(response=>response.json())";
    html += ".then(data=>alert('Device rebooting...'))";
    html += ".catch(error=>alert('Error: '+error));}";

    html += getCommonScript();
    html += "</script></body></html>";

    server->send(200, "text/html", html);
}

// 调试页面处理函数
void MyWebServer::handleDebug()
{
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>Debug Console</title>";
    html += getCommonStyle();
    html += "</head><body>";
    html += getNavBar("debug");

    html += "<div class='container'>";

    // Debug Console Card
    html += "<div class='card card-status'>";
    html += "<h2>Debug Console</h2>";
    html += "<div class='control-item'>";
    html += "<pre id='debug-log' style='height:200px;overflow-y:auto;background:#1a1a1a;color:#fff;padding:10px;'></pre>";
    html += "<input type='text' id='debug-cmd' style='width:80%' placeholder='Enter command...'>";
    html += "<button class='btn btn-status' onclick='sendCommand()'>Send</button>";
    html += "</div></div>";

    // System Tests Card
    html += "<div class='card card-motor'>";
    html += "<h2>System Tests</h2>";
    html += "<div class='control-item'>";
    html += "<button class='btn btn-motor' onclick='runTest(\"motors\")'>Test Motors</button> ";
    html += "<button class='btn btn-motor' onclick='runTest(\"sensors\")'>Test Sensors</button> ";
    html += "<button class='btn btn-motor' onclick='runTest(\"wifi\")'>Test WiFi</button>";
    html += "</div></div>";

    html += "</div>";

    // JavaScript
    html += "<script>";
    html += "let debugLog=document.getElementById('debug-log');";
    html += "function addLog(text){";
    html += "debugLog.innerHTML+=text+'\\n';";
    html += "debugLog.scrollTop=debugLog.scrollHeight;}";

    html += "function sendCommand(){";
    html += "const cmd=document.getElementById('debug-cmd').value;";
    html += "fetch('/api/system/command',{method:'POST',headers:{'Content-Type':'application/json'},";
    html += "body:JSON.stringify({command:cmd})})";
    html += ".then(response=>response.json())";
    html += ".then(data=>{addLog('> '+cmd);addLog(JSON.stringify(data));})";
    html += ".catch(error=>addLog('Error: '+error))";
    html += "document.getElementById('debug-cmd').value='';}";

    html += "function runTest(type){";
    html += "fetch('/api/system/command',{method:'POST',headers:{'Content-Type':'application/json'},";
    html += "body:JSON.stringify({command:'test_'+type})})";
    html += ".then(response=>response.json())";
    html += ".then(data=>addLog(JSON.stringify(data)))";
    html += ".catch(error=>addLog('Error: '+error));}";

    html += "document.getElementById('debug-cmd').addEventListener('keyup',function(event){";
    html += "if(event.key==='Enter')sendCommand();});";

    html += getCommonScript();
    html += "</script></body></html>";

    server->send(200, "text/html", html);
}

// 日志页面处理函数
void MyWebServer::handleLog()
{
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>System Logs</title>";
    html += getCommonStyle();
    html += "</head><body>";
    html += getNavBar("log");

    html += "<div class='container'>";

    // Log Display Card
    html += "<div class='card card-status'>";
    html += "<h2>System Logs</h2>";
    html += "<div class='control-item'>";
    html += "<pre id='log-content' style='height:300px;overflow-y:auto;background:#fff;padding:10px;'></pre>";
    html += "<button class='btn btn-status' onclick='refreshLogs()'>Refresh</button> ";
    html += "<button class='btn btn-status' onclick='clearLogs()'>Clear Logs</button>";
    html += "</div></div>";

    html += "</div>";

    // JavaScript
    html += "function refreshLogs(){";
    html += "fetch('/api/log/data')";
    html += ".then(response=>response.json())";
    html += ".then(data=>{";
    html += "const logContent=document.getElementById('log-content');";
    html += "logContent.innerHTML=data.join('\\n');";
    html += "logContent.scrollTop=logContent.scrollHeight;";
    html += "}).catch(error=>alert('Error: '+error));}";

    html += "function clearLogs(){";
    html += "if(confirm('Clear all logs?')){";
    html += "fetch('/api/log/clear',{method:'POST'})";
    html += ".then(response=>response.json())";
    html += ".then(()=>{";
    html += "document.getElementById('log-content').innerHTML='';";
    html += "alert('Logs cleared');";
    html += "}).catch(error=>alert('Error: '+error));}}";

    html += "refreshLogs();";                 // 初始加载日志
    html += "setInterval(refreshLogs,5000);"; // 每5秒自动刷新

    html += getCommonScript();
    html += "</script></body></html>";

    server->send(200, "text/html", html);
}

// 传感器监控页面处理函数
void MyWebServer::handleSensor()
{
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>Sensor Monitor</title>";
    html += getCommonStyle();
    html += "</head><body>";
    html += getNavBar("sensor");

    html += "<div class='container'>";

    // Sensor Data Card
    html += "<div class='card card-status'>";
    html += "<h2>Sensor Data</h2>";
    html += "<div class='control-item'>";
    html += "<div id='sensor-data'>Loading...</div>";
    html += "<button class='btn btn-status' onclick='toggleUpdate()'>Start/Stop Update</button>";
    html += "</div></div>";

    // Motor Status Card
    html += "<div class='card card-motor'>";
    html += "<h2>Motor Status</h2>";
    html += "<div class='control-item'>";
    html += "<div id='motor-status'>Loading...</div>";
    html += "</div></div>";

    // Battery Status Card
    html += "<div class='card card-speed'>";
    html += "<h2>Battery Status</h2>";
    html += "<div class='control-item'>";
    html += "<div id='battery-status'>Loading...</div>";
    html += "</div></div>";

    html += "</div>";

    // JavaScript
    html += "<script>";
    html += "let updateInterval=null;";

    html += "function updateSensorData(){";
    html += "fetch('/api/sensor/data')";
    html += ".then(response=>response.json())";
    html += ".then(data=>{";
    html += "document.getElementById('sensor-data').innerHTML=";
    html += "'Temperature: '+data.temperature+'°C<br>'+";
    html += "'Humidity: '+data.humidity+'%<br>'+";
    html += "'IMU: '+JSON.stringify(data.imu)+'<br>';";
    html += "document.getElementById('motor-status').innerHTML=";
    html += "'Motor1 Speed: '+data.motors.m1+'<br>'+";
    html += "'Motor2 Speed: '+data.motors.m2+'<br>';";
    html += "document.getElementById('battery-status').innerHTML=";
    html += "'Voltage: '+data.battery.voltage+'V<br>'+";
    html += "'Current: '+data.battery.current+'mA<br>';";
    html += "}).catch(error=>console.error('Error:',error));}";

    html += "function toggleUpdate(){";
    html += "if(updateInterval){";
    html += "clearInterval(updateInterval);";
    html += "updateInterval=null;";
    html += "}else{";
    html += "updateInterval=setInterval(updateSensorData,1000);}}";

    html += "updateSensorData();"; // 初始更新
    html += "toggleUpdate();";     // 开始自动更新

    html += getCommonScript();
    html += "</script></body></html>";

    server->send(200, "text/html", html);
}

// 日志相关API处理函数
void MyWebServer::handleGetLogData()
{
    DynamicJsonDocument doc(4096);
    JsonArray array = doc.to<JsonArray>();

    for (int i = 0; i < logCount; i++)
    {
        array.add(logs[i]);
    }

    String response;
    serializeJson(doc, response);
    server->send(200, "application/json", response);
}

void MyWebServer::handleClearLog()
{
    clearLogs();
    log("Logs cleared by user");
    server->send(200, "application/json", "{\"status\":\"OK\"}");
}

// 传感器数据API处理函数
void MyWebServer::handleGetSensorData()
{
    DynamicJsonDocument doc(1024);

    // TODO: 替换为实际的传感器读数
    doc["temperature"] = 25.5;
    doc["humidity"] = 65;
    doc["imu"]["ax"] = 0;
    doc["imu"]["ay"] = 0;
    doc["imu"]["az"] = 9.8;
    doc["imu"]["gx"] = 0;
    doc["imu"]["gy"] = 0;
    doc["imu"]["gz"] = 0;
    doc["motors"]["m1"] = 0;
    doc["motors"]["m2"] = 0;
    doc["battery"]["voltage"] = 12.6;
    doc["battery"]["current"] = 100;

    String response;
    serializeJson(doc, response);
    server->send(200, "application/json", response);
}

// 设置保存处理函数
void MyWebServer::handleSaveSettings()
{
    if (!server->hasArg("plain"))
    {
        server->send(400, "application/json", "{\"error\":\"No body\"}");
        return;
    }

    String body = server->arg("plain");
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, body);

    if (error)
    {
        server->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
        return;
    }

    const char *type = doc["type"];
    String response = "{\"status\":\"OK\"}";

    if (String(type) == "wifi")
    {
        // 保存WiFi设置
        const char *ssid = doc["ssid"];
        const char *password = doc["password"];
        // TODO: 实现WiFi设置保存
        log("WiFi settings updated: SSID=" + String(ssid));
    }
    else if (String(type) == "motor")
    {
        // 保存电机设置
        int maxSpeed = doc["max_speed"];
        int deadZone = doc["dead_zone"];
        // TODO: 实现电机设置保存
        log("Motor settings updated: maxSpeed=" + String(maxSpeed) + ", deadZone=" + String(deadZone));
    }
    else if (String(type) == "reset")
    {
        // 重置设置
        // TODO: 实现设置重置
        log("Settings reset to default");
    }
    else if (String(type) == "reboot")
    {
        // 重启设备
        log("System reboot requested");
        server->send(200, "application/json", response);
        delay(1000);
        ESP.restart();
        return;
    }

    server->send(200, "application/json", response);
}

// 系统命令处理函数
void MyWebServer::handleSystemCommand()
{
    if (!server->hasArg("plain"))
    {
        server->send(400, "application/json", "{\"error\":\"No body\"}");
        return;
    }

    String body = server->arg("plain");
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, body);

    const char *command = doc["command"];
    String response;

    if (String(command).startsWith("test_"))
    {
        // 处理测试命令
        if (String(command) == "test_motors")
        {
            // TODO: 实现电机测试
            response = "{\"status\":\"OK\",\"message\":\"Motor test completed\"}";
            log("Motor test executed");
        }
        else if (String(command) == "test_sensors")
        {
            // TODO: 实现传感器测试
            response = "{\"status\":\"OK\",\"message\":\"Sensor test completed\"}";
            log("Sensor test executed");
        }
        else if (String(command) == "test_wifi")
        {
            // TODO: 实现WiFi测试
            response = "{\"status\":\"OK\",\"message\":\"WiFi test completed\"}";
            log("WiFi test executed");
        }
    }
    else
    {
        // 处理其他命令
        log("System command executed: " + String(command));
        response = "{\"status\":\"OK\",\"message\":\"Command executed\"}";
    }

    server->send(200, "application/json", response);
}

// 添加缺失的运动模式处理函数
void MyWebServer::handleMode() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>Motion Modes</title>";
    html += getCommonStyle();
    html += "</head><body>";
    html += getNavBar("mode");
    
    html += "<div class='container'>";
    
    // Mode Selection Card
    html += "<div class='card card-status'>";
    html += "<h2>Motion Mode</h2>";
    html += "<div class='control-item'>";
    html += "Current Mode: <select id='motion_mode'>";
    html += "<option value='manual'>Manual Control</option>";
    html += "<option value='auto'>Autonomous</option>";
    html += "<option value='follow'>Line Following</option>";
    html += "</select><br>";
    html += "<button class='btn btn-status' onclick='setMode()'>Apply Mode</button>";
    html += "</div></div>";

    // Mode Settings Card
    html += "<div class='card card-motor'>";
    html += "<h2>Mode Parameters</h2>";
    html += "<div class='control-item'>";
    html += "Max Speed: <input type='number' id='max_speed' value='255'><br>";
    html += "Min Speed: <input type='number' id='min_speed' value='50'><br>";
    html += "<button class='btn btn-motor' onclick='saveParams()'>Save Parameters</button>";
    html += "</div></div>";
    
    html += "</div>";
    
    // JavaScript
    html += "<script>";
    html += "function setMode(){";
    html += "const mode=document.getElementById('motion_mode').value;";
    html += "fetch('/api/mode/save',{method:'POST',headers:{'Content-Type':'application/json'},";
    html += "body:JSON.stringify({command:'set_mode',mode:mode})})";
    html += ".then(response=>response.json())";
    html += ".then(data=>alert('Mode set to: '+mode))";
    html += ".catch(error=>alert('Error: '+error));}";
    
    html += "function saveParams(){";
    html += "const maxSpeed=parseInt(document.getElementById('max_speed').value);";
    html += "const minSpeed=parseInt(document.getElementById('min_speed').value);";
    html += "fetch('/api/mode/save',{method:'POST',headers:{'Content-Type':'application/json'},";
    html += "body:JSON.stringify({command:'save_params',maxSpeed:maxSpeed,minSpeed:minSpeed})})";
    html += ".then(response=>response.json())";
    html += ".then(data=>alert('Parameters saved'))";
    html += ".catch(error=>alert('Error: '+error));}";
    
    html += getCommonScript();
    html += "</script></body></html>";
    
    server->send(200, "text/html", html);
}

// 添加高级设置处理函数
void MyWebServer::handleAdvanced() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>Advanced Settings</title>";
    html += getCommonStyle();
    html += "</head><body>";
    html += getNavBar("advanced");
    
    html += "<div class='container'>";
    
    // Hardware Settings Card
    html += "<div class='card card-status'>";
    html += "<h2>Hardware Settings</h2>";
    html += "<div class='control-item'>";
    html += "CPU Frequency: <select id='cpu_freq'>";
    html += "<option value='80'>80 MHz</option>";
    html += "<option value='160'>160 MHz</option>";
    html += "</select><br>";
    html += "<button class='btn btn-status' onclick='saveHardware()'>Save Hardware Settings</button>";
    html += "</div></div>";

    // Network Settings Card
    html += "<div class='card card-motor'>";
    html += "<h2>Network Settings</h2>";
    html += "<div class='control-item'>";
    html += "Static IP: <input type='text' id='static_ip'><br>";
    html += "Gateway: <input type='text' id='gateway'><br>";
    html += "Subnet: <input type='text' id='subnet'><br>";
    html += "<button class='btn btn-motor' onclick='saveNetwork()'>Save Network Settings</button>";
    html += "</div></div>";
    
    html += "</div>";
    
    // JavaScript
    html += "<script>";
    html += "function saveHardware(){";
    html += "const cpuFreq=document.getElementById('cpu_freq').value;";
    html += "fetch('/api/advanced/save',{method:'POST',headers:{'Content-Type':'application/json'},";
    html += "body:JSON.stringify({type:'hardware',cpu_freq:cpuFreq})})";
    html += ".then(response=>response.json())";
    html += ".then(data=>alert('Hardware settings saved'))";
    html += ".catch(error=>alert('Error: '+error));}";
    
    html += "function saveNetwork(){";
    html += "const settings={";
    html += "static_ip:document.getElementById('static_ip').value,";
    html += "gateway:document.getElementById('gateway').value,";
    html += "subnet:document.getElementById('subnet').value";
    html += "};";
    html += "fetch('/api/advanced/save',{method:'POST',headers:{'Content-Type':'application/json'},";
    html += "body:JSON.stringify({type:'network',...settings})})";
    html += ".then(response=>response.json())";
    html += ".then(data=>alert('Network settings saved'))";
    html += ".catch(error=>alert('Error: '+error));}";
    
    html += getCommonScript();
    html += "</script></body></html>";
    
    server->send(200, "text/html", html);
}

// 添加模式保存处理函数
void MyWebServer::handleSaveMode() {
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

    const char* command = doc["command"];
    String response = "{\"status\":\"OK\"}";

    if (String(command) == "set_mode") {
        const char* mode = doc["mode"];
        // TODO: 实现模式切换逻辑
        log("Motion mode changed to: " + String(mode));
    }
    else if (String(command) == "save_params") {
        int maxSpeed = doc["maxSpeed"];
        int minSpeed = doc["minSpeed"];
        // TODO: 实现参数保存逻辑
        log("Motion parameters updated: maxSpeed=" + String(maxSpeed) + ", minSpeed=" + String(minSpeed));
    }
    
    server->send(200, "application/json", response);
}