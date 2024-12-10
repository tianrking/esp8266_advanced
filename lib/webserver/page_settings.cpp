#include "page_settings.h"

void SettingsPage::handleSettings(ESP8266WebServer* server) {
    String html = HtmlCommon::getHeader("System Settings");
    html += HtmlCommon::getNavBar("settings");
    
    html += "<div class='container'>";
    html += getWiFiSettingsCard();
    html += getMotorSettingsCard();
    html += getSensorSettingsCard();
    html += getSystemSettingsCard();
    html += "</div>";
    
    html += getSettingsScript();
    html += HtmlCommon::getFooter();
    
    server->send(200, "text/html", html);
}

String SettingsPage::getWiFiSettingsCard() {
    String html = "<div class='card card-status'>";
    html += "<h2>WiFi Settings</h2>";
    html += "<div class='control-item'>";
    html += "<div class='form-group'>";
    html += "SSID: <input type='text' id='wifi-ssid'><br>";
    html += "Password: <input type='password' id='wifi-password'><br>";
    html += "<label><input type='checkbox' id='static-ip'> Use Static IP</label><br>";
    html += "<div id='static-ip-fields' style='display:none'>";
    html += "IP: <input type='text' id='static-ip-addr'><br>";
    html += "Gateway: <input type='text' id='static-gateway'><br>";
    html += "Subnet: <input type='text' id='static-subnet'><br>";
    html += "</div>";
    html += "<button class='btn btn-status' onclick='saveWiFiSettings()'>Save WiFi Settings</button>";
    html += "</div></div></div>";
    return html;
}

String SettingsPage::getMotorSettingsCard() {
    String html = "<div class='card card-motor'>";
    html += "<h2>Motor Settings</h2>";
    html += "<div class='control-item'>";
    html += "Motor 1 Max Speed: <input type='number' id='motor1-max' min='0' max='255'><br>";
    html += "Motor 2 Max Speed: <input type='number' id='motor2-max' min='0' max='255'><br>";
    html += "Motor Dead Zone: <input type='number' id='motor-deadzone' min='0' max='50'><br>";
    html += "Motor Ramp Rate: <input type='number' id='motor-ramp' min='1' max='20'><br>";
    html += "<button class='btn btn-motor' onclick='saveMotorSettings()'>Save Motor Settings</button>";
    html += "</div></div>";
    return html;
}

String SettingsPage::getSensorSettingsCard() {
    String html = "<div class='card card-speed'>";
    html += "<h2>Sensor Settings</h2>";
    html += "<div class='control-item'>";
    html += "Update Interval (ms): <input type='number' id='sensor-interval' min='50' max='1000'><br>";
    html += "<label><input type='checkbox' id='sensor-auto'> Auto Update</label><br>";
    html += "Filter Level: <select id='sensor-filter'>";
    html += "<option value='0'>None</option>";
    html += "<option value='1'>Light</option>";
    html += "<option value='2'>Medium</option>";
    html += "<option value='3'>Heavy</option>";
    html += "</select><br>";
    html += "<button class='btn btn-speed' onclick='saveSensorSettings()'>Save Sensor Settings</button>";
    html += "</div></div>";
    return html;
}

String SettingsPage::getSystemSettingsCard() {
    String html = "<div class='card card-led'>";
    html += "<h2>System Settings</h2>";
    html += "<div class='control-item'>";
    html += "Log Level: <select id='log-level'>";
    html += "<option value='0'>Debug</option>";
    html += "<option value='1'>Info</option>";
    html += "<option value='2'>Warning</option>";
    html += "<option value='3'>Error</option>";
    html += "</select><br>";
    html += "Max Log Entries: <input type='number' id='max-logs' min='50' max='500'><br>";
    html += "<label><input type='checkbox' id='watchdog-enabled'> Enable Watchdog</label><br>";
    html += "Watchdog Timeout (ms): <input type='number' id='watchdog-timeout' min='500' max='5000'><br>";
    html += "<button class='btn btn-led' onclick='saveSystemSettings()'>Save System Settings</button>";
    html += "<button class='btn btn-led' onclick='if(confirm(\"Reset all settings?\"))resetSettings()'>Reset All Settings</button>";
    html += "</div></div>";
    return html;
}

