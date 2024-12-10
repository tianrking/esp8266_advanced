#include "page_log.h"

void LogPage::handleLog(ESP8266WebServer* server) {
    String html = HtmlCommon::getHeader("System Logs");
    html += HtmlCommon::getNavBar("log");
    
    html += "<div class='container'>";
    html += getLogDisplayCard();
    html += getLogFilterCard();
    html += getLogSettingsCard();
    html += "</div>";
    
    html += getLogScript();
    html += HtmlCommon::getFooter();
    
    server->send(200, "text/html", html);
}

String LogPage::getLogDisplayCard() {
    String html = "<div class='card card-status'>";
    html += "<h2>System Logs</h2>";
    html += "<div class='control-item'>";
    html += "<div style='display:flex;justify-content:space-between;margin-bottom:10px'>";
    html += "<button class='btn btn-status' onclick='refreshLogs()'>Refresh</button>";
    html += "<button class='btn btn-status' onclick='clearLogs()'>Clear</button>";
    html += "<button class='btn btn-status' onclick='toggleAutoRefresh()'>Auto Refresh</button>";
    html += "<button class='btn btn-status' onclick='downloadLogs()'>Download</button>";
    html += "</div>";
    html += "<pre id='log-content' style='height:400px;overflow-y:auto;background:#fff;padding:10px;font-size:14px;line-height:1.5'></pre>";
    html += "</div></div>";
    return html;
}

String LogPage::getLogFilterCard() {
    String html = "<div class='card card-motor'>";
    html += "<h2>Log Filter</h2>";
    html += "<div class='control-item'>";
    html += "<div style='margin-bottom:10px'>";
    html += "Search: <input type='text' id='log-filter' style='width:200px' onkeyup='filterLogs()'>";
    html += "</div><div>";
    html += "Level: <select id='log-level' onchange='filterLogs()'>";
    html += "<option value='all'>All</option>";
    html += "<option value='info'>Info</option>";
    html += "<option value='warning'>Warning</option>";
    html += "<option value='error'>Error</option>";
    html += "</select>";
    html += " Time Range: <select id='log-time' onchange='filterLogs()'>";
    html += "<option value='all'>All Time</option>";
    html += "<option value='1h'>Last Hour</option>";
    html += "<option value='24h'>Last 24 Hours</option>";
    html += "<option value='7d'>Last 7 Days</option>";
    html += "</select>";
    html += "</div></div></div>";
    return html;
}

String LogPage::getLogSettingsCard() {
    String html = "<div class='card card-speed'>";
    html += "<h2>Log Settings</h2>";
    html += "<div class='control-item'>";
    html += "<div style='margin-bottom:10px'>";
    html += "Auto Refresh Interval: <input type='number' id='refresh-interval' value='5' min='1'> seconds";
    html += "</div><div>";
    html += "Max Log Entries: <input type='number' id='max-logs' value='100' min='10'>";
    html += "</div><div style='margin-top:10px'>";
    html += "<button class='btn btn-speed' onclick='saveLogSettings()'>Save Settings</button>";
    html += "</div></div></div>";
    return html;
}

String LogPage::getLogScript() {
    String script = "<script>";
    
    // 变量定义
    script += "let autoRefreshInterval = null;";
    script += "let rawLogs = [];";
    
    // 刷新日志
    script += "function refreshLogs() {";
    script += "fetch('/api/log/data')";
    script += ".then(response => response.json())";
    script += ".then(data => {";
    script += "  rawLogs = data;";
    script += "  filterLogs();";
    script += "}).catch(error => console.error('Error:', error));}";
    
    // 清除日志
    script += "function clearLogs() {";
    script += "if(confirm('Clear all logs?')) {";
    script += "  fetch('/api/log/clear', {method:'POST'})";
    script += "  .then(() => {";
    script += "    rawLogs = [];";
    script += "    document.getElementById('log-content').innerHTML = '';";
    script += "    alert('Logs cleared');";
    script += "  }).catch(error => console.error('Error:', error));}}";
    
    // 自动刷新
    script += "function toggleAutoRefresh() {";
    script += "if(autoRefreshInterval) {";
    script += "  clearInterval(autoRefreshInterval);";
    script += "  autoRefreshInterval = null;";
    script += "} else {";
    script += "  const interval = document.getElementById('refresh-interval').value * 1000;";
    script += "  autoRefreshInterval = setInterval(refreshLogs, interval);}}";
    
    // 过滤日志
    script += "function filterLogs() {";
    script += "const filter = document.getElementById('log-filter').value.toLowerCase();";
    script += "const level = document.getElementById('log-level').value;";
    script += "const timeRange = document.getElementById('log-time').value;";
    script += "let filteredLogs = rawLogs;";
    script += "if(filter) {";
    script += "  filteredLogs = filteredLogs.filter(log => log.toLowerCase().includes(filter));}";
    script += "if(level !== 'all') {";
    script += "  filteredLogs = filteredLogs.filter(log => log.toLowerCase().includes('['+level+']'));}";
    script += "if(timeRange !== 'all') {";
    script += "  const now = Date.now();";
    script += "  const ranges = {'1h':3600000,'24h':86400000,'7d':604800000};";
    script += "  const range = ranges[timeRange];";
    script += "  filteredLogs = filteredLogs.filter(log => {";
    script += "    const timestamp = new Date(log.split('[')[0]).getTime();";
    script += "    return (now - timestamp) <= range;";
    script += "  });}";
    script += "const logContent = document.getElementById('log-content');";
    script += "logContent.innerHTML = filteredLogs.join('\\n');";
    script += "logContent.scrollTop = logContent.scrollHeight;}";
    
    // 下载日志
    script += "function downloadLogs() {";
    script += "const logText = document.getElementById('log-content').innerText;";
    script += "const blob = new Blob([logText], {type: 'text/plain'});";
    script += "const url = window.URL.createObjectURL(blob);";
    script += "const a = document.createElement('a');";
    script += "a.href = url;";
    script += "a.download = 'robot_logs_' + new Date().toISOString() + '.txt';";
    script += "document.body.appendChild(a);";
    script += "a.click();";
    script += "window.URL.revokeObjectURL(url);";
    script += "document.body.removeChild(a);}";
    
    // 保存设置
    script += "function saveLogSettings() {";
    script += "const settings = {";
    script += "  refreshInterval: document.getElementById('refresh-interval').value,";
    script += "  maxLogs: document.getElementById('max-logs').value";
    script += "};";
    script += "fetch('/api/settings/save', {";
    script += "  method: 'POST',";
    script += "  headers: {'Content-Type': 'application/json'},";
    script += "  body: JSON.stringify({type:'log', ...settings})";
    script += "}).then(() => alert('Log settings saved'))";
    script += ".catch(error => console.error('Error:', error));}";
    
    // 初始化
    script += "refreshLogs();";  // 初始加载日志
    
    script += "</script>";
    
    // 添加样式
    script += "<style>";
    script += "#log-content::-webkit-scrollbar{width:8px}";
    script += "#log-content::-webkit-scrollbar-track{background:#f1f1f1}";
    script += "#log-content::-webkit-scrollbar-thumb{background:#888;border-radius:4px}";
    script += "#log-content::-webkit-scrollbar-thumb:hover{background:#555}";
    script += "</style>";
    
    return script;
}