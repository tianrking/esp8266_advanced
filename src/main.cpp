// #include <ESP8266WiFi.h>

// const char* ssid = "";
// const char* password = "";
// WiFiServer server(23); // 使用TCP端口23

// void setup() {
//   Serial.begin(192000); // 开启串口，波特率115200
//   WiFi.begin(ssid, password);
  
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("\nConnected to WiFi");
  
//   Serial.print("Assigned IP address: ");
//   Serial.println(WiFi.localIP());
  
//   server.begin();
// }

// void loop() {
//   WiFiClient client = server.available(); // 监听客户端连接
//   if (client) {
//     while (client.connected()) {
//       if (client.available()) {
//         size_t len = client.available();
//         uint8_t buf[len];
        
//         client.read(buf, len); // 从客户端读取数据
//         client.write(buf, len); // 将读取的数据回传给客户端

//         // 同时发送数据到串口
//         Serial.write(buf, len);
//       }
//     }
//     client.stop(); // 关闭客户端连接
//   }
// }

// #include <ESP8266WiFi.h>

// const char* ssid = "";
// const char* password = "";
// WiFiServer server(23); // 使用TCP端口23

// void setup() {
//   Serial.begin(921600); // 提高串口波特率至921600
//   WiFi.begin(ssid, password);
  
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(100); // 减少延迟，提高连接尝试的响应性
//     Serial.print(".");
//   }
//   Serial.println("\nConnected to WiFi");
  
//   Serial.print("Assigned IP address: ");
//   Serial.println(WiFi.localIP());
  
//   server.begin();
// }

// void loop() {
//   WiFiClient client = server.available(); // 监听客户端连接
//   if (client) {
//     while (client.connected()) {
//       if (client.available()) {
//         size_t len = client.available();
//         uint8_t* buf = new uint8_t[len]; // 动态分配缓冲区
        
//         client.read(buf, len); // 从客户端读取数据
//         client.write(buf, len); // 将读取的数据回传给客户端

//         // 同时发送数据到串口
//         Serial.write(buf, len);
        
//         delete[] buf; // 释放缓冲区
//       }
//     }
//     client.stop(); // 关闭客户端连接
//   }
// }
// #include <ESP8266WiFi.h>

// WiFiServer server(23);
// WiFiClient persistentClient;  // 持久的客户端对象
// String inputBuffer = "";  // 用于累积输入数据，直到遇到换行符
// String ssid = "";  // 默认SSID，替换为你的网络SSID
// String password = "";  // 默认密码，替换为你的网络密码

// void handleCommand(String command, Print& output);
// void processInput(char c, Print& output);

// void setup() {
//     Serial.begin(921600);
//     WiFi.begin(ssid.c_str(), password.c_str());
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500);
//         Serial.print(".");
//     }
//     Serial.println("\nWiFi connected");
//     Serial.print("IP address: ");
//     Serial.println(WiFi.localIP());
//     server.begin();
//     Serial.println("Server started");
// }

// void loop() {
//     // 尝试接受新的客户端连接
//     if (!persistentClient || !persistentClient.connected()) {
//         persistentClient = server.available();
//         if (persistentClient) {
//             Serial.println("Client Connected");
//         }
//     }

//     // 处理来自TCP客户端的数据
//     while (persistentClient && persistentClient.connected() && persistentClient.available() > 0) {
//         char c = persistentClient.read();
//         Serial.write(c);  // 将TCP客户端的数据写入串口
//         processInput(c, persistentClient);
//     }

//     // 处理来自串口的数据
//     while (Serial.available() > 0) {
//         char c = Serial.read();
//         if (persistentClient && persistentClient.connected()) {
//             persistentClient.write(c);  // 将串口的数据写入TCP客户端
//         }
//         processInput(c, Serial);
//     }
// }

// void processInput(char c, Print& output) {
//     if (c == '\n' || c == '\r') {
//         if (inputBuffer.startsWith("AT")) {
//             handleCommand(inputBuffer, output);
//         } else {
//             output.print(inputBuffer + "\r\n"); // 回显非命令输入
//         }
//         inputBuffer = ""; // 重置输入缓冲区
//     } else {
//         inputBuffer += c; // 累积输入直到换行
//     }
// }

// void handleCommand(String command, Print& output) {
//     command.trim();
//     if (command.startsWith("AT+GETIP")) {
//         output.println("IP: " + WiFi.localIP().toString());
//     } else if (command.startsWith("AT+SSID=")) {
//         ssid = command.substring(8);
//         WiFi.begin(ssid.c_str(), password.c_str());
//         output.println("SSID set to " + ssid);
//     } else if (command.startsWith("AT+PASSWD=")) {
//         password = command.substring(10);
//         WiFi.begin(ssid.c_str(), password.c_str());
//         output.println("Password set to " + password);
//     } else if (command.startsWith("AT+CONNECT")) {
//         WiFi.begin(ssid.c_str(), password.c_str());
//         output.println("Connecting...");
//     } else if (command.startsWith("AT+STATUS")) {
//         output.println("SSID: " + ssid);
//         output.println("Password: " + password);
//         output.println("IP: " + WiFi.localIP().toString());
//     } else if (command.startsWith("AT+RST")) {
//         ESP.restart();
//     } else {
//         output.println("ERROR: Unknown command");
//     }
// }