String SettingsPage::getSettingsScript() {
    String script = "<script>";
    
    // 加载设置
    script += "function loadSettings() {";
    script += "  fetch('/api/settings/load')";
    script += "  .then(response => response.json())";
    script += "  .then(data => {";
    script += "    // WiFi设置";
    script += "    document.getElementById('wifi-ssid').value = data.wifi.ssid;";
    script += "    document.getElementById('static-ip').checked = data.wifi.staticIP;";
    script += "    document.getElementById('static-ip-addr').value = data.wifi.ip;";
    script += "    document.getElementById('static-gateway').value = data.wifi.gateway;";
    script += "    document.getElementById('static-subnet').value = data.wifi.subnet;";
    script += "    document.getElementById('static-ip-fields').style.display = data.wifi.staticIP ? 'block' : 'none';";
    
    script += "    // 电机设置";
    script += "    document.getElementById('motor1-max').value = data.motor.motor1_max;";
    script += "    document.getElementById('motor2-max').value = data.motor.motor2_max;";
    script += "    document.getElementById('motor-deadzone').value = data.motor.deadzone;";
    script += "    document.getElementById('motor-ramp').value = data.motor.ramp;";
    
    script += "    // 传感器设置";
    script += "    document.getElementById('sensor-interval').value = data.sensor.update_interval;";
    script += "    document.getElementById('sensor-auto').checked = data.sensor.auto_update;";
    script += "    document.getElementById('sensor-filter').value = data.sensor.filter_level;";
    
    script += "    // 系统设置";
    script += "    document.getElementById('log-level').value = data.system.log_level;";
    script += "    document.getElementById('max-logs').value = data.system.max_logs;";
    script += "    document.getElementById('watchdog-enabled').checked = data.system.watchdog_enabled;";
    script += "    document.getElementById('watchdog-timeout').value = data.system.watchdog_timeout;";
    script += "  });";
    script += "}";
    
    // 保存各类设置的函数
    script += "function saveWiFiSettings() {";
    script += "  const settings = {";
    script += "    wifi: {";
    script += "      ssid: document.getElementById('wifi-ssid').value,";
    script += "      password: document.getElementById('wifi-password').value,";
    script += "      staticIP: document.getElementById('static-ip').checked,";
    script += "      ip: document.getElementById('static-ip-addr').value,";
    script += "      gateway: document.getElementById('static-gateway').value,";
    script += "      subnet: document.getElementById('static-subnet').value";
    script += "    }";
    script += "  };";
    script += "  saveSettings(settings, 'WiFi');";
    script += "}";
    
    script += "function saveMotorSettings() {";
    script += "  const settings = {";
    script += "    motor: {";
    script += "      motor1_max: parseInt(document.getElementById('motor1-max').value),";
    script += "      motor2_max: parseInt(document.getElementById('motor2-max').value),";
    script += "      deadzone: parseInt(document.getElementById('motor-deadzone').value),";
    script += "      ramp: parseInt(document.getElementById('motor-ramp').value)";
    script += "    }";
    script += "  };";
    script += "  saveSettings(settings, 'Motor');";
    script += "}";
    
    script += "function saveSensorSettings() {";
    script += "  const settings = {";
    script += "    sensor: {";
    script += "      update_interval: parseInt(document.getElementById('sensor-interval').value),";
    script += "      auto_update: document.getElementById('sensor-auto').checked,";
    script += "      filter_level: parseInt(document.getElementById('sensor-filter').value)";
    script += "    }";
    script += "  };";
    script += "  saveSettings(settings, 'Sensor');";
    script += "}";
    
    script += "function saveSystemSettings() {";
    script += "  const settings = {";
    script += "    system: {";
    script += "      log_level: parseInt(document.getElementById('log-level').value),";
    script += "      max_logs: parseInt(document.getElementById('max-logs').value),";
    script += "      watchdog_enabled: document.getElementById('watchdog-enabled').checked,";
    script += "      watchdog_timeout: parseInt(document.getElementById('watchdog-timeout').value)";
    script += "    }";
    script += "  };";
    script += "  saveSettings(settings, 'System');";
    script += "}";
    
    // 通用保存设置函数
    script += "function saveSettings(settings, type) {";
    script += "  fetch('/api/settings/save', {";
    script += "    method: 'POST',";
    script += "    headers: {'Content-Type': 'application/json'},";
    script += "    body: JSON.stringify(settings)";
    script += "  }).then(response => response.json())";
    script += "  .then(data => alert(type + ' settings saved'))";
    script += "  .catch(error => alert('Error saving settings: ' + error));";
    script += "}";
    
    // 重置设置
    script += "function resetSettings() {";
    script += "  fetch('/api/settings/reset', {method: 'POST'})";
    script += "  .then(response => response.json())";
    script += "  .then(data => {";
    script += "    alert('Settings reset to defaults');";
    script += "    loadSettings();";
    script += "  });";
    script += "}";
    
    // 初始化
    script += "document.getElementById('static-ip').onchange = function() {";
    script += "  document.getElementById('static-ip-fields').style.display = this.checked ? 'block' : 'none';";
    script += "};";
    
    script += "loadSettings();";  // 页面加载时加载设置
    
    script += "</script>";
    return script;
}