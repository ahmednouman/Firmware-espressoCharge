import pandas as pd
import os

def twos_complement(hexstr, bits):
    value = int(hexstr, 16)
    if value & (1 << (bits - 1)):
        value -= 1 << bits
    return value

input_csv_file = 'battery_data.csv'

df = pd.read_csv(input_csv_file)

df['Timestamp'] = pd.to_datetime(df['Timestamp'])
df['Time'] = df['Timestamp'].dt.strftime('%H:%M:%S')

df['BatteryVoltage (mV)'] = df['voltage[1]'] * 256 + df['voltage[0]']
df['RemainingCapacity (mAh)'] = df['remainingCapacity[1]'] * 256 + df['remainingCapacity[0]']
df['RemainingCapacity (Ah)'] = df['RemainingCapacity (mAh)'] / 1000 

df['FullCapacity (mAh)'] = df['fullCapacity[1]'] * 256 + df['fullCapacity[0]']
df['Temperature (C)'] = (df['temperature[1]'] * 256 + df['temperature[0]']) * 0.1
df['Temperature (C)'] = df['Temperature (C)'] - 273.15

df['BatteryCurrent (mA)'] = (df['current[1]'] * 256 + df['current[0]']).apply(lambda x: twos_complement(hex(x), 16) )

df['BatteryCurrent (A)'] = df['BatteryCurrent (mA)'].apply(lambda x: x / 1000.0)

df['TimeToEmpty (min)'] = df['timeToEmpty[1]'] * 256 + df['timeToEmpty[0]']
df['TimeToFull (min)'] = df['timeToFull[1]'] * 256 + df['timeToFull[0]']

df['TimeToEmpty (hrs)'] = df['TimeToEmpty (min)'] / 60.0
df['TimeToFull (hrs)'] = df['TimeToFull (min)'] / 60.0

df['BatteryVoltage (V)'] = df['BatteryVoltage (mV)'] / 1000.0

if 'SOC' in df.columns:
    df.rename(columns={'SOC': 'SOC (%)'}, inplace=True)

if 'SOH' in df.columns:
    df.rename(columns={'SOH': 'SOH (%)'}, inplace=True)

column_order = [
    'Time', 'BatteryVoltage (mV)', 'BatteryVoltage (V)', 'BatteryCurrent (mA)', 'BatteryCurrent (A)',
    'Temperature (C)', 'RemainingCapacity (mAh)', 'RemainingCapacity (Ah)', 'FullCapacity (mAh)',
    'TimeToEmpty (min)', 'TimeToEmpty (hrs)', 'TimeToFull (min)', 'TimeToFull (hrs)',
    'SOC (%)', 'SOH (%)'
]

if 'power_direction' in df.columns:
    column_order.append('power_direction')

if 'Unnamed: 0' in df.columns:
    df.drop(columns=['Unnamed: 0'], inplace=True)

new_df = df[column_order]

base_name = os.path.splitext(input_csv_file)[0]

new_output_csv_file = f'{base_name}_Processed.csv'
new_df.to_csv(new_output_csv_file, index=False)

print(f"New file '{new_output_csv_file}' created with processed columns in the specified order, including 'BatteryVoltage (V)' and 'SOC (%)', 'SOH (%)' if they exist in the original file.")
