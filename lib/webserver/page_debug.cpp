#include "page_debug.h"
#include <ESP8266WiFi.h>

void DebugPage::handleDebug(ESP8266WebServer* server) {
    String html = HtmlCommon::getHeader("Debug Console");
    html += HtmlCommon::getNavBar("debug");
    
    html += "<div class='container'>";
    html += getConsoleCard();
    html += getSystemTestCard();
    html += getMemoryMonitorCard();
    html += getNetworkMonitorCard();
    html += getPerformanceCard();
    html += getDebugControlCard();
    html += "</div>";
    
    html += getDebugScript();
    html += HtmlCommon::getFooter();
    
    server->send(200, "text/html", html);
}

String DebugPage::getConsoleCard() {
    String html = "<div class='card card-status'>";
    html += "<h2>Debug Console</h2>";
    html += "<div class='control-item'>";
    
    // 调试控制台输出区
    html += "<pre id='debug-output' style='height:300px;overflow-y:auto;background:#1a1a1a;color:#fff;padding:10px;font-family:monospace'></pre>";
    
    // 命令输入区
    html += "<div style='display:flex;gap:10px;margin-top:10px'>";
    html += "<input type='text' id='debug-input' style='flex:1' placeholder='Enter debug command...'>";
    html += "<button class='btn btn-status' onclick='sendDebugCommand()'>Send</button>";
    html += "<button class='btn btn-status' onclick='clearConsole()'>Clear</button>";
    html += "</div>";
    
    html += "<div class='debug-controls' style='margin-top:10px'>";
    html += "<label><input type='checkbox' id='auto-scroll'> Auto Scroll</label> ";
    html += "<label><input type='checkbox' id='timestamp'> Show Timestamp</label> ";
    html += "<select id='log-level' onchange='setLogLevel()'>";
    html += "<option value='0'>Debug</option>";
    html += "<option value='1'>Info</option>";
    html += "<option value='2'>Warning</option>";
    html += "<option value='3'>Error</option>";
    html += "</select>";
    html += "</div>";
    
    html += "</div></div>";
    return html;
}

String DebugPage::getSystemTestCard() {
    String html = "<div class='card card-motor'>";
    html += "<h2>System Tests</h2>";
    html += "<div class='control-item'>";
    
    // 系统测试按钮组
    html += "<div class='test-buttons' style='display:grid;grid-template-columns:repeat(2,1fr);gap:10px'>";
    html += "<button class='btn btn-motor' onclick='runTest(\"memory\")'>Memory Test</button>";
    html += "<button class='btn btn-motor' onclick='runTest(\"wifi\")'>WiFi Test</button>";
    html += "<button class='btn btn-motor' onclick='runTest(\"motors\")'>Motor Test</button>";
    html += "<button class='btn btn-motor' onclick='runTest(\"sensors\")'>Sensor Test</button>";
    html += "<button class='btn btn-motor' onclick='runTest(\"storage\")'>Storage Test</button>";
    html += "<button class='btn btn-motor' onclick='runTest(\"network\")'>Network Test</button>";
    html += "</div>";
    
    // 测试结果显示
    html += "<div id='test-results' style='margin-top:10px;padding:10px;background:#f8fafc;border-radius:8px'>";
    html += "<p>Test Results:</p>";
    html += "<pre id='test-output' style='margin:0'></pre>";
    html += "</div>";
    
    html += "</div></div>";
    return html;
}

String DebugPage::getMemoryMonitorCard() {
    String html = "<div class='card card-speed'>";
    html += "<h2>Memory Monitor</h2>";
    html += "<div class='control-item'>";
    
    // 内存使用情况图表
    html += "<canvas id='memory-chart' width='400' height='200'></canvas>";
    
    // 内存信息详情
    html += "<div id='memory-info' style='margin-top:10px'>";
    html += "<p>Free Heap: <span id='free-heap'>--</span> bytes</p>";
    html += "<p>Heap Fragmentation: <span id='heap-frag'>--</span>%</p>";
    html += "<p>Stack Usage: <span id='stack-usage'>--</span> bytes</p>";
    html += "</div>";
    
    // 内存监控控制
    html += "<div style='margin-top:10px'>";
    html += "<button class='btn btn-speed' onclick='toggleMemoryMonitor()'>Start/Stop Monitor</button>";
    html += "Update Interval: <input type='number' id='memory-interval' value='1000' min='100' max='5000'> ms";
    html += "</div>";
    
    html += "</div></div>";
    return html;
}

String DebugPage::getNetworkMonitorCard() {
    String html = "<div class='card card-led'>";
    html += "<h2>Network Monitor</h2>";
    html += "<div class='control-item'>";
    
    // 网络状态图表
    html += "<canvas id='network-chart' width='400' height='200'></canvas>";
    
    // 网络信息详情
    html += "<div id='network-info' style='margin-top:10px'>";
    html += "<p>WiFi RSSI: <span id='wifi-rssi'>--</span> dBm</p>";
    html += "<p>TX Rate: <span id='tx-rate'>--</span> bytes/s</p>";
    html += "<p>RX Rate: <span id='rx-rate'>--</span> bytes/s</p>";
    html += "<p>Packet Loss: <span id='packet-loss'>--</span>%</p>";
    html += "</div>";
    
    // 网络测试工具
    html += "<div style='margin-top:10px'>";
    html += "<input type='text' id='ping-host' placeholder='Host to ping' style='width:200px'>";
    html += "<button class='btn btn-led' onclick='pingHost()'>Ping</button>";
    html += "</div>";
    
    html += "</div></div>";
    return html;
}

