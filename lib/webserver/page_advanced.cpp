#include "page_advanced.h"
#include <ESP8266WiFi.h>

void AdvancedPage::handleAdvanced(ESP8266WebServer* server) {
    String html = HtmlCommon::getHeader("Advanced Settings");
    html += HtmlCommon::getNavBar("advanced");
    
    html += "<div class='container'>";
    html += getSystemInfoCard();
    html += getNetworkConfigCard();
    html += getFirmwareUpdateCard();
    html += getCalibrationCard();
    html += getCustomCommandCard();
    html += getDataVisualizationCard();
    html += "</div>";
    
    html += getAdvancedScript();
    html += HtmlCommon::getFooter();
    
    server->send(200, "text/html", html);
}

String AdvancedPage::getSystemInfoCard() {
    String html = "<div class='card card-status'>";
    html += "<h2>System Information</h2>";
    html += "<div class='control-item'>";
    html += "<div id='system-info'>";
    html += "<p>Chip ID: " + String(ESP.getChipId(), HEX) + "</p>";
    html += "<p>Flash Size: " + String(ESP.getFlashChipSize() / 1024.0 / 1024.0, 2) + " MB</p>";
    html += "<p>Free Heap: <span id='free-heap'>" + String(ESP.getFreeHeap()) + "</span> bytes</p>";
    html += "<p>CPU Frequency: " + String(ESP.getCpuFreqMHz()) + " MHz</p>";
    html += "<p>SDK Version: " + String(ESP.getSdkVersion()) + "</p>";
    html += "<p>Reset Reason: " + ESP.getResetReason() + "</p>";
    html += "</div>";
    html += "<button class='btn btn-status' onclick='refreshSystemInfo()'>Refresh</button>";
    html += "</div></div>";
    return html;
}

String AdvancedPage::getNetworkConfigCard() {
    String html = "<div class='card card-motor'>";
    html += "<h2>Network Configuration</h2>";
    html += "<div class='control-item'>";
    html += "<div id='network-info'>";
    html += "<p>MAC Address: " + WiFi.macAddress() + "</p>";
    html += "<p>IP Address: " + WiFi.localIP().toString() + "</p>";
    html += "<p>Gateway: " + WiFi.gatewayIP().toString() + "</p>";
    html += "<p>Subnet Mask: " + WiFi.subnetMask().toString() + "</p>";
    html += "<p>DNS: " + WiFi.dnsIP().toString() + "</p>";
    html += "<p>RSSI: <span id='rssi'>" + String(WiFi.RSSI()) + "</span> dBm</p>";
    html += "</div>";
    html += "<h3>MDNS Configuration</h3>";
    html += "Hostname: <input type='text' id='mdns-name' value='esp8266-robot'><br>";
    html += "<button class='btn btn-motor' onclick='setMDNS()'>Set MDNS</button>";
    html += "</div></div>";
    return html;
}

String AdvancedPage::getFirmwareUpdateCard() {
    String html = "<div class='card card-speed'>";
    html += "<h2>Firmware Update</h2>";
    html += "<div class='control-item'>";
    html += "<form method='POST' action='/api/update' enctype='multipart/form-data' id='update-form'>";
    html += "<input type='file' id='firmware' name='firmware' accept='.bin'><br>";
    html += "<div id='update-progress' style='display:none'>";
    html += "<progress id='upload-progress' value='0' max='100'></progress>";
    html += "<span id='progress-text'>0%</span>";
    html += "</div>";
    html += "<button class='btn btn-speed' type='button' onclick='uploadFirmware()'>Update Firmware</button>";
    html += "</form>";
    html += "<div id='update-status'></div>";
    html += "</div></div>";
    return html;
}

String AdvancedPage::getCalibrationCard() {
    String html = "<div class='card card-led'>";
    html += "<h2>Sensor Calibration</h2>";
    html += "<div class='control-item'>";
    html += "<h3>IMU Calibration</h3>";
    html += "<button class='btn btn-led' onclick='calibrateIMU()'>Calibrate IMU</button>";
    html += "<div id='imu-status'></div>";
    html += "<h3>Motor Calibration</h3>";
    html += "<button class='btn btn-led' onclick='calibrateMotors()'>Calibrate Motors</button>";
    html += "<div id='motor-status'></div>";
    html += "</div></div>";
    return html;
}

String AdvancedPage::getCustomCommandCard() {
    String html = "<div class='card card-buzzer'>";
    html += "<h2>Custom Commands</h2>";
    html += "<div class='control-item'>";
    html += "<div style='display:flex;gap:10px;margin-bottom:10px'>";
    html += "<input type='text' id='command-input' style='flex:1' placeholder='Enter command...'>";
    html += "<button class='btn btn-buzzer' onclick='sendCommand()'>Send</button>";
    html += "</div>";
    html += "<pre id='command-output' style='background:#1a1a1a;color:#fff;padding:10px;height:200px;overflow-y:auto'></pre>";
    html += "</div></div>";
    return html;
}

