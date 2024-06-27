import serial
import time

def main():
    port = '/dev/ttyUSB1'  # 适用于Linux系统，Windows可能是'COMx'
    baudrate = 921600  # 与ESP8266上的串口设置相匹配
    data = "Hello ESP8266 " * 1000  # 生成要发送的数据

    with serial.Serial(port, baudrate, timeout=1) as ser:
        print("Sending data to ESP8266...")
        start_time = time.time()
        ser.write(data.encode('utf-8'))
        ser.flush()  # 确保数据完全发送
        end_time = time.time()
        print(f"Data sent in {end_time - start_time:.2f} seconds")

if __name__ == '__main__':
    main()
