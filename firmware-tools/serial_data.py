import serial
import time
import sys

def read_serial_data(ser, num_bytes=11):
    try:
        received_bytes = 0
        while received_bytes < num_bytes:
            data = ser.read(num_bytes - received_bytes)  # Read the remaining bytes needed
            if data:
                received_bytes += len(data)
                print(f"Received data: {data}")

    except serial.SerialException as e:
        print(f"Error: {e}")
    except KeyboardInterrupt:
        print("Serial data reading stopped by the user.")

def send_data_to_serial(ser, data):
    try:
        ser.write(data)

    except serial.SerialException as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <action>")
        sys.exit(1)

    action = sys.argv[1]

    serial_port = 'COM16'
    ser = serial.Serial(serial_port, baudrate=115200, timeout=1)
    print(f"Connected to {ser.name}")

    if action == 'sw_to_sink':
        data_bytes = bytes([0x02, 0xF2, 0x38, 0x01, 0x03])  # swap to sink
    elif action == 'sw_to_source':
        data_bytes = bytes([0x02, 0xF2, 0x38, 0x02, 0x03])  # swap to source
    elif action == 'get_report':
        data_bytes = bytes([0x02, 0xF3, 0x00, 0x00, 0x03])  # get report
    elif action == 'get_fw_version':
        data_bytes = bytes([0x02, 0xF3, 0x01, 0x00, 0x03])  # get mcu fw version
    else:
        print("Invalid action. Available actions: sw_to_sink, sw_to_source, get_report")
        sys.exit(1)

    send_data_to_serial(ser, data_bytes)
    time.sleep(0.5)

    if action == 'get_report':
        read_serial_data(ser, 12)
    if action == 'get_fw_version':
        read_serial_data(ser, 7)

    ser.close()