String AdvancedPage::getDataVisualizationCard() {
    String html = "<div class='card card-speed'>";
    html += "<h2>Data Visualization</h2>";
    html += "<div class='control-item'>";
    html += "<div style='display:flex;gap:10px;margin-bottom:10px'>";
    html += "<select id='data-type'>";
    html += "<option value='imu'>IMU Data</option>";
    html += "<option value='motors'>Motor Data</option>";
    html += "<option value='power'>Power Data</option>";
    html += "</select>";
    html += "<button class='btn btn-speed' onclick='toggleDataStream()'>Start/Stop</button>";
    html += "</div>";
    html += "<canvas id='data-canvas' width='600' height='300' style='background:#fff'></canvas>";
    html += "</div></div>";
    return html;
}

String AdvancedPage::getAdvancedScript() {
    String script = "<script>";

    // 系统信息更新
    script += "function refreshSystemInfo() {";
    script += "  fetch('/api/system/info')";
    script += "  .then(response => response.json())";
    script += "  .then(data => {";
    script += "    document.getElementById('free-heap').textContent = data.freeHeap;";
    script += "    document.getElementById('rssi').textContent = data.rssi;";
    script += "  });";
    script += "}";

    // 设置MDNS
    script += "function setMDNS() {";
    script += "  const hostname = document.getElementById('mdns-name').value;";
    script += "  fetch('/api/network/mdns', {";
    script += "    method: 'POST',";
    script += "    headers: {'Content-Type': 'application/json'},";
    script += "    body: JSON.stringify({hostname: hostname})";
    script += "  }).then(response => response.json())";
    script += "  .then(data => alert('MDNS hostname set to: ' + hostname));";
    script += "}";

    // 固件更新
    script += "function uploadFirmware() {";
    script += "  const form = document.getElementById('update-form');";
    script += "  const file = document.getElementById('firmware').files[0];";
    script += "  if (!file) {";
    script += "    alert('Please select a firmware file');";
    script += "    return;";
    script += "  }";
    script += "  const formData = new FormData(form);";
    script += "  const progress = document.getElementById('update-progress');";
    script += "  const bar = document.getElementById('upload-progress');";
    script += "  const text = document.getElementById('progress-text');";
    script += "  progress.style.display = 'block';";
    script += "  fetch('/api/update', {";
    script += "    method: 'POST',";
    script += "    body: formData";
    script += "  }).then(response => response.json())";
    script += "  .then(data => {";
    script += "    document.getElementById('update-status').textContent = 'Update successful. Rebooting...';";
    script += "    setTimeout(() => window.location.reload(), 5000);";
    script += "  }).catch(error => {";
    script += "    document.getElementById('update-status').textContent = 'Update failed: ' + error;";
    script += "  });";
    script += "}";

    // 传感器校准
    script += "function calibrateIMU() {";
    script += "  const status = document.getElementById('imu-status');";
    script += "  status.textContent = 'Calibrating IMU...';";
    script += "  fetch('/api/calibrate/imu', {method: 'POST'})";
    script += "  .then(response => response.json())";
    script += "  .then(data => status.textContent = data.message);";
    script += "}";

    script += "function calibrateMotors() {";
    script += "  const status = document.getElementById('motor-status');";
    script += "  status.textContent = 'Calibrating motors...';";
    script += "  fetch('/api/calibrate/motors', {method: 'POST'})";
    script += "  .then(response => response.json())";
    script += "  .then(data => status.textContent = data.message);";
    script += "}";

    // 自定义命令
    script += "function sendCommand() {";
    script += "  const cmd = document.getElementById('command-input').value;";
    script += "  const output = document.getElementById('command-output');";
    script += "  output.textContent += '> ' + cmd + '\\n';";
    script += "  fetch('/api/system/command', {";
    script += "    method: 'POST',";
    script += "    headers: {'Content-Type': 'application/json'},";
    script += "    body: JSON.stringify({command: cmd})";
    script += "  }).then(response => response.json())";
    script += "  .then(data => {";
    script += "    output.textContent += JSON.stringify(data, null, 2) + '\\n';";
    script += "    output.scrollTop = output.scrollHeight;";
    script += "  });";
    script += "  document.getElementById('command-input').value = '';";
    script += "}";

    // 数据可视化
    script += "let dataStreamInterval = null;";
    script += "let dataChart = null;";

    script += "function initChart() {";
    script += "  const ctx = document.getElementById('data-canvas').getContext('2d');";
    script += "  const data = [];";
    script += "  // TODO: 初始化图表";
    script += "}";

    script += "function toggleDataStream() {";
    script += "  if (dataStreamInterval) {";
    script += "    clearInterval(dataStreamInterval);";
    script += "    dataStreamInterval = null;";
    script += "  } else {";
    script += "    const type = document.getElementById('data-type').value;";
    script += "    dataStreamInterval = setInterval(() => updateData(type), 100);";
    script += "  }";
    script += "}";

    script += "function updateData(type) {";
    script += "  fetch(`/api/data/${type}`)";
    script += "  .then(response => response.json())";
    script += "  .then(data => {";
    script += "    // TODO: 更新图表数据";
    script += "  });";
    script += "}";

    // 自动更新和初始化
    script += "setInterval(refreshSystemInfo, 1000);";  // 每秒更新系统信息
    script += "document.getElementById('command-input').addEventListener('keyup', function(e) {";
    script += "  if (e.key === 'Enter') sendCommand();";
    script += "});";

    script += "</script>";
    return script;
}