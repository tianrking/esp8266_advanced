#include "style_common.h"

String StyleCommon::getCommonStyle() {
    String style = "<style>";
    style += "body{font-family:system-ui,-apple-system,Arial;margin:0;padding:0;background:#e0e7ff;color:#1f2937}";
    style += "nav{background:#1e40af;padding:1rem;color:white}";
    style += "nav a{color:white;text-decoration:none;padding:0.5rem 1rem;margin:0 0.5rem;border-radius:0.5rem}";
    style += "nav a:hover{background:rgba(255,255,255,0.2)}";
    style += "nav a.active{background:rgba(255,255,255,0.3)}";
    style += ".container{padding:20px}";
    style += "h2{margin:0 0 16px 0;font-size:1.5rem;font-weight:600}";
    style += "</style>";
    return style;
}

String StyleCommon::getCardStyle() {
    String style = "<style>";
    style += ".card{border-radius:12px;padding:24px;margin:15px 0;transition:all 0.3s;box-shadow:0 2px 4px rgba(0,0,0,0.04)}";
    style += ".card-status{background:linear-gradient(145deg,#fff,#f1f5f9)}";
    style += ".card-speed{background:linear-gradient(145deg,#fff,#f0fdf4)}";
    style += ".card-motor{background:linear-gradient(145deg,#fff,#f0f9ff)}";
    style += ".card-led{background:linear-gradient(145deg,#fff,#fdf4ff)}";
    style += ".card-buzzer{background:linear-gradient(145deg,#fff,#fff7ed)}";
    style += ".control-item{margin:10px 0;padding:16px;border:1px solid rgba(255,255,255,0.3);border-radius:8px;background:rgba(255,255,255,0.3)}";
    style += ".status{background:rgba(255,255,255,0.3);padding:16px;margin:10px 0;border-radius:8px;font-family:monospace}";
    style += "</style>";
    return style;
}

String StyleCommon::getFormStyle() {
    String style = "<style>";
    style += "input{width:80px;padding:8px 12px;margin:5px;border:1px solid #e2e8f0;border-radius:8px;background:white}";
    style += "input:focus{outline:none;border-color:#93c5fd;box-shadow:0 0 0 3px rgba(147,197,253,0.2)}";
    style += "select{padding:8px 12px;margin:5px;border:1px solid #e2e8f0;border-radius:8px;background:white}";
    style += "textarea{width:100%;padding:8px;border-radius:8px;border:1px solid #e2e8f0}";
    style += "</style>";
    return style;
}

String StyleCommon::getButtonStyle() {
    String style = "<style>";
    style += ".btn{color:white;border:none;padding:8px 16px;border-radius:8px;cursor:pointer;margin:5px;font-weight:500;transition:all 0.2s}";
    style += ".btn:hover{transform:translateY(-1px);filter:brightness(110%)}";
    style += ".btn-status{background:#3b82f6}";
    style += ".btn-speed{background:#10b981}";
    style += ".btn-motor{background:#6366f1}";
    style += ".btn-led{background:#8b5cf6}";
    style += ".btn-buzzer{background:#f59e0b}";
    style += "</style>";
    return style;
}