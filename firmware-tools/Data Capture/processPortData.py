import pandas as pd

input_csv_file = 'combined_monitor_data.csv'
df = pd.read_csv(input_csv_file)


df['Time'] = pd.to_datetime(df['Time']).dt.strftime('%H:%M:%S')


df.to_csv(input_csv_file, index=False)
