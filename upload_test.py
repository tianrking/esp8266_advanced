import socket

def main():
    host = '192.168.0.166'  # ESP8266的IP地址
    port = 23  # 与ESP8266服务器端口相同

    message = "Hello, ESP8266!\n" * 100  # 重复一些数据来增加发送量

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((host, port))
        sock.sendall(message.encode('utf-8'))  # 发送数据
        print(f"Data sent: {len(message)} bytes")
        sock.close()  # 发送完数据后立即关闭连接

if __name__ == '__main__':
    main()
