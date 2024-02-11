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
                print(f"Received data: {data}")
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
    if len(sys.argv) != 2:
        print("Usage: python script.py <action>")
        sys.exit(1)

    action = sys.argv[1]

    serial_port = find_serial_port()
    if serial_port is None:
        print(f"Device with description containing '{targetDescription}' not found.")
        sys.exit(1)

    ser = serial.Serial(serial_port, baudrate=115200, timeout=1)

    if action == 'sw_to_sink':
        data_bytes = bytes([0xF2, 0x02, 0x01, 0x00])  # swap to sink
        checksum = calculate_xor_checksum(data_bytes)
        data_load = data_bytes + bytes([checksum])
        encData = cobs.encode(data_load)
        encData =  encData + bytes([0x00])
        print("DATA => ", data_load)
    elif action == 'sw_to_source':
        data_bytes = bytes([0xF2, 0x02, 0x02, 0x00])  # swap to source
        checksum = calculate_xor_checksum(data_bytes)
        data_load = data_bytes + bytes([checksum])
        encData = cobs.encode(data_load)
        encData =  encData + bytes([0x00])
    elif action == 'reset_pd':
        data_bytes = bytes([0xF2, 0x02, 0xFF, 0x00])  # reset pd
        checksum = calculate_xor_checksum(data_bytes)
        data_load = data_bytes + bytes([checksum])
        encData = cobs.encode(data_load)
        encData =  encData + bytes([0x00])
    elif action == 'reset_fuel_gauge':
        data_bytes = bytes([0xF2, 0x05, 0xFF, 0x00])  # reset pd
        checksum = calculate_xor_checksum(data_bytes)
        data_load = data_bytes + bytes([checksum])
        encData = cobs.encode(data_load)
        encData =  encData + bytes([0x00])
        print("DATA => ", data_load)
    elif action == 'get_report':
        data_bytes = bytes([0xF3, 0x05, 0x10, 0x00])  # get report
        checksum = calculate_xor_checksum(data_bytes)
        data_load = data_bytes + bytes([checksum])
        encData = cobs.encode(data_load)
        encData =  encData + bytes([0x00])
        print("DATA => ", data_load)
    # elif action == 'get_active_pdo':
    #     data_bytes = bytes([0xF3, 0x02, 0xAA, 0x00])  
    #     checksum = calculate_xor_checksum(data_bytes)
    #     data_load = data_bytes + bytes([checksum])
    #     encData = cobs.encode(data_load)
    #     encData =  encData + bytes([0x00])
    #     print("DATA => ", data_load)
    elif action == 'get_pd_mode_and_dead_flag':
        data_bytes = bytes([0xF3, 0x02, 0x19, 0x00])  
        checksum = calculate_xor_checksum(data_bytes)
        data_load = data_bytes + bytes([checksum])
        encData = cobs.encode(data_load)
        encData =  encData + bytes([0x00])
        print("DATA => ", data_load)
    elif action == 'read_i2c':
        device_addr = 0x20
        reg_addr = 0x03
        size = 4
        data_bytes = bytes([0xF3, 0x40, device_addr, reg_addr, size])  # get i2c
        checksum = calculate_xor_checksum(data_bytes)
        data_load = data_bytes + bytes([checksum])
        encData = cobs.encode(data_load)
        encData =  encData + bytes([0x00])
        print("DATA => ", data_load)
    elif action == 'write_i2c':
        device_addr = 0x20
        data_to_write = [0x08, 0x04, 0x53, 0x57, 0x53, 0x6B]
        reg_addr = data_to_write[0]
        size = len(data_to_write)
        data_bytes = bytes([0xF2, 0x40, device_addr, reg_addr, size] + data_to_write)
        checksum = calculate_xor_checksum(data_bytes)
        data_load = data_bytes + bytes([checksum])
        encData = cobs.encode(data_load)
        encData =  encData + bytes([0x00])
        print("DATA => ", data_load)
    elif action == 'get_charge_ok':
        data_bytes = bytes([0xF3, 0x03, 0x58, 0x00])  # 
        checksum = calculate_xor_checksum(data_bytes)
        data_load = data_bytes + bytes([checksum])
        encData = cobs.encode(data_load)
        encData =  encData + bytes([0x00])
        print("DATA => ", data_load)
    elif action == 'get_fw_version':
        data_bytes = bytes([0xF3, 0x01, 0x08, 0x00])  # get mcu fw version
        checksum = calculate_xor_checksum(data_bytes)
        data_load = data_bytes + bytes([checksum])
        encData = cobs.encode(data_load)
        encData =  encData + bytes([0x00])
        print("DATA => ", data_load)

    elif action == 'debug':
            encData = 0
            print("DEBUG:")
    else:
        print("Invalid action. Available actions: sw_to_sink, sw_to_source, get_report")
        sys.exit(1)

    if(action != "debug"):
        print(encData)
        for i in range(len(encData)):
            send_data_to_serial(ser, encData[i])
            print("sent :", hex(encData[i]))
        # if(i == 0):
        #     time.sleep(1)
        # else:
        #     time.sleep(.01)
    # send_data_to_serial(ser, data_bytes)
    # time.sleep(0.5)

    if action == 'get_report':
        read_serial_data(ser)
    # if action == 'get_active_pdo':
    #     read_serial_data(ser)
    if action == 'get_pd_mode_and_dead_flag':
        read_serial_data(ser)
    if action == 'read_i2c':
        read_serial_data(ser)
    if action == 'get_fw_version':
        read_serial_data(ser)
    if action == 'get_charge_ok':
        read_serial_data(ser)
    if action == 'debug':
        read_debug(ser)

    ser.close()
