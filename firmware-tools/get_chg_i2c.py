import serial
import time
import sys
import serial.tools.list_ports
from cobs import cobs


targetDescription = 'Silicon Labs CP210x USB to UART Bridge'


def calculate_xor_checksum(data):
    checksum = 0
    for byte in data:
        checksum ^= byte
    return checksum


def find_serial_port():
    ports = list(serial.tools.list_ports.comports())
    for port in ports:
        if targetDescription in port.description:
            return port.device
    return None

def read_debug(ser):
    try:
        while True:
            line = ser.readline().decode('utf-8')
            if line:
                print("Received:", line.strip())

    except serial.SerialException as e:
        print(f"Error: {e}")
    except KeyboardInterrupt:
        print("Serial data reading stopped by the user.")


def read_serial_data(ser):
    try:
        received_bytes = 0
        data_list = []
        while True:
            data = ser.read(1)  
            if data:
                received_bytes += 1
                # print(f"Received data: {data}")
                if data == bytes([0x00]):
                    break
                data_list.append(data)

        data_bytes_list = bytes([int(i.hex(), 16) for i in data_list])
        data_list.append(bytes([0x00]))
        print("Received Encoded:", data_list)
        decoded_data = cobs.decode(data_bytes_list)
        print("Received DECODED:", " ".join([f"{x:02X}" for x in decoded_data]))

    except serial.SerialException as e:
        print(f"Error: {e}")
    except KeyboardInterrupt:
        print("Serial data reading stopped by the user.")

def send_data_to_serial(ser, data):
    try:
        ser.write([data])

    except serial.SerialException as e:
        print(f"Error: {e}")

if __name__ == "__main__":


    serial_port = find_serial_port()
    if serial_port is None:
        print(f"Device with description containing '{targetDescription}' not found.")
        sys.exit(1)

    ser = serial.Serial(serial_port, baudrate=115200, timeout=1)

    device_addr = 0x20
    data_to_write = [0x09, 0x03, 0x6b, 0x2f, 0x01]
    reg_addr = data_to_write[0]
    size = len(data_to_write)
    data_bytes = bytes([0xF2, 0x40, device_addr, reg_addr, size] + data_to_write)
    checksum = calculate_xor_checksum(data_bytes)
    data_load = data_bytes + bytes([checksum])
    encData = cobs.encode(data_load)
    encData =  encData + bytes([0x00])
    for i in range(len(encData)):
        send_data_to_serial(ser, encData[i])
        print("sent :", hex(encData[i]))
    time.sleep(1)
    
    data_to_write = [0x08, 0x04, 0x49, 0x32, 0x43, 0x72]
    reg_addr = data_to_write[0]
    size = len(data_to_write)
    data_bytes = bytes([0xF2, 0x40, device_addr, reg_addr, size] + data_to_write)
    checksum = calculate_xor_checksum(data_bytes)
    data_load = data_bytes + bytes([checksum])
    encData = cobs.encode(data_load)
    encData =  encData + bytes([0x00])
    print("DATA => ", data_load)
    for i in range(len(encData)):
        send_data_to_serial(ser, encData[i])
        print("sent :", hex(encData[i]))

    ser.close()

    time.sleep(1)

    serial_port = find_serial_port()
    if serial_port is None:
        print(f"Device with description containing '{targetDescription}' not found.")
        sys.exit(1)

    ser = serial.Serial(serial_port, baudrate=115200, timeout=1)

    device_addr = 0x20
    reg_addr = 0x09
    size = 4
    data_bytes = bytes([0xF3, 0x40, device_addr, reg_addr, size])  # get i2c
    checksum = calculate_xor_checksum(data_bytes)
    data_load = data_bytes + bytes([checksum])
    encData = cobs.encode(data_load)
    encData =  encData + bytes([0x00])
    print("DATA => ", data_load)
    for i in range(len(encData)):
        send_data_to_serial(ser, encData[i])
        print("sent :", hex(encData[i]))
    
    read_serial_data(ser)

    ser.close()