String DebugPage::getPerformanceCard() {
    String html = "<div class='card card-buzzer'>";
    html += "<h2>Performance Monitor</h2>";
    html += "<div class='control-item'>";
    
    // CPU使用率图表
    html += "<canvas id='cpu-chart' width='400' height='200'></canvas>";
    
    // 性能指标详情
    html += "<div id='performance-info' style='margin-top:10px'>";
    html += "<p>CPU Load: <span id='cpu-load'>--</span>%</p>";
    html += "<p>Loop Time: <span id='loop-time'>--</span> ms</p>";
    html += "<p>Task Queue: <span id='task-queue'>--</span></p>";
    html += "</div>";
    
    // 性能分析工具
    html += "<div style='margin-top:10px'>";
    html += "<button class='btn btn-buzzer' onclick='startProfiling()'>Start Profiling</button>";
    html += "<button class='btn btn-buzzer' onclick='stopProfiling()'>Stop Profiling</button>";
    html += "</div>";
    
    html += "</div></div>";
    return html;
}

String DebugPage::getDebugControlCard() {
    String html = "<div class='card card-status'>";
    html += "<h2>Debug Controls</h2>";
    html += "<div class='control-item'>";
    
    // 调试选项
    html += "<div class='debug-options'>";
    html += "<label><input type='checkbox' id='verbose-mode'> Verbose Mode</label><br>";
    html += "<label><input type='checkbox' id='debug-output'> Debug Output</label><br>";
    html += "<label><input type='checkbox' id='safety-checks'> Safety Checks</label><br>";
    html += "</div>";
    
    // 调试工具
    html += "<div style='margin-top:10px'>";
    html += "<button class='btn btn-status' onclick='saveDebugDump()'>Save Debug Dump</button>";
    html += "<button class='btn btn-status' onclick='loadTestData()'>Load Test Data</button>";
    html += "<button class='btn btn-status' onclick='resetDebugState()'>Reset Debug State</button>";
    html += "</div>";
    
    html += "</div></div>";
    return html;
}

String DebugPage::getDebugScript() {
    String script = "<script>";
    
    // 调试控制台功能
    script += "let debugAutoScroll = true;";
    script += "let debugShowTimestamp = true;";
    
    script += "function sendDebugCommand() {";
    script += "  const cmd = document.getElementById('debug-input').value;";
    script += "  appendToConsole('> ' + cmd);";
    script += "  fetch('/api/debug/command', {";
    script += "    method: 'POST',";
    script += "    headers: {'Content-Type': 'application/json'},";
    script += "    body: JSON.stringify({command: cmd})";
    script += "  }).then(response => response.json())";
    script += "  .then(data => appendToConsole(JSON.stringify(data, null, 2)));";
    script += "  document.getElementById('debug-input').value = '';";
    script += "}";
    
    script += "function appendToConsole(text) {";
    script += "  const console = document.getElementById('debug-output');";
    script += "  if (debugShowTimestamp) {";
    script += "    const now = new Date().toISOString();";
    script += "    text = now + ' ' + text;";
    script += "  }";
    script += "  console.textContent += text + '\\n';";
    script += "  if (debugAutoScroll) {";
    script += "    console.scrollTop = console.scrollHeight;";
    script += "  }";
    script += "}";
    
    // 系统测试功能
    script += "function runTest(type) {";
    script += "  const output = document.getElementById('test-output');";
    script += "  output.textContent = 'Running ' + type + ' test...';";
    script += "  fetch('/api/debug/test/' + type)";
    script += "  .then(response => response.json())";
    script += "  .then(data => {";
    script += "    output.textContent = JSON.stringify(data, null, 2);";
    script += "    appendToConsole('Test completed: ' + type);";
    script += "  });";
    script += "}";
    
    // 内存监控功能
    script += "let memoryChart = null;";
    script += "let memoryMonitorInterval = null;";
    
    script += "function initMemoryChart() {";
    script += "  const ctx = document.getElementById('memory-chart').getContext('2d');";
    script += "  // TODO: 初始化内存使用图表";
    script += "}";
    
    script += "function toggleMemoryMonitor() {";
    script += "  if (memoryMonitorInterval) {";
    script += "    clearInterval(memoryMonitorInterval);";
    script += "    memoryMonitorInterval = null;";
    script += "  } else {";
    script += "    const interval = parseInt(document.getElementById('memory-interval').value);";
    script += "    memoryMonitorInterval = setInterval(updateMemoryInfo, interval);";
    script += "  }";
    script += "}";
    
    // 性能监控功能
    script += "function startProfiling() {";
    script += "  fetch('/api/debug/profile/start', {method: 'POST'})";
    script += "  .then(response => response.json())";
    script += "  .then(data => appendToConsole('Profiling started'));";
    script += "}";
    
    script += "function stopProfiling() {";
    script += "  fetch('/api/debug/profile/stop', {method: 'POST'})";
    script += "  .then(response => response.json())";
    script += "  .then(data => {";
    script += "    appendToConsole('Profiling results:');";
    script += "    appendToConsole(JSON.stringify(data, null, 2));";
    script += "  });";
    script += "}";
    
    // 页面初始化
    script += "document.addEventListener('DOMContentLoaded', function() {";
    script += "  initMemoryChart();";
    script += "  document.getElementById('debug-input').addEventListener('keyup', function(e) {";
    script += "    if (e.key === 'Enter') sendDebugCommand();";
    script += "  });";
    script += "  document.getElementById('auto-scroll').checked = debugAutoScroll;";
    script += "  document.getElementById('timestamp').checked = debugShowTimestamp;";
    script += "});";
    
    script += "</script>";
    return script;
}