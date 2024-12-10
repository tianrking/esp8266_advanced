#include "page_mode.h"

void ModePage::handleMode(ESP8266WebServer* server) {
    String html = HtmlCommon::getHeader("Motion Modes");
    html += HtmlCommon::getNavBar("mode");
    
    html += "<div class='container'>";
    html += getModeSelectionCard();
    html += getParameterCard();
    html += getTrajectoryCard();
    html += getStatusCard();
    html += "</div>";
    
    html += getModeScript();
    html += HtmlCommon::getFooter();
    
    server->send(200, "text/html", html);
}

String ModePage::getModeSelectionCard() {
    String html = "<div class='card card-status'>";
    html += "<h2>Motion Mode Selection</h2>";
    html += "<div class='control-item'>";
    
    // 添加模式选择器
    html += "<div class='mode-selector'>";
    html += "<button class='btn btn-status mode-btn' data-mode='manual' onclick='selectMode(\"manual\")'>Manual</button>";
    html += "<button class='btn btn-status mode-btn' data-mode='auto' onclick='selectMode(\"auto\")'>Autonomous</button>";
    html += "<button class='btn btn-status mode-btn' data-mode='follow' onclick='selectMode(\"follow\")'>Line Follow</button>";
    html += "<button class='btn btn-status mode-btn' data-mode='avoid' onclick='selectMode(\"avoid\")'>Obstacle Avoid</button>";
    html += "</div>";
    
    // 添加模式状态显示
    html += "<div class='mode-status'>";
    html += "<p>Current Mode: <span id='current-mode'>Manual</span></p>";
    html += "<p>Status: <span id='mode-status'>Stopped</span></p>";
    html += "</div>";
    
    html += "</div></div>";
    return html;
}

String ModePage::getParameterCard() {
    String html = "<div class='card card-motor parameter-cards'>";
    html += "<h2>Mode Parameters</h2>";
    
    // Manual Mode Parameters
    html += "<div class='control-item' id='manual-params'>";
    html += "<h3>Manual Control Parameters</h3>";
    html += "Max Speed: <input type='number' id='manual-max-speed' value='255'><br>";
    html += "Speed Ramp: <input type='number' id='manual-ramp' value='10'><br>";
    html += "Dead Zone: <input type='number' id='manual-deadzone' value='5'><br>";
    html += "<button class='btn btn-motor' onclick='saveParams(\"manual\")'>Save</button>";
    html += "</div>";
    
    // Autonomous Mode Parameters
    html += "<div class='control-item' id='auto-params' style='display:none'>";
    html += "<h3>Autonomous Parameters</h3>";
    html += "Navigation Mode: <select id='auto-nav-mode'>";
    html += "<option value='astar'>A* Algorithm</option>";
    html += "<option value='rrt'>RRT Algorithm</option>";
    html += "</select><br>";
    html += "Target X: <input type='number' id='auto-target-x' value='0'><br>";
    html += "Target Y: <input type='number' id='auto-target-y' value='0'><br>";
    html += "Safe Distance: <input type='number' id='auto-safe-dist' value='30'><br>";
    html += "<button class='btn btn-motor' onclick='saveParams(\"auto\")'>Save</button>";
    html += "</div>";
    
    // Line Following Parameters
    html += "<div class='control-item' id='follow-params' style='display:none'>";
    html += "<h3>Line Following Parameters</h3>";
    html += "Base Speed: <input type='number' id='follow-base-speed' value='150'><br>";
    html += "Kp: <input type='number' id='follow-kp' value='1.0' step='0.1'><br>";
    html += "Ki: <input type='number' id='follow-ki' value='0.0' step='0.1'><br>";
    html += "Kd: <input type='number' id='follow-kd' value='0.0' step='0.1'><br>";
    html += "<button class='btn btn-motor' onclick='saveParams(\"follow\")'>Save</button>";
    html += "</div>";
    
    // Obstacle Avoidance Parameters
    html += "<div class='control-item' id='avoid-params' style='display:none'>";
    html += "<h3>Obstacle Avoidance Parameters</h3>";
    html += "Detection Range: <input type='number' id='avoid-range' value='50'><br>";
    html += "Turn Speed: <input type='number' id='avoid-turn-speed' value='150'><br>";
    html += "Safe Distance: <input type='number' id='avoid-safe-dist' value='30'><br>";
    html += "Strategy: <select id='avoid-strategy'>";
    html += "<option value='stop'>Stop and Turn</option>";
    html += "<option value='continuous'>Continuous Motion</option>";
    html += "</select><br>";
    html += "<button class='btn btn-motor' onclick='saveParams(\"avoid\")'>Save</button>";
    html += "</div>";
    
    html += "</div>";
    return html;
}

