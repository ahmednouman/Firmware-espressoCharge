import subprocess
import time

def run_flash_fuel_gauge():
    print("Running flash_fuel_gauge...")
    subprocess.run(["python", "flash_fuel_gauge.py"])
    print("flash_fuel_gauge completed.")

def run_flash_pd():
    print("Waiting for 3 seconds...")
    time.sleep(3)
    print("Running flash_pd...")
    subprocess.run(["python", "flash_pd.py"])
    print("flash_pd completed.")

if __name__ == "__main__":
    run_flash_fuel_gauge()
    run_flash_pd()
