#include "page_control.h"

void ControlPage::handleControl(ESP8266WebServer* server) {
    String html = HtmlCommon::getHeader("Robot Control");
    html += HtmlCommon::getNavBar("control");
    
    html += "<div class='container'>";
    html += getStatusCard();
    html += getSpeedControlCard();
    html += getMotorControlCard();
    html += getLEDControlCard();
    html += getBuzzerControlCard();
    html += "</div>";
    
    html += getControlScript();
    html += HtmlCommon::getCommonScript();
    html += HtmlCommon::getFooter();
    
    server->send(200, "text/html", html);
}

String ControlPage::getStatusCard() {
    String html = "<div class='card card-status'>";
    html += "<h2>System Status</h2>";
    html += "<button class='btn btn-status' onclick='getStatus()'>Get Status</button>";
    html += "<button class='btn btn-status' onclick='getVersion()'>Get Version</button>";
    html += "<pre id='status' class='status'>Status will appear here...</pre>";
    html += "</div>";
    return html;
}

String ControlPage::getSpeedControlCard() {
    String html = "<div class='card card-speed'>";
    html += "<h2>Speed Control (V,W)</h2>";
    html += "<div class='control-item'>";
    html += "Linear (m/s): <input type='number' id='v_speed' step='0.1' value='0'>";
    html += "Angular (rad/s): <input type='number' id='w_speed' step='0.1' value='0'>";
    html += "<button class='btn btn-speed' onclick='setVW()'>Set Speed</button>";
    html += "<div id='speed-indicator' style='margin-top:10px'></div>";
    html += "</div></div>";
    return html;
}

String ControlPage::getMotorControlCard() {
    String html = "<div class='card card-motor'>";
    html += "<h2>Direct Motor Control</h2>";
    html += "<div class='control-item'>";
    html += "Motor: <select id='motor_select'>";
    html += "<option value='1'>Motor 1</option>";
    html += "<option value='2'>Motor 2</option>";
    html += "</select>";
    html += "Speed: <input type='number' id='motor_speed' min='-255' max='255' value='0'>";
    html += "<button class='btn btn-motor' onclick='setMotor()'>Set Motor</button>";
    html += "<div class='motor-status' style='margin-top:10px'>";
    html += "<div>Motor 1: <span id='motor1-status'>Stopped</span></div>";
    html += "<div>Motor 2: <span id='motor2-status'>Stopped</span></div>";
    html += "</div>";
    html += "</div></div>";
    return html;
}

String ControlPage::getLEDControlCard() {
    String html = "<div class='card card-led'>";
    html += "<h2>LED Control</h2>";
    html += "<div class='control-item'>";
    html += "Mode: <select id='led_mode'>";
    html += "<option value='off'>OFF</option>";
    html += "<option value='on'>ON</option>";
    html += "<option value='blink'>BLINK</option>";
    html += "</select>";
    html += "<button class='btn btn-led' onclick='setLED()'>Set LED</button>";
    html += "<div id='led-indicator' class='led-off' style='margin-top:10px'></div>";
    html += "</div></div>";
    return html;
}

String ControlPage::getBuzzerControlCard() {
    String html = "<div class='card card-buzzer'>";
    html += "<h2>Buzzer Control</h2>";
    html += "<div class='control-item'>";
    html += "Mode: <select id='buzzer_mode'>";
    html += "<option value='off'>OFF</option>";
    html += "<option value='beep'>BEEP</option>";
    html += "<option value='alarm'>ALARM</option>";
    html += "</select>";
    html += "<button class='btn btn-buzzer' onclick='setBuzzer()'>Set Buzzer</button>";
    html += "</div></div>";
    return html;
}

String ControlPage::getControlScript() {
    String script = "<script>";
    script += "let speedUpdateInterval = null;";
    script += "let lastVW = {v:0, w:0};";
    
    // Speed Control
    script += "function setVW(){";
    script += "const v=parseFloat(document.getElementById('v_speed').value);";
    script += "const w=parseFloat(document.getElementById('w_speed').value);";
    script += "lastVW={v:v,w:w};";
    script += "fetch('/api/setVW',{method:'POST',headers:{'Content-Type':'application/json'},";
    script += "body:JSON.stringify({v:v,w:w})}).then(response=>response.json())";
    script += ".then(data=>{";
    script += "updateStatus(data);";
    script += "updateSpeedIndicator(v,w);";
    script += "}).catch(error=>updateStatus('Error: '+error));}";
    
    // Motor Control
    script += "function setMotor(){";
    script += "const motor=parseInt(document.getElementById('motor_select').value);";
    script += "const speed=parseInt(document.getElementById('motor_speed').value);";
    script += "fetch('/api/setMotor',{method:'POST',headers:{'Content-Type':'application/json'},";
    script += "body:JSON.stringify({motor:motor,speed:speed})}).then(response=>response.json())";
    script += ".then(data=>{";
    script += "updateStatus(data);";
    script += "updateMotorStatus(motor,speed);";
    script += "}).catch(error=>updateStatus('Error: '+error));}";
    
    // LED Control
    script += "function setLED(){";
    script += "const mode=document.getElementById('led_mode').value;";
    script += "fetch('/api/setLED',{method:'POST',headers:{'Content-Type':'application/json'},";
    script += "body:JSON.stringify({mode:mode})}).then(response=>response.json())";
    script += ".then(data=>{";
    script += "updateStatus(data);";
    script += "updateLEDIndicator(mode);";
    script += "}).catch(error=>updateStatus('Error: '+error));}";
    
    // Buzzer Control
    script += "function setBuzzer(){";
    script += "const mode=document.getElementById('buzzer_mode').value;";
    script += "fetch('/api/setBuzzer',{method:'POST',headers:{'Content-Type':'application/json'},";
    script += "body:JSON.stringify({mode:mode})}).then(response=>response.json())";
    script += ".then(data=>updateStatus(data)).catch(error=>updateStatus('Error: '+error));}";
    
    // Status Indicators
    script += "function updateSpeedIndicator(v,w){";
    script += "const indicator=document.getElementById('speed-indicator');";
    script += "indicator.innerHTML='Current Speed: '+v.toFixed(2)+' m/s, '+w.toFixed(2)+' rad/s';}";
    
    script += "function updateMotorStatus(motor,speed){";
    script += "const status=document.getElementById('motor'+motor+'-status');";
    script += "status.textContent=speed==0?'Stopped':(speed>0?'Forward '+speed:'Reverse '+(-speed));}";
    
    script += "function updateLEDIndicator(mode){";
    script += "const indicator=document.getElementById('led-indicator');";
    script += "indicator.className='led-'+mode;}";
    
    script += "</script>";
    
    // 添加LED指示器样式
    script += "<style>";
    script += ".led-off{width:20px;height:20px;border-radius:50%;background:#ccc;margin:10px auto;}";
    script += ".led-on{width:20px;height:20px;border-radius:50%;background:#22c55e;box-shadow:0 0 10px #22c55e;margin:10px auto;}";
    script += ".led-blink{width:20px;height:20px;border-radius:50%;background:#22c55e;box-shadow:0 0 10px #22c55e;margin:10px auto;animation:blink 1s infinite;}";
    script += "@keyframes blink{0%{opacity:1}50%{opacity:0.3}100%{opacity:1}}";
    script += "</style>";
    
    return script;
}