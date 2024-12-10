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
    server->on("/api/setVW", HTTP_POST, [this]() { this->handleSetVW(); });
    server->on("/api/setMotor", HTTP_POST, [this]() { this->handleSetMotor(); });
    server->on("/api/setLED", HTTP_POST, [this]() { this->handleSetLED(); });
    server->on("/api/setBuzzer", HTTP_POST, [this]() { this->handleSetBuzzer(); });
    server->on("/api/status", HTTP_GET, [this]() { this->handleGetStatus(); });
    server->on("/api/version", HTTP_GET, [this]() { this->handleGetVersion(); });

    // Handle CORS
    server->on("/api/setVW", HTTP_OPTIONS, [this]() {
        server->sendHeader("Access-Control-Allow-Origin", "*");
        server->sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
        server->sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
        server->send(204);
    });
}

void MyWebServer::handleRoot() {
    server->send(200, "text/html", getHTML());
}

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

    // TODO: Implement motor control logic here

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

    // TODO: Implement motor control logic here

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
    DeserializationError error = deserializeJson(doc, body);

    if (error) {
        server->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
        return;
    }

    String mode = doc["mode"];

    // TODO: Implement LED control logic here

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
    DeserializationError error = deserializeJson(doc, body);

    if (error) {
        server->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
        return;
    }

    String mode = doc["mode"];

    // TODO: Implement buzzer control logic here

    String response = "{\"status\":\"OK\",\"buzzer_mode\":\"" + mode + "\"}";
    server->send(200, "application/json", response);
}

void MyWebServer::handleGetStatus() {
    server->sendHeader("Access-Control-Allow-Origin", "*");
    
    // TODO: Get actual status values
    String status = "{\"status\":\"running\",\"uptime\":\"" + String(millis()/1000) + "\"}";
    server->send(200, "application/json", status);
}

void MyWebServer::handleGetVersion() {
    server->sendHeader("Access-Control-Allow-Origin", "*");
    
    String version = "{\"version\":\"1.0.0\",\"build_date\":\"" + 
                    String(__DATE__) + "\",\"build_time\":\"" + 
                    String(__TIME__) + "\"}";
    server->send(200, "application/json", version);
}

String MyWebServer::getHTML() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>ESP8266 Robot Control</title>";

    // CSS styles
    html += "<style>";
    html += "body{font-family:system-ui,-apple-system,Arial;margin:0;padding:20px;background:#e0e7ff;color:#1f2937}";
    html += "h2{margin:0 0 16px 0;font-size:1.5rem;font-weight:600}";
    html += ".card{border-radius:12px;padding:24px;margin:15px 0;transition:all 0.3s;box-shadow:0 2px 4px rgba(0,0,0,0.04)}";
    html += ".card-status{background:linear-gradient(145deg,#fff,#f1f5f9)}";
    html += ".card-speed{background:linear-gradient(145deg,#fff,#f0fdf4)}";
    html += ".card-motor{background:linear-gradient(145deg,#fff,#f0f9ff)}";
    html += ".card-led{background:linear-gradient(145deg,#fff,#fdf4ff)}";
    html += ".card-buzzer{background:linear-gradient(145deg,#fff,#fff7ed)}";
    html += "input{width:80px;padding:8px 12px;margin:5px;border:1px solid #e2e8f0;border-radius:8px;background:white}";
    html += "input:focus{outline:none;border-color:#93c5fd;box-shadow:0 0 0 3px rgba(147,197,253,0.2)}";
    html += "select{padding:8px 12px;margin:5px;border:1px solid #e2e8f0;border-radius:8px;background:white}";
    html += ".btn{color:white;border:none;padding:8px 16px;border-radius:8px;cursor:pointer;margin:5px;font-weight:500;transition:all 0.2s}";
    html += ".btn:hover{transform:translateY(-1px);filter:brightness(110%)}";
    html += ".btn-status{background:#3b82f6}";
    html += ".btn-speed{background:#10b981}";
    html += ".btn-motor{background:#6366f1}";
    html += ".btn-led{background:#8b5cf6}";
    html += ".btn-buzzer{background:#f59e0b}";
    html += ".control-item{margin:10px 0;padding:16px;border:1px solid rgba(255,255,255,0.3);border-radius:8px;background:rgba(255,255,255,0.3)}";
    html += ".status{background:rgba(255,255,255,0.3);padding:16px;margin:10px 0;border-radius:8px;font-family:monospace}";
    html += "</style>";

    // Body content
    html += "</head><body>";

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

    // JavaScript
    html += "<script>";
    
    // Status update helper
    html += "function updateStatus(text){document.getElementById('status').innerText=JSON.stringify(text,null,2)}";
    
    // API calls
    html += "function getStatus(){fetch('/api/status')";
    html += ".then(response=>response.json())";
    html += ".then(data=>updateStatus(data))";
    html += ".catch(error=>updateStatus('Error: '+error));}";

    html += "function getVersion(){fetch('/api/version')";
    html += ".then(response=>response.json())";
    html += ".then(data=>updateStatus(data))";
    html += ".catch(error=>updateStatus('Error: '+error));}";
    
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
    
    html += "getStatus();"; // Get initial status on page load
    html += "</script></body></html>";

    return html;
}