import serial
import sys
import serial.tools.list_ports
from cobs import cobs
import time
import csv
from datetime import datetime

targetDescription = 'Silicon Labs CP210x USB to UART Bridge'

time_counter = 1

labels = [
    "Timestamp", "Time Counter",
    "SOC", "SOH", "remainingCapacity[0]", "remainingCapacity[1]",
    "fullCapacity[0]", "fullCapacity[1]",
    "voltage[0]", "voltage[1]",
    "current[0]", "current[1]",
    "temperature[0]", "temperature[1]",
    "timeToEmpty[0]", "timeToEmpty[1]",
    "timeToFull[0]", "timeToFull[1]",
    "power_direction"
    # "Voltage Decimal", "Current Decimal",
    # "Remaining Capacity", "Full Capacity",
    # "Time To Empty", "Time To Full"
]

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
    global time_counter  # Declare time_counter as a global variable
    
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
        print("Received DECODED:", " ".join([f"{x:02X}" for x in decoded_data]))

        # Calculate and print voltage and current as a single decimal value
        # voltage_decimal = (decoded_data[7] << 8) | decoded_data[6]
        # current_decimal = (decoded_data[9] << 8) | decoded_data[8]
        # remainingCapacity = (decoded_data[4] << 8) | decoded_data[5]
        # fullCapacity = (decoded_data[2] << 8) | decoded_data[3]
        # timeToEmpty = (decoded_data[14] << 8) | decoded_data[15]
        # timeToFull = (decoded_data[12] << 8) | decoded_data[13]

        # print(f"Voltage = {voltage_decimal}")
        # print(f"Current = {current_decimal}")
        # print(f"remainingCapacity = {remainingCapacity}")
        # print(f"fullCapacity = {fullCapacity}")
        # print(f"timeToEmpty = {timeToEmpty}")
        # print(f"timeToFull = {timeToFull}")

        timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        
        time_counter = time_counter + 2

        decoded_data_list = list(decoded_data)
        # decoded_data_list.extend([voltage_decimal, current_decimal, remainingCapacity, fullCapacity, timeToEmpty, timeToFull])

        with open('data.csv', 'a', newline='') as csvfile:
            csvwriter = csv.writer(csvfile)
            
            if csvfile.tell() == 0:
                csvwriter.writerow(labels)
            
            # Write the data
            csvwriter.writerow([timestamp, time_counter] + decoded_data_list)

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


    while True:
        action = 'get_report'

        data_bytes = bytes([0xF3, 0x05, 0x10, 0x00])  # get report
        checksum = calculate_xor_checksum(data_bytes)
        data_load = data_bytes + bytes([checksum])
        encData = cobs.encode(data_load)
        encData = encData + bytes([0x00])
        # print("DATA => ", data_load)

        # print(encData)
        for i in range(len(encData)):
            send_data_to_serial(ser, encData[i])
            # print("sent:", hex(encData[i]))

        read_serial_data(ser)

        # Wait for 2 seconds before sending the next request
        time.sleep(2)
