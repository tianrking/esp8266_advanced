#include "html_common.h"
#include "style_common.h"

String HtmlCommon::getHeader(const String& title) {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>" + title + "</title>";
    html += StyleCommon::getCommonStyle();
    html += StyleCommon::getCardStyle();
    html += StyleCommon::getFormStyle();
    html += StyleCommon::getButtonStyle();
    html += "</head><body>";
    return html;
}

String HtmlCommon::getFooter() {
    return "</body></html>";
}

String HtmlCommon::getNavBar(const String& currentPage) {
    String html = "<nav>";
    html += "<a href='/' " + String(currentPage == "home" ? "class='active'" : "") + ">Home</a>";
    html += "<a href='/control' " + String(currentPage == "control" ? "class='active'" : "") + ">Control</a>";
    html += "<a href='/sensor' " + String(currentPage == "sensor" ? "class='active'" : "") + ">Sensors</a>";
    html += "<a href='/mode' " + String(currentPage == "mode" ? "class='active'" : "") + ">Modes</a>";
    html += "<a href='/log' " + String(currentPage == "log" ? "class='active'" : "") + ">Logs</a>";
    html += "<a href='/settings' " + String(currentPage == "settings" ? "class='active'" : "") + ">Settings</a>";
    html += "<a href='/advanced' " + String(currentPage == "advanced" ? "class='active'" : "") + ">Advanced</a>";
    html += "<a href='/debug' " + String(currentPage == "debug" ? "class='active'" : "") + ">Debug</a>";
    html += "</nav>";
    return html;
}

String HtmlCommon::getCommonScript() {
    String script = "<script>";
    script += "function updateStatus(text){document.getElementById('status').innerText=JSON.stringify(text,null,2)}";
    script += "function getStatus(){fetch('/api/status')";
    script += ".then(response=>response.json())";
    script += ".then(data=>updateStatus(data))";
    script += ".catch(error=>updateStatus('Error: '+error));}";
    script += "function getVersion(){fetch('/api/version')";
    script += ".then(response=>response.json())";
    script += ".then(data=>updateStatus(data))";
    script += ".catch(error=>updateStatus('Error: '+error));}";
    script += "</script>";
    return script;
}