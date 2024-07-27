import serial
import struct
import numpy as np
import matplotlib.pyplot as plt
import csv
# Constants
serial_port = 'COM6'  
baud_rate = 115200
num_pins = 18
buffer_size = num_pins * 2 

def decode_serial_data(data):
    
    readings = struct.unpack('<' + 'H' * num_pins, data)

    return readings

def main():
    data=['A0', 'A1', 'A2', 'A3', 'A4', 'A5', 'A6', 'A7', 'A8', 'A9', 'A10', 'A11', 'A12', 'A13', 'A14', 'A15', 'A16', 'A17']
    count=0
    with serial.Serial(serial_port, baud_rate) as ser:
        while True:
        
            data = ser.read(buffer_size)
            
            if len(data) == buffer_size:
                readings = decode_serial_data(data)
                data.append(list(readings))
                count+=1
                
                if(count>=100000):
                    break
            else:
                print("Incomplete data received")
    file_path = 'output.csv'

    
    with open(file_path, 'w', newline='') as file:
        writer = csv.writer(file)
        for row in data:
            writer.writerow(row)


if __name__ == "__main__":
    main()
