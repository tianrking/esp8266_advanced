#include "page_sensor.h"

void SensorPage::handleSensor(ESP8266WebServer* server) {
    String html = HtmlCommon::getHeader("Sensor Monitor");
    html += HtmlCommon::getNavBar("sensor");
    
    html += "<div class='container'>";
    html += getSensorDataCard();
    html += getMotorStatusCard();
    html += getBatteryStatusCard();
    html += getIMUDataCard();
    html += "</div>";
    
    html += getSensorScript();
    html += HtmlCommon::getFooter();
    
    server->send(200, "text/html", html);
}

String SensorPage::getSensorDataCard() {
    String html = "<div class='card card-status'>";
    html += "<h2>Sensor Data</h2>";
    html += "<div class='control-item'>";
    html += "<div style='display:flex;justify-content:space-between;margin-bottom:10px'>";
    html += "<button class='btn btn-status' onclick='toggleUpdate()'>Start/Stop Update</button>";
    html += "<span id='update-status'>Stopped</span>";
    html += "</div>";
    html += "<div id='sensor-data' style='font-family:monospace'>";
    html += "<div>Temperature: <span id='temperature'>--</span>°C</div>";
    html += "<div>Humidity: <span id='humidity'>--</span>%</div>";
    html += "<div>Distance: <span id='distance'>--</span>cm</div>";
    html += "</div>";
    html += "</div></div>";
    return html;
}

String SensorPage::getMotorStatusCard() {
    String html = "<div class='card card-motor'>";
    html += "<h2>Motor Status</h2>";
    html += "<div class='control-item'>";
    html += "<canvas id='motor-canvas' width='300' height='150'></canvas>";
    html += "<div style='display:flex;justify-content:space-between;margin-top:10px'>";
    html += "<div>Motor 1: <span id='motor1-speed'>0</span> RPM</div>";
    html += "<div>Motor 2: <span id='motor2-speed'>0</span> RPM</div>";
    html += "</div></div></div>";
    return html;
}

String SensorPage::getBatteryStatusCard() {
    String html = "<div class='card card-speed'>";
    html += "<h2>Battery Status</h2>";
    html += "<div class='control-item'>";
    html += "<canvas id='battery-canvas' width='300' height='100'></canvas>";
    html += "<div style='text-align:center;margin-top:10px'>";
    html += "<div>Voltage: <span id='battery-voltage'>0.0</span>V</div>";
    html += "<div>Current: <span id='battery-current'>0.0</span>A</div>";
    html += "</div></div></div>";
    return html;
}

String SensorPage::getIMUDataCard() {
    String html = "<div class='card card-led'>";
    html += "<h2>IMU Data</h2>";
    html += "<div class='control-item'>";
    html += "<canvas id='imu-canvas' width='300' height='300'></canvas>";
    html += "<div style='display:grid;grid-template-columns:1fr 1fr 1fr;gap:10px;margin-top:10px'>";
    html += "<div>Acc X: <span id='acc-x'>0.0</span></div>";
    html += "<div>Acc Y: <span id='acc-y'>0.0</span></div>";
    html += "<div>Acc Z: <span id='acc-z'>0.0</span></div>";
    html += "<div>Gyro X: <span id='gyro-x'>0.0</span></div>";
    html += "<div>Gyro Y: <span id='gyro-y'>0.0</span></div>";
    html += "<div>Gyro Z: <span id='gyro-z'>0.0</span></div>";
    html += "</div></div></div>";
    return html;
}

