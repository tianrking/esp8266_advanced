// #include <ESP8266WiFi.h>

// const char* ssid = "KUNIU";
// const char* password = "kuniu666";
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

// const char* ssid = "KUNIU";
// const char* password = "kuniu666";
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


#include <ESP8266WiFi.h>

WiFiServer server(23); // 使用TCP端口23

String ssid = "KUNIU";  // 默认SSID
String password = "kuniu666";  // 默认密码

void handleCommand(String command, Print& output);

void setup() {
    Serial.begin(921600);
    Serial.println("Send AT commands to configure WiFi settings.");
    server.begin();
}

void connectToWiFi() {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi.");
    Serial.print("Assigned IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    static String inputBuffer = "";
    WiFiClient client = server.accept(); // 接受新的客户端连接

    if (client) {
        while (client.connected()) {
            while (client.available()) {
                char c = client.read();
                Serial.write(c);  // 直接将接收到的每个字符写入串口
                inputBuffer += c;
                
                if (c == '\n') {
                    if (inputBuffer.startsWith("AT")) {
                        handleCommand(inputBuffer, client);
                    } else {
                        client.print(inputBuffer); // 回显接收到的数据
                    }
                    inputBuffer = ""; // 重置输入缓冲区
                }
            }
        }
        client.stop(); // 关闭客户端连接
    }

    // 处理来自串口的数据
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n') {
            if (inputBuffer.startsWith("AT")) {
                handleCommand(inputBuffer, Serial);
            } else {
                Serial.print(inputBuffer);
            }
            inputBuffer = "";
        } else {
            inputBuffer += c;
        }
    }
}

void handleCommand(String command, Print& output) {
    command.trim();
    if (command.startsWith("AT+GETIP")) {
        output.println("OK: " + WiFi.localIP().toString());
    } else if (command.startsWith("AT+RST")) {
        ESP.restart();
    } else if (command.startsWith("AT+SSID=")) {
        ssid = command.substring(8);
        output.println("OK: SSID set to " + ssid);
    } else if (command.startsWith("AT+PASSWD=")) {
        password = command.substring(10);
        output.println("OK: Password set to " + password);
    } else if (command.startsWith("AT+CONNECT")) {
        connectToWiFi();
        output.println("OK: Connected to WiFi.");
    } else if (command.startsWith("AT+STATUS")) {
        String status = "Current SSID: " + WiFi.SSID() +
                        "\nCurrent IP: " + WiFi.localIP().toString() +
                        "\nConfigured SSID: " + ssid +
                        "\nConfigured Password: " + password;
        output.println("STATUS: " + status);
    } else {
        output.println("ERROR: Unknown command");
    }
}