#include <ESP8266WiFi.h>

WiFiServer server(23);
WiFiClient persistentClient;  // 持久的客户端对象
String inputBuffer = "";  // 用于累积输入数据，直到遇到换行符

String ssid = "KUNIU";  // 替换为你的网络SSID
String password = "kuniu666";  // 替换为你的网络密码

void handleCommand(String command, Print& output);
void processInput(char c, Print& output);
void handleSerialData();
void handleClientData();
void scanNetworks(Print& output);
String encryptionType(int thisType);

void setup() {
    Serial.begin(921600);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
    Serial.println("Server started");
}

void loop() {
    if (!persistentClient || !persistentClient.connected()) {
        persistentClient = server.available();
    }

    handleClientData();
    handleSerialData();
}

void handleClientData() {
    if (persistentClient && persistentClient.connected()) {
        while (persistentClient.available() > 0) {
            char c = persistentClient.read();
            Serial.write(c);  // Echo to serial
            processInput(c, persistentClient);
        }
    }
}

void handleSerialData() {
    while (Serial.available() > 0) {
        char c = Serial.read();
        if (persistentClient && persistentClient.connected()) {
            persistentClient.write(c);  // Echo to TCP client
        }
        processInput(c, Serial);
    }
}

void processInput(char c, Print& output) {
    if (c == '\n' || c == '\r') {
        if (inputBuffer.startsWith("AT")) {
            handleCommand(inputBuffer, output);
        } else {
            output.print(inputBuffer + "\r\n"); // Echo non-command input
        }
        inputBuffer = ""; // Reset buffer
    } else {
        inputBuffer += c;
    }
}

// void handleCommand(String command, Print& output) {
//     command.trim();
//     if (command.startsWith("AT+GETIP")) {
//         output.println("IP: " + WiFi.localIP().toString());
//     } else if (command.startsWith("AT+SSID=")) {
//         ssid = command.substring(8);
//         WiFi.begin(ssid.c_str(), password.c_str());
//         output.println("SSID set to " + ssid);
//     } else if (command.startsWith("AT+PASSWD=")) {
//         password = command.substring(10);
//         WiFi.begin(ssid.c_str(), password.c_str());
//         output.println("Password set to " + password);
//     } else if (command.startsWith("AT+CONNECT")) {
//         WiFi.begin(ssid.c_str(), password.c_str());
//         output.println("Connecting...");
//     } else if (command.startsWith("AT+STATUS")) {
//         output.println("SSID: " + ssid);
//         output.println("Password: " + password);
//         output.println("IP: " + WiFi.localIP().toString());
//     } else if (command.startsWith("AT+RST")) {
//         ESP.restart();
//     } else {
//         output.println("ERROR: Unknown command");
//     }
// }


void handleCommand(String command, Print& output) {
    command.trim();
    if (command.startsWith("AT+GETIP")) {
        output.println("IP: " + WiFi.localIP().toString());
    } else if (command.startsWith("AT+SSID=")) {
        ssid = command.substring(8);
        WiFi.begin(ssid.c_str(), password.c_str());
        output.println("SSID set to " + ssid);
    } else if (command.startsWith("AT+PASSWD=")) {
        password = command.substring(10);
        WiFi.begin(ssid.c_str(), password.c_str());
        output.println("Password set to " + password);
    } else if (command.startsWith("AT+CONNECT")) {
        WiFi.begin(ssid.c_str(), password.c_str());
        output.println("Connecting...");
    } else if (command.startsWith("AT+STATUS")) {
        output.println("SSID: " + ssid);
        output.println("Password: " + password);
        output.println("IP: " + WiFi.localIP().toString());
    } else if (command.startsWith("AT+SCAN")) {
        scanNetworks(output);
    } else if (command.startsWith("AT+RST")) {
        ESP.restart();
    } else {
        output.println("ERROR: Unknown command");
    }
}

void scanNetworks(Print& output) {
    output.println("Starting network scan...");
    WiFi.disconnect();
    delay(100);

    // 启用主动扫描
    int numNetworks = WiFi.scanNetworks(false, true);
    output.println("Scan complete. Networks found: " + String(numNetworks));

    if (numNetworks == 0) {
        output.println("No networks found.");
    } else {
        for (int i = 0; i < numNetworks; i++) {
            output.print("Network " + String(i + 1) + ": ");
            output.print("SSID: " + WiFi.SSID(i));
            output.print(", BSSID: " + WiFi.BSSIDstr(i));
            output.print(", Signal: " + String(WiFi.RSSI(i)) + " dBm");
            output.print(", Channel: " + String(WiFi.channel(i)));
            output.print(", Encryption: " + encryptionType(WiFi.encryptionType(i)));
            output.println(", Hidden: " + String(WiFi.isHidden(i) ? "Yes" : "No"));
        }
    }
    WiFi.reconnect(); // Reconnect to your preferred network
}

// Helper function to decode encryption type
String encryptionType(int thisType) {
    switch (thisType) {
        case ENC_TYPE_WEP: return "WEP";
        case ENC_TYPE_TKIP: return "WPA/TKIP";
        case ENC_TYPE_CCMP: return "WPA2/CCMP";
        case ENC_TYPE_NONE: return "None";
        case ENC_TYPE_AUTO: return "Auto";
        default: return "Unknown";
    }
}