String ModePage::getTrajectoryCard() {
    String html = "<div class='card card-speed'>";
    html += "<h2>Trajectory Visualization</h2>";
    html += "<div class='control-item'>";
    html += "<canvas id='trajectory-canvas' width='400' height='400' style='background:#fff'></canvas>";
    html += "<div class='trajectory-controls'>";
    html += "<button class='btn btn-speed' onclick='clearTrajectory()'>Clear</button>";
    html += "<button class='btn btn-speed' onclick='saveTrajectory()'>Save</button>";
    html += "<button class='btn btn-speed' onclick='toggleTrajectoryTrace()'>Toggle Trace</button>";
    html += "</div>";
    html += "</div></div>";
    return html;
}

String ModePage::getStatusCard() {
    String html = "<div class='card card-led'>";
    html += "<h2>Real-time Status</h2>";
    html += "<div class='control-item'>";
    html += "<div id='realtime-status' style='font-family:monospace;white-space:pre'></div>";
    html += "<canvas id='status-canvas' width='200' height='200'></canvas>";
    html += "</div></div>";
    return html;
}

String ModePage::getModeScript() {
    String script = "<script>";
    
    // 变量定义
    script += "let currentMode = 'manual';";
    script += "let trajectoryCtx = null;";
    script += "let statusCtx = null;";
    script += "let updateInterval = null;";
    script += "let showTrace = true;";
    script += "let trajectoryPoints = [];";
    
    // 初始化函数
    script += "function initCanvases() {";
    script += "  trajectoryCtx = document.getElementById('trajectory-canvas').getContext('2d');";
    script += "  statusCtx = document.getElementById('status-canvas').getContext('2d');";
    script += "  drawGrid();";
    script += "  startStatusUpdate();";
    script += "}";
    
    // 绘制网格
    script += "function drawGrid() {";
    script += "  const canvas = trajectoryCtx.canvas;";
    script += "  trajectoryCtx.strokeStyle = '#e2e8f0';";
    script += "  trajectoryCtx.beginPath();";
    script += "  for(let i = 0; i <= canvas.width; i += 40) {";
    script += "    trajectoryCtx.moveTo(i, 0);";
    script += "    trajectoryCtx.lineTo(i, canvas.height);";
    script += "    trajectoryCtx.moveTo(0, i);";
    script += "    trajectoryCtx.lineTo(canvas.width, i);";
    script += "  }";
    script += "  trajectoryCtx.stroke();";
    script += "}";
    
    // 模式选择
    script += "function selectMode(mode) {";
    script += "  currentMode = mode;";
    script += "  document.getElementById('current-mode').textContent = mode.charAt(0).toUpperCase() + mode.slice(1);";
    script += "  document.querySelectorAll('.parameter-cards .control-item').forEach(item => item.style.display = 'none');";
    script += "  document.getElementById(mode + '-params').style.display = 'block';";
    script += "  document.querySelectorAll('.mode-btn').forEach(btn => btn.classList.remove('active'));";
    script += "  document.querySelector(`[data-mode=\"${mode}\"]`).classList.add('active');";
    script += "  fetch('/api/mode/save', {";
    script += "    method: 'POST',";
    script += "    headers: {'Content-Type': 'application/json'},";
    script += "    body: JSON.stringify({mode: mode})";
    script += "  });";
    script += "}";
    
    // 保存参数
    script += "function saveParams(mode) {";
    script += "  const params = {};";
    script += "  switch(mode) {";
    script += "    case 'manual':";
    script += "      params.maxSpeed = parseInt(document.getElementById('manual-max-speed').value);";
    script += "      params.ramp = parseInt(document.getElementById('manual-ramp').value);";
    script += "      params.deadzone = parseInt(document.getElementById('manual-deadzone').value);";
    script += "      break;";
    script += "    case 'auto':";
    script += "      params.navMode = document.getElementById('auto-nav-mode').value;";
    script += "      params.targetX = parseInt(document.getElementById('auto-target-x').value);";
    script += "      params.targetY = parseInt(document.getElementById('auto-target-y').value);";
    script += "      params.safeDist = parseInt(document.getElementById('auto-safe-dist').value);";
    script += "      break;";
    script += "    case 'follow':";
    script += "      params.baseSpeed = parseInt(document.getElementById('follow-base-speed').value);";
    script += "      params.kp = parseFloat(document.getElementById('follow-kp').value);";
    script += "      params.ki = parseFloat(document.getElementById('follow-ki').value);";
    script += "      params.kd = parseFloat(document.getElementById('follow-kd').value);";
    script += "      break;";
    script += "    case 'avoid':";
    script += "      params.range = parseInt(document.getElementById('avoid-range').value);";
    script += "      params.turnSpeed = parseInt(document.getElementById('avoid-turn-speed').value);";
    script += "      params.safeDist = parseInt(document.getElementById('avoid-safe-dist').value);";
    script += "      params.strategy = document.getElementById('avoid-strategy').value;";
    script += "      break;";
    script += "  }";
    script += "  fetch('/api/mode/save', {";
    script += "    method: 'POST',";
    script += "    headers: {'Content-Type': 'application/json'},";
    script += "    body: JSON.stringify({mode: mode, params: params})";
    script += "  }).then(response => response.json())";
    script += "  .then(data => alert('Parameters saved'))";
    script += "  .catch(error => alert('Error: ' + error));";
    script += "}";
    
    // 轨迹相关函数
    script += "function updateTrajectory(x, y, theta) {";
    script += "  trajectoryPoints.push({x, y, theta});";
    script += "  if(showTrace) {";
    script += "    const canvas = trajectoryCtx.canvas;";
    script += "    const scale = canvas.width / 400;  // 假设400x400cm的实际空间";
    script += "    const px = x * scale + canvas.width/2;";
    script += "    const py = canvas.height/2 - y * scale;";
    script += "    trajectoryCtx.fillStyle = '#6366f1';";
    script += "    trajectoryCtx.beginPath();";
    script += "    trajectoryCtx.arc(px, py, 2, 0, Math.PI*2);";
    script += "    trajectoryCtx.fill();";
    script += "  }";
    script += "  drawRobot(x, y, theta);";
    script += "}";
    
    // 绘制机器人
    script += "function drawRobot(x, y, theta) {";
    script += "  const canvas = trajectoryCtx.canvas;";
    script += "  const scale = canvas.width / 400;";
    script += "  const px = x * scale + canvas.width/2;";
    script += "  const py = canvas.height/2 - y * scale;";
    script += "  const size = 20;";
    script += "  trajectoryCtx.save();";
    script += "  trajectoryCtx.translate(px, py);";
    script += "  trajectoryCtx.rotate(-theta);";
    script += "  trajectoryCtx.fillStyle = '#10b981';";
    script += "  trajectoryCtx.beginPath();";
    script += "  trajectoryCtx.moveTo(size/2, 0);";
    script += "  trajectoryCtx.lineTo(-size/2, size/2);";
    script += "  trajectoryCtx.lineTo(-size/2, -size/2);";
    script += "  trajectoryCtx.closePath();";
    script += "  trajectoryCtx.fill();";
    script += "  trajectoryCtx.restore();";
    script += "}";
    
    // 清除轨迹
    script += "function clearTrajectory() {";
    script += "  trajectoryPoints = [];";
    script += "  const canvas = trajectoryCtx.canvas;";
    script += "  trajectoryCtx.clearRect(0, 0, canvas.width, canvas.height);";
    script += "  drawGrid();";
    script += "}";
    
    // 切换轨迹显示
    script += "function toggleTrajectoryTrace() {";
    script += "  showTrace = !showTrace;";
    script += "  const canvas = trajectoryCtx.canvas;";
    script += "  trajectoryCtx.clearRect(0, 0, canvas.width, canvas.height);";
    script += "  drawGrid();";
    script += "  if(showTrace) {";
    script += "    trajectoryPoints.forEach(p => updateTrajectory(p.x, p.y, p.theta));";
    script += "  } else {";
    script += "    const last = trajectoryPoints[trajectoryPoints.length - 1];";
    script += "    if(last) updateTrajectory(last.x, last.y, last.theta);";
    script += "  }";
    script += "}";
    
    // 保存轨迹
    script += "function saveTrajectory() {";
    script += "  const data = JSON.stringify(trajectoryPoints);";
    script += "  const blob = new Blob([data], {type: 'application/json'});";
    script += "  const url = URL.createObjectURL(blob);";
    script += "  const a = document.createElement('a');";
    script += "  a.href = url;";
    script += "  a.download = 'trajectory_' + new Date().toISOString() + '.json';";
    script += "  document.body.appendChild(a);";
    script += "  a.click();";
    script += "  document.body.removeChild(a);";
    script += "  URL.revokeObjectURL(url);";
    script += "}";
    
    // 状态更新相关函数
    script += "function drawStatusVisual(data) {";
    script += "  const canvas = statusCtx.canvas;";
    script += "  statusCtx.clearRect(0, 0, canvas.width, canvas.height);";
    script += "  const centerX = canvas.width/2;";
    script += "  const centerY = canvas.height/2;";
    
    // 绘制速度指示器
    script += "  statusCtx.strokeStyle = '#e2e8f0';";
    script += "  statusCtx.beginPath();";
    script += "  statusCtx.arc(centerX, centerY, 80, 0, Math.PI*2);";
    script += "  statusCtx.stroke();";
    
    // 绘制速度向量
    script += "  const speed = Math.sqrt(data.vx*data.vx + data.vy*data.vy);";
    script += "  const angle = Math.atan2(data.vy, data.vx);";
    script += "  statusCtx.strokeStyle = '#10b981';";
    script += "  statusCtx.lineWidth = 3;";
    script += "  statusCtx.beginPath();";
    script += "  statusCtx.moveTo(centerX, centerY);";
    script += "  statusCtx.lineTo(";
    script += "    centerX + Math.cos(angle) * speed * 60,";
    script += "    centerY + Math.sin(angle) * speed * 60";
    script += "  );";
    script += "  statusCtx.stroke();";
    
    // 绘制角速度指示
    script += "  const angSpeed = data.w * 30;";
    script += "  statusCtx.strokeStyle = '#6366f1';";
    script += "  statusCtx.beginPath();";
    script += "  statusCtx.arc(centerX, centerY, 40,";
    script += "    -Math.PI/2, -Math.PI/2 + angSpeed, angSpeed < 0);";
    script += "  statusCtx.stroke();";
    script += "}";
    
    // 状态更新
    script += "function updateStatus() {";
    script += "  fetch('/api/mode/status')";
    script += "  .then(response => response.json())";
    script += "  .then(data => {";
    script += "    document.getElementById('mode-status').textContent = data.status;";
    script += "    document.getElementById('realtime-status').innerHTML = ";
    script += "      `Speed: ${data.speed.toFixed(2)} m/s\\n` +";
    script += "      `Angular: ${data.angular.toFixed(2)} rad/s\\n` +";
    script += "      `Position X: ${data.x.toFixed(2)} cm\\n` +";
    script += "      `Position Y: ${data.y.toFixed(2)} cm\\n` +";
    script += "      `Heading: ${(data.theta * 180 / Math.PI).toFixed(1)}°`;";
    script += "    updateTrajectory(data.x, data.y, data.theta);";
    script += "    drawStatusVisual({";
    script += "      vx: data.vx,";
    script += "      vy: data.vy,";
    script += "      w: data.angular";
    script += "    });";
    script += "  })";
    script += "  .catch(error => console.error('Error:', error));";
    script += "}";
    
    // 开始状态更新
    script += "function startStatusUpdate() {";
    script += "  updateStatus();";
    script += "  updateInterval = setInterval(updateStatus, 100);";
    script += "}";
    
    // 样式定义
    script += "</script>";
    script += "<style>";
    script += ".mode-selector{display:flex;gap:10px;margin-bottom:15px}";
    script += ".mode-btn{flex:1}";
    script += ".mode-btn.active{background:#1d4ed8;transform:translateY(-2px)}";
    script += ".trajectory-controls{display:flex;justify-content:space-between;margin-top:10px}";
    script += ".parameter-cards .control-item h3{margin-top:0;color:#4b5563;font-size:1.1em}";
    script += ".parameter-cards input[type=number]{width:120px}";
    script += "</style>";
    
    // 初始化
    script += "<script>window.onload = function() {";
    script += "  initCanvases();";
    script += "  selectMode('manual');";  // 默认选择手动模式
    script += "}</script>";
    
    return script;
}