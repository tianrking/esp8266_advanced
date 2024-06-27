import socket
import time

def main():
    host = '192.168.0.166'  # ESP8266的IP地址
    port = 23  # 与ESP8266服务器端口相同
    iterations = 10  # 测试次数

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((host, port))
        
        message = "Hello, ESP8266! " * 2000  # 增加数据大小
        total_bytes_sent = 0
        total_bytes_received = 0
        total_time_elapsed = 0
        
        for _ in range(iterations):
            start_time = time.time()  # 开始计时
            sock.sendall(message.encode('utf-8'))  # 发送数据
            total_bytes_sent += len(message)

            # 接收返回的数据
            response = b""
            while len(response) < len(message.encode('utf-8')):
                part = sock.recv(4096)
                response += part
                if not part:
                    break
            
            end_time = time.time()  # 结束计时
            total_bytes_received += len(response)
            total_time_elapsed += (end_time - start_time)

        # 计算并打印传输速率
        average_rate = total_bytes_received / total_time_elapsed  # bytes per second
        print(f"Total bytes sent: {total_bytes_sent} bytes")
        print(f"Total bytes received: {total_bytes_received} bytes")
        print(f"Total elapsed time: {total_time_elapsed:.2f} seconds")
        print(f"Average data transfer rate: {average_rate:.2f} bytes/second")

if __name__ == '__main__':
    main()