String SensorPage::getSensorScript() {
    String script = "<script>";
    
    // 变量定义
    script += "let updateInterval = null;";
    script += "let motorCtx = null;";
    script += "let batteryCtx = null;";
    script += "let imuCtx = null;";
    
    // 初始化函数
    script += "function initCanvases() {";
    script += "  motorCtx = document.getElementById('motor-canvas').getContext('2d');";
    script += "  batteryCtx = document.getElementById('battery-canvas').getContext('2d');";
    script += "  imuCtx = document.getElementById('imu-canvas').getContext('2d');";
    script += "  drawMotorGraph(0, 0);";
    script += "  drawBatteryGauge(0);";
    script += "  drawIMUVisual(0, 0, 0);";
    script += "}";

    // 电机速度图表绘制
    script += "function drawMotorGraph(m1, m2) {";
    script += "  const canvas = motorCtx.canvas;";
    script += "  motorCtx.clearRect(0, 0, canvas.width, canvas.height);";
    script += "  motorCtx.fillStyle = '#f0f9ff';";
    script += "  motorCtx.fillRect(0, 0, canvas.width, canvas.height);";
    
    // 绘制网格
    script += "  motorCtx.strokeStyle = '#e2e8f0';";
    script += "  motorCtx.beginPath();";
    script += "  for(let i = 0; i <= canvas.height; i += 20) {";
    script += "    motorCtx.moveTo(0, i);";
    script += "    motorCtx.lineTo(canvas.width, i);";
    script += "  }";
    script += "  motorCtx.stroke();";
    
    // 绘制电机速度条
    script += "  const maxSpeed = 255;";
    script += "  const m1Height = (m1/maxSpeed) * (canvas.height-40);";
    script += "  const m2Height = (m2/maxSpeed) * (canvas.height-40);";
    script += "  motorCtx.fillStyle = '#6366f1';";
    script += "  motorCtx.fillRect(50, canvas.height-m1Height-20, 60, m1Height);";
    script += "  motorCtx.fillRect(190, canvas.height-m2Height-20, 60, m2Height);";
    script += "  motorCtx.fillStyle = '#1f2937';";
    script += "  motorCtx.font = '14px Arial';";
    script += "  motorCtx.fillText('M1', 70, canvas.height-5);";
    script += "  motorCtx.fillText('M2', 210, canvas.height-5);";
    script += "}";

    // 电池电量计绘制
    script += "function drawBatteryGauge(voltage) {";
    script += "  const canvas = batteryCtx.canvas;";
    script += "  batteryCtx.clearRect(0, 0, canvas.width, canvas.height);";
    script += "  const minVoltage = 10;";
    script += "  const maxVoltage = 13;";
    script += "  const percentage = Math.min(100, Math.max(0, (voltage-minVoltage)/(maxVoltage-minVoltage)*100));";
    
    // 绘制电池外形
    script += "  batteryCtx.strokeStyle = '#1f2937';";
    script += "  batteryCtx.lineWidth = 2;";
    script += "  batteryCtx.beginPath();";
    script += "  batteryCtx.roundRect(10, 20, 260, 60, 5);";
    script += "  batteryCtx.moveTo(270, 40);";
    script += "  batteryCtx.lineTo(290, 40);";
    script += "  batteryCtx.lineTo(290, 60);";
    script += "  batteryCtx.lineTo(270, 60);";
    script += "  batteryCtx.stroke();";
    
    // 绘制电量填充
    script += "  const fillWidth = 250 * (percentage/100);";
    script += "  const gradient = batteryCtx.createLinearGradient(15, 0, fillWidth+15, 0);";
    script += "  gradient.addColorStop(0, percentage > 50 ? '#22c55e' : '#f59e0b');";
    script += "  gradient.addColorStop(1, percentage > 50 ? '#10b981' : '#f97316');";
    script += "  batteryCtx.fillStyle = gradient;";
    script += "  batteryCtx.fillRect(15, 25, fillWidth, 50);";
    
    // 绘制百分比文字
    script += "  batteryCtx.fillStyle = '#1f2937';";
    script += "  batteryCtx.font = 'bold 16px Arial';";
    script += "  batteryCtx.textAlign = 'center';";
    script += "  batteryCtx.fillText(percentage.toFixed(1) + '%', canvas.width/2, canvas.height-5);";
    script += "}";

    // IMU可视化
    script += "function drawIMUVisual(pitch, roll, yaw) {";
    script += "  const canvas = imuCtx.canvas;";
    script += "  imuCtx.clearRect(0, 0, canvas.width, canvas.height);";
    script += "  const centerX = canvas.width/2;";
    script += "  const centerY = canvas.height/2;";
    script += "  const size = Math.min(canvas.width, canvas.height) - 40;";
    
    // 绘制姿态指示器背景
    script += "  imuCtx.beginPath();";
    script += "  imuCtx.arc(centerX, centerY, size/2, 0, Math.PI*2);";
    script += "  imuCtx.fillStyle = '#f0f9ff';";
    script += "  imuCtx.fill();";
    script += "  imuCtx.strokeStyle = '#e2e8f0';";
    script += "  imuCtx.lineWidth = 1;";
    script += "  imuCtx.stroke();";
    
    // 绘制pitch和roll指示线
    script += "  imuCtx.save();";
    script += "  imuCtx.translate(centerX, centerY);";
    script += "  imuCtx.rotate(roll * Math.PI/180);";
    script += "  imuCtx.strokeStyle = '#6366f1';";
    script += "  imuCtx.lineWidth = 2;";
    script += "  imuCtx.beginPath();";
    script += "  imuCtx.moveTo(-size/2, 0);";
    script += "  imuCtx.lineTo(size/2, 0);";
    script += "  imuCtx.stroke();";
    script += "  imuCtx.rotate(pitch * Math.PI/180);";
    script += "  imuCtx.strokeStyle = '#10b981';";
    script += "  imuCtx.beginPath();";
    script += "  imuCtx.moveTo(0, -size/2);";
    script += "  imuCtx.lineTo(0, size/2);";
    script += "  imuCtx.stroke();";
    script += "  imuCtx.restore();";
    
    // 绘制yaw指示器
    script += "  imuCtx.save();";
    script += "  imuCtx.translate(centerX, centerY);";
    script += "  imuCtx.rotate(yaw * Math.PI/180);";
    script += "  imuCtx.fillStyle = '#f97316';";
    script += "  imuCtx.beginPath();";
    script += "  imuCtx.moveTo(0, -size/4);";
    script += "  imuCtx.lineTo(10, 0);";
    script += "  imuCtx.lineTo(-10, 0);";
    script += "  imuCtx.closePath();";
    script += "  imuCtx.fill();";
    script += "  imuCtx.restore();";
    script += "}";

    // 数据更新
    script += "function updateSensorData() {";
    script += "  fetch('/api/sensor/data')";
    script += "  .then(response => response.json())";
    script += "  .then(data => {";
    script += "    document.getElementById('temperature').textContent = data.temperature.toFixed(1);";
    script += "    document.getElementById('humidity').textContent = data.humidity.toFixed(1);";
    script += "    document.getElementById('distance').textContent = data.distance;";
    script += "    document.getElementById('motor1-speed').textContent = data.motors.m1;";
    script += "    document.getElementById('motor2-speed').textContent = data.motors.m2;";
    script += "    document.getElementById('battery-voltage').textContent = data.battery.voltage.toFixed(1);";
    script += "    document.getElementById('battery-current').textContent = data.battery.current.toFixed(2);";
    script += "    document.getElementById('acc-x').textContent = data.imu.ax.toFixed(2);";
    script += "    document.getElementById('acc-y').textContent = data.imu.ay.toFixed(2);";
    script += "    document.getElementById('acc-z').textContent = data.imu.az.toFixed(2);";
    script += "    document.getElementById('gyro-x').textContent = data.imu.gx.toFixed(2);";
    script += "    document.getElementById('gyro-y').textContent = data.imu.gy.toFixed(2);";
    script += "    document.getElementById('gyro-z').textContent = data.imu.gz.toFixed(2);";
    
    script += "    drawMotorGraph(data.motors.m1, data.motors.m2);";
    script += "    drawBatteryGauge(data.battery.voltage);";
    script += "    const pitch = Math.atan2(data.imu.ax, Math.sqrt(data.imu.ay*data.imu.ay + data.imu.az*data.imu.az)) * 180/Math.PI;";
    script += "    const roll = Math.atan2(-data.imu.ay, -data.imu.az) * 180/Math.PI;";
    script += "    const yaw = Math.atan2(data.imu.gy, data.imu.gx) * 180/Math.PI;";
    script += "    drawIMUVisual(pitch, roll, yaw);";
    script += "  });";
    script += "}";

    // 切换自动更新
    script += "function toggleUpdate() {";
    script += "  if (updateInterval) {";
    script += "    clearInterval(updateInterval);";
    script += "    updateInterval = null;";
    script += "    document.getElementById('update-status').textContent = 'Stopped';";
    script += "  } else {";
    script += "    updateInterval = setInterval(updateSensorData, 100);";
    script += "    document.getElementById('update-status').textContent = 'Running';";
    script += "  }";
    script += "}";

    // 初始化
    script += "window.onload = function() {";
    script += "  initCanvases();";
    script += "  updateSensorData();";
    script += "  toggleUpdate();";
    script += "};";

    script += "</script>";
    return script;
}