import serial
import time
import sys
import serial.tools.list_ports
from cobs import cobs
import struct

data_comms = {
    "pd_read_mode": {
        "dev_addr": 0x20,
        "reg_addr": 0x03,
        "length": 4
    },
    "pd_dead_battery_flag": {
        "dev_addr": 0x20,
        "reg_addr": 0x26,
        "length": 6
    },
    "charger_read_device_id": {
        "dev_addr": 0x6B,
        "reg_addr": 0x2F,
        "length": 1
    },
}

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

def process_logs(dec_data, action, argu):
    if action == "get_fw_version":
        major_version = dec_data[-2]
        minor_version = dec_data[-3]
        patch_version = dec_data[-4]

        formatted_version = f"{major_version}.{minor_version}.{patch_version}"

        
        if formatted_version == "1.4.0":
            version_color = "\033[92m"  
        else:
            version_color = "\033[91m"  

        print("Firmware Version:", f"{version_color}{formatted_version}\033[0m")  
    elif action == "read_i2c":
        if argu == "pd_read_mode":
            pd_mode = ''.join([chr(byte) for byte in dec_data[1:4]])
            if pd_mode == "APP":
                pd_mode_color = "\033[92m"  
            else:
                pd_mode_color = "\033[91m"  
            print("PD Mode:", f"{pd_mode_color}{pd_mode}\033[0m")  
        elif argu == "pd_dead_battery_flag":
            flag_val = (dec_data[5] >> 6) & 0x03
            flag_color = "\033[92m" if flag_val == 1 else "\033[91m"
            print("Dead Battery Flag is:", f"{flag_color}OFF\033[0m" if flag_val == 1 else f"\033[91mON\033[0m")
        elif argu == "charger_read_device_id":
            charger_device_id = hex(dec_data[2])
            device_id_color = "\033[92m" if charger_device_id == "0xd6" else "\033[91m"
            print("Charger Device ID:", f"{device_id_color}{charger_device_id}\033[0m")  
    elif action == "get_report":
        soc = dec_data[0]
        soc_color = "\033[92m" if 0 <= soc <= 100 else "\033[91m"

        soh = dec_data[1]
        soh_color = "\033[92m" if 80 <= soh <= 100 else "\033[91m"

        voltage = ((dec_data[7] << 8) | dec_data[6]) / 1000.0
        voltage_color = "\033[92m" if 13 <= voltage <= 21.2 else "\033[91m"

        print("Battery State of Charge: {}{}%{}\033[0m".format(soc_color, soc, soc_color))  
        print("Battery State of Health: {}{}%{}\033[0m".format(soh_color, soh, soh_color))  
        print("Battery Voltage: {}{:.2f} V{}\033[0m".format(voltage_color, voltage, voltage_color)) 

        board_temperature = ((dec_data[11] << 8) | dec_data[10]) * 0.1 - 273.15
        temperature_color = "\033[92m" if 0 <= board_temperature <= 60 else "\033[91m"
        print("Board Temperature: {}{:.2f}{}\033[0m".format(temperature_color, board_temperature, temperature_color))
        
        battery_current_signed = struct.unpack('<h', bytes(dec_data[8:10]))[0]
        current_color = "\033[92m" if -4000 <= battery_current_signed <= 2500 else "\033[91m"
        print("Battery Current: {}{} mA{}\033[0m".format(current_color, battery_current_signed, current_color))  

        battery_remaining_capacity = (dec_data[3] << 8) | dec_data[2]
        remaining_capacity_color = "\033[92m" if 0 <= battery_remaining_capacity <= 6400 else "\033[91m"
        print("Battery Remaining Capacity: {}{} mAh{}\033[0m".format(remaining_capacity_color, battery_remaining_capacity, remaining_capacity_color))  

        battery_full_capacity = (dec_data[5] << 8) | dec_data[4]
        capacity_color = "\033[92m" if 5000 <= battery_full_capacity <= 6400 else "\033[91m"
        print("Battery Full Capacity: {}{} mAh{}\033[0m".format(capacity_color, battery_full_capacity, capacity_color))  

        device_mode = "Source" if dec_data[-2] == 1 else "Sink"
        device_mode_color = "\033[94m" if device_mode == "Source" else "\033[93m"
        print(f"Device is in {device_mode_color}{device_mode} Mode\033[0m") 

def read_serial_data(ser, action, argu):
    try:
        received_bytes = 0
        data_list = []
        while True:
            data = ser.read(1)
            if data:
                received_bytes += 1
                if data == bytes([0x00]):
                    break
                data_list.append(data)

        data_bytes_list = bytes([int(i.hex(), 16) for i in data_list])
        data_list.append(bytes([0x00]))
        decoded_data = cobs.decode(data_bytes_list)
        process_logs(decoded_data, action, argu)

    except serial.SerialException as e:
        print(f"Error: {e}")
    except KeyboardInterrupt:
        print("Serial data reading stopped by the user.")

def send_data_to_serial(ser, data):
    try:
        ser.write([data])

    except serial.SerialException as e:
        print(f"Error: {e}")

