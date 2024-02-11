import serial
import time
import sys
import serial.tools.list_ports
from cobs import cobs
from data_fuel_gauage import *


targetDescription = 'Silicon Labs CP210x USB to UART Bridge'

def count_commands(data_list):
    char_commands = {'W': 0, 'C': 0, 'X': 0}
    
    for item in data_list:
        if isinstance(item, str) and item in char_commands:
            char_commands[item] += 1
            
    return char_commands

def getPatchData():
    dataLength = len(data)
    command_num = count_commands(data)

    command_num['total_commands'] = command_num['W'] + command_num['C'] + command_num['X']

    command_num['length'] = dataLength

    print("data info = ", command_num)

    return command_num

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
        # print("Received Encoded:", data_list)
        decoded_data = cobs.decode(data_bytes_list)
        # print("Received DECODED:", " ".join([f"{x:02X}" for x in decoded_data]))
        return decoded_data

    except serial.SerialException as e:
        print(f"Error: {e}")
    except KeyboardInterrupt:
        print("Serial data reading stopped by the user.")

def send_data_to_serial(ser, data):
    try:
        ser.write([data])

    except serial.SerialException as e:
        print(f"Error: {e}")

def rcv_ok():
    data_rcv = read_serial_data(ser)
    # print("Received DECODED:", " ".join([f"{x:02X}" for x in data_rcv]))
    checksum = calculate_xor_checksum(data_rcv[:-1])
    if(checksum == data_rcv[-1]):
        if(data_rcv[0] == 0x4F and data_rcv[1] == 0x4B):
            # print("RCV: OK")
            return True
        else:
            return False

if __name__ == "__main__":
    serial_port = find_serial_port()
    if serial_port is None:
        print(f"Device with description containing '{targetDescription}' not found.")
        sys.exit(1)

    ser = serial.Serial(serial_port, baudrate=115200, timeout=1)
    print("Starting Fuel Gauge Firmware Flashing")

    data_info = getPatchData()
    command_count = data_info['total_commands']
    dataLength = data_info['length']

    data_bytes = bytes([0xF0, 0x05, (command_count >> 8) & 0xFF , command_count & 0xFF])
    checksum = calculate_xor_checksum(data_bytes)
    data_load = data_bytes + bytes([checksum])
    encData = cobs.encode(data_load)
    encData =  encData + bytes([0x00])

    # print(encData)
    for i in range(len(encData)):
        send_data_to_serial(ser, encData[i])
    #     print("sent :", hex(encData[i]))

    data_rcv = read_serial_data(ser)
    # print("Received DECODED:", " ".join([f"{x:02X}" for x in data_rcv]))
    checksum = calculate_xor_checksum(data_rcv[:-1])
    if(checksum == data_rcv[-1]):
        print("SYSTEM FLASH MODE OK")

        fuelAddress = 0x55
        cmdLen = 0
        regAddress = 0

        total_iterations = dataLength
        print_intervals = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
        next_interval_index = 0

        i = 0
        while (i < dataLength):
            command = data[i]
            i += 1

            if (command == 'W'):
                fuelAddress = data[i] >> 1
                i += 1
                cmdLen = data[i]
                i += 1

                data_slice = [0x57, fuelAddress] + data[i:i+cmdLen]

                data_bytes = bytes(data_slice)
                checksum = calculate_xor_checksum(data_bytes)
                data_load = data_bytes + bytes([checksum])
                encData = cobs.encode(data_load)
                encData =  encData + bytes([0x00])
                for n in range(len(encData)):
                    send_data_to_serial(ser, encData[n])
                current_percentage = i / total_iterations
                if current_percentage >= print_intervals[next_interval_index]:
                    print("Progress: {:.0%}".format(current_percentage))
                    next_interval_index += 1
                while(not rcv_ok()):
                    pass
                i += cmdLen

            elif (command == 'C'):
                fuelAddress = data[i] >> 1
                i += 1
                cmdLen = data[i] - 1
                i += 1
                regAddress = data[i]
                i += 1

                regContent = []
                for j in range(0, cmdLen): 
                    regContent.append(data[i+j])

                data_slice = [0x43, fuelAddress, regAddress, cmdLen] + regContent

                data_bytes = bytes(data_slice)
                checksum = calculate_xor_checksum(data_bytes)
                data_load = data_bytes + bytes([checksum])
                encData = cobs.encode(data_load)
                encData =  encData + bytes([0x00])
                for n in range(len(encData)):
                    send_data_to_serial(ser, encData[n])
                current_percentage = i / total_iterations
                if current_percentage >= print_intervals[next_interval_index]:
                    print("Progress: {:.0%}".format(current_percentage))
                    next_interval_index += 1
                while(not rcv_ok()):
                    pass
                i += cmdLen

            elif(command == 'X'):
                delay = int ( (((data[i] << 8) & 0xFF00) + data[i+1] ) / 100 )
                if(delay < 1):
                    delay = 1
                data_slice = [0x58, delay]

                data_bytes = bytes(data_slice)
                checksum = calculate_xor_checksum(data_bytes)
                data_load = data_bytes + bytes([checksum])
                encData = cobs.encode(data_load)
                encData =  encData + bytes([0x00])
                for n in range(len(encData)):
                    send_data_to_serial(ser, encData[n])
                current_percentage = i / total_iterations
                if current_percentage >= print_intervals[next_interval_index]:
                    print("Progress: {:.0%}".format(current_percentage))
                    next_interval_index += 1
                while(not rcv_ok()):
                    pass


                i += 2
            else:
                print("ERROR")
        
        if next_interval_index < len(print_intervals):
            print("Progress: 100%")
            print("Fuel Gauge has been Flashed Successfully!!")
            print("System Reset")
            print("=" * 20)

