import serial
import time
import sys
import serial.tools.list_ports
from cobs import cobs


filePath = 'PD_TPS_14SEP.bin'


targetDescription = 'Silicon Labs CP210x USB to UART Bridge'

def getPatchData():
    with open(filePath, 'rb') as f:
        patchDataString = f.read().hex()


    patchDataMap = map(''.join, zip(*[iter(patchDataString)]*2))

    patchData = []
    for item in patchDataMap:
        patchData.append(int(item, 16))

    # print("PatchData = ", patchData)
    # print("PatchLength = ", len(patchData) )
    return patchData

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
    print("Starting PD IC Firmware Flashing")

    binary_data = getPatchData()
    binary_data_len = len(binary_data)
    print("patch size = ", binary_data_len)

    data_bytes = bytes([0xF0, 0x03, (binary_data_len >> 8) & 0xFF , binary_data_len & 0xFF])
    checksum = calculate_xor_checksum(data_bytes)
    data_load = data_bytes + bytes([checksum])
    encData = cobs.encode(data_load)
    encData =  encData + bytes([0x00])


    print(encData)
    for i in range(len(encData)):
        send_data_to_serial(ser, encData[i])
        # print("sent :", hex(encData[i]))

    data_rcv = read_serial_data(ser)
    print("Received DECODED:", " ".join([f"{x:02X}" for x in data_rcv]))
    checksum = calculate_xor_checksum(data_rcv[:-1])
    if(checksum == data_rcv[-1]):
        print("SYSTEM FLASH MODE OK")
        print("wait...")
        pageIndex = 0
        data = [0]

        total_pages = binary_data_len // 64
        print_intervals = [i / 10 for i in range(1, 11)]  
        next_interval_index = 0

        while (pageIndex < (binary_data_len / 64)):
            pageAddress = ((pageIndex >> 2) & 0b01111111) + 0b10000000
            byteAddress = (pageIndex << 6) & 0b11000000
            data.clear()
            data = (binary_data[(pageIndex * 64) : ((pageIndex * 64) + 64)])
            data.insert(0, pageAddress)
            data.insert(1, byteAddress)

            # print("sending :", data)
            # print("length : ",len(data))
            current_percentage = pageIndex / (binary_data_len / 64)
            if current_percentage >= print_intervals[next_interval_index]:
                print("Progress: {:.0%}".format(current_percentage))
                next_interval_index += 1

            data_bytes = bytes(data)
            checksum = calculate_xor_checksum(data_bytes)
            data_load = data_bytes + bytes([checksum])
            encData = cobs.encode(data_load)
            encData =  encData + bytes([0x00])

            for i in range(len(encData)):
                send_data_to_serial(ser, encData[i])

            # print("="*20)
            while(not rcv_ok()):
                pass
            pageIndex += 1
        if next_interval_index < len(print_intervals):
            print("Progress: 100%")
            print("PD patch success!")
            print("PD has been flashed successfully!!")
            print("System Reset")
            print("="*20)

    