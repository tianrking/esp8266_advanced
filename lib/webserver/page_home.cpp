#include "page_home.h"
#include <ESP8266WiFi.h>

void HomePage::handleRoot(ESP8266WebServer* server) {
    String html = HtmlCommon::getHeader("ESP8266 Robot");
    html += HtmlCommon::getNavBar("home");
    
    html += "<div class='container'>";
    html += getWelcomeCard();
    html += getQuickAccessCard();
    html += "</div>";
    
    html += getUptimeScript();
    html += HtmlCommon::getFooter();
    
    server->send(200, "text/html", html);
}

String HomePage::getWelcomeCard() {
    String html = "<div class='card card-status'>";
    html += "<h2>Welcome to ESP8266 Robot Control</h2>";
    html += "<div class='control-item'>";
    html += "<p>IP Address: " + WiFi.localIP().toString() + "</p>";
    html += "<p>MAC Address: " + WiFi.macAddress() + "</p>";
    html += "<p>Uptime: <span id='uptime'>0</span> seconds</p>";
    html += "<p>WiFi RSSI: " + String(WiFi.RSSI()) + " dBm</p>";
    html += "<p>Free Memory: " + String(ESP.getFreeHeap()) + " bytes</p>";
    html += "</div></div>";
    return html;
}

String HomePage::getQuickAccessCard() {
    String html = "<div class='card card-motor'>";
    html += "<h2>Quick Access</h2>";
    html += "<div class='control-item'>";
    html += "<a href='/control' class='btn btn-motor'>Control Panel</a> ";
    html += "<a href='/sensor' class='btn btn-speed'>Sensor Monitor</a> ";
    html += "<a href='/mode' class='btn btn-led'>Motion Modes</a> ";
    html += "<a href='/log' class='btn btn-buzzer'>System Logs</a>";
    html += "</div></div>";
    return html;
}

String HomePage::getUptimeScript() {
    String script = "<script>";
    script += "setInterval(() => {";
    script += "const uptime = Math.floor(Date.now() / 1000);";
    script += "document.getElementById('uptime').textContent = uptime;";
    script += "}, 1000);";
    script += "</script>";
    return script;
}