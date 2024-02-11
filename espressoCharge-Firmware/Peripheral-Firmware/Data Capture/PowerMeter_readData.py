import serial
import csv
import time

ser = serial.Serial('COM20', 115200)

with open('monitor_data.csv', 'w', newline='') as csvfile:
    fieldnames = ['Time', 'Time Counter', 'Voltage (V)', 'Current (mA)', 'Current (A)', 'Wattage (W)']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
    writer.writeheader()

    try:
        current_time = time.time()
        time_counter = 1

        while True:
            line = ser.readline().decode().strip()
            values = line.split(', ')

            data_dict = {}
            for value in values:
                parts = value.split(':', 1)
                if len(parts) == 2:
                    label, data = parts
                    if label == 'V':
                        fieldname = 'Voltage (V)'
                    elif label == 'C':
                        fieldname = 'Current (mA)'
                        data_dict['Current (A)'] = '{:.3f}'.format(float(data) / 1000)
                    elif label == 'W':
                        fieldname = 'Wattage (W)'
                    else:
                        fieldname = label
                    data_dict[fieldname] = data

            current_time += 2
            data_dict['Time'] = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(current_time))

            data_dict['Time Counter'] = time_counter
            time_counter += 2
            
            writer.writerow(data_dict)

            print(f"{data_dict['Time']}: Time Counter={data_dict['Time Counter']}, Voltage(V)={data_dict.get('Voltage (V)', 'N/A')}, Current(mA)={data_dict.get('Current (mA)', 'N/A')}, Current(A)={data_dict.get('Current (A)', 'N/A')}, Wattage(W)={data_dict.get('Wattage (W)', 'N/A')}")

    except KeyboardInterrupt:
        ser.close()
        print("Serial connection closed.")