def i2c_read_call(arg_call):
    device_addr = data_comms[arg_call]["dev_addr"]
    reg_addr = data_comms[arg_call]["reg_addr"]
    size = data_comms[arg_call]["length"]
    data_buf = bytes([0xF3, 0x40, device_addr, reg_addr, size])

    checksum = calculate_xor_checksum(data_buf)
    data_load = data_buf + bytes([checksum])
    enc_data = cobs.encode(data_load)
    enc_data = enc_data + bytes([0x00])
    for i in range(len(enc_data)):
        send_data_to_serial(ser, enc_data[i])
    return data_buf

def generate_enc_data(action):
    if action == 'sw_to_sink':
        data_bytes = bytes([0xF2, 0x02, 0x01, 0x00])  
    elif action == 'sw_to_source':
        data_bytes = bytes([0xF2, 0x02, 0x02, 0x00])  
    elif action == 'reset_pd':
        data_bytes = bytes([0xF2, 0x02, 0xFF, 0x00])  
    elif action == 'reset_fuel_gauge':
        data_bytes = bytes([0xF2, 0x05, 0xFF, 0x00])  
    elif action == 'get_report':
        data_bytes = bytes([0xF3, 0x05, 0x10, 0x00])  
    elif action == 'get_pd_mode_and_dead_flag':
        data_bytes = bytes([0xF3, 0x02, 0x19, 0x00])
    elif action == 'write_i2c':
        device_addr = 0x20
        data_to_write = [0x08, 0x04, 0x53, 0x57, 0x53, 0x6B]
        reg_addr = data_to_write[0]
        size = len(data_to_write)
        data_bytes = bytes([0xF2, 0x40, device_addr, reg_addr, size] + data_to_write)
    elif action == 'get_charge_ok':
        data_bytes = bytes([0xF3, 0x03, 0x58, 0x00])   
    elif action == 'get_fw_version':
        data_bytes = bytes([0xF3, 0x01, 0x08, 0x00])  
    elif action == 'debug':
        data_bytes = bytes([0x00])  
    else:
        print("Invalid action. Available actions: sw_to_sink, sw_to_source, get_report")
        sys.exit(1)

    checksum = calculate_xor_checksum(data_bytes)
    data_load = data_bytes + bytes([checksum])
    enc_data = cobs.encode(data_load)
    enc_data = enc_data + bytes([0x00])
    for i in range(len(enc_data)):
            send_data_to_serial(ser, enc_data[i])
    return enc_data

def read_chg_i2c(ser, action, argu):
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
    time.sleep(1)

    data_to_write = [0x08, 0x04, 0x49, 0x32, 0x43, 0x72]
    reg_addr = data_to_write[0]
    size = len(data_to_write)
    data_bytes = bytes([0xF2, 0x40, device_addr, reg_addr, size] + data_to_write)
    checksum = calculate_xor_checksum(data_bytes)
    data_load = data_bytes + bytes([checksum])
    encData = cobs.encode(data_load)
    encData =  encData + bytes([0x00])
    for i in range(len(encData)):
        send_data_to_serial(ser, encData[i])

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
    data_bytes = bytes([0xF3, 0x40, device_addr, reg_addr, size])
    checksum = calculate_xor_checksum(data_bytes)
    data_load = data_bytes + bytes([checksum])
    encData = cobs.encode(data_load)
    encData =  encData + bytes([0x00])
    for i in range(len(encData)):
        send_data_to_serial(ser, encData[i])

    read_serial_data(ser, action, argu)

    ser.close()

header_message = """
*************************************************************
* espressoCharge Production End-of-Line Testing Script V1.0 *
*************************************************************
"""

print(header_message)

targetDescription = 'Silicon Labs CP210x USB to UART Bridge'

serial_port = find_serial_port()
if serial_port is None:
    print(f"Device with description containing '{targetDescription}' not found.")
    sys.exit(1)

ser = serial.Serial(serial_port, baudrate=115200, timeout=1)

test_sequence = [
    ('get_fw_version', 'Get Firmware Version', ''),
    ('read_i2c', 'Read PD Mode', 'pd_read_mode'),
    ('read_i2c', 'Read PD Dead Battery Flag', 'pd_dead_battery_flag'),
    ('read_i2c', 'Read Charger Device ID', 'charger_read_device_id'),
    ('get_report', 'Get Battery Report', ''),
]

for action, action_description, argu in test_sequence:
    print(f"\n[TEST] {action_description} ")

    if action == 'get_report':
        serial_port = find_serial_port()
        if serial_port is None:
            print(f"Device with description containing '{targetDescription}' not found.")
            sys.exit(1)

        ser = serial.Serial(serial_port, baudrate=115200, timeout=1)
        generate_enc_data(action)
        read_serial_data(ser, action, argu)
    elif action == 'get_pd_mode_and_dead_flag':
        read_serial_data(ser, action, argu)
    elif action == 'read_i2c':
        if argu != 'charger_read_device_id':
            i2c_read_call(argu)
            read_serial_data(ser, action, argu)
        elif argu == 'charger_read_device_id':
            read_chg_i2c(ser, action, argu)
    elif action == 'get_fw_version':
        generate_enc_data(action)
        read_serial_data(ser, action, argu)
    elif action == 'get_charge_ok':
        read_serial_data(ser, action, argu)

    # time.sleep(.5)

ser.close()
