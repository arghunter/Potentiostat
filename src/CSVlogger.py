import serial
import struct
import numpy as np
import time
import csv
import sys
# Constants
read_serial_port = 'COM14'
control_serial_port = 'COM13'# Replace with your actual serial port
read_baud_rate = 115200 
control_baud_rate=9600
pwm=100
pwm_samplerate=300
num_pins = 18
buffer_size = num_pins * 2  
duration = 5
sampleRate = 10000
def decode_serial_data(data):
    
    readings = struct.unpack('<' + 'H' * num_pins, data)
  
    return readings

def main():
    file_path = 'output.csv'
    
    arr=[['A0', 'A1', 'A2', 'A3', 'A4', 'A5', 'A6', 'A7', 'A8', 'A9', 'A10', 'A11', 'A12', 'A13', 'A14', 'A15', 'A16', 'A17']]
    count=0
    pwm=100
    pwm_samplerate=300
    sampleRate = 10000
    duration = 5
    if(len(sys.argv)>=2):
        read_serial_port=sys.argv[1];
        if(len(sys.argv)>=3):
            control_serial_port=sys.argv[2];
            if(len(sys.argv)>=4):
                duration=int(sys.argv[3]);
                if(len(sys.argv)>=5):
                    sampleRate=int(sys.argv[4]);
                    if(len(sys.argv)>=6):
                        pwm=sys.argv[5];
                        if(len(sys.argv)>=7):
                            pwm_samplerate=sys.argv[6];
    with open(file_path, 'w', newline='') as file:
        writer = csv.writer(file)
        for row in arr:
            writer.writerow(row)
    with serial.Serial(control_serial_port, control_baud_rate) as ser:
     
        ser.write(("readrate "+str(sampleRate)+"\r").encode('utf-8'))
        time.sleep(0.1) 
        ser.write(("pwm "+str(pwm)+" "+str(pwm_samplerate)+"\r").encode('utf-8'))
            # ser.write(("pwm "+str(pwm)+" "+str(pwm_samplerate)+"\r\n").encode('utf-8'))
        
    maxCount=duration*sampleRate
    with serial.Serial(read_serial_port, read_baud_rate) as ser:
        ser.reset_input_buffer()
        ser.reset_output_buffer()
        while True:
            # Read the buffer size of data
            data = ser.read(buffer_size)
            
            if len(data) == buffer_size:
                readings = decode_serial_data(data)
                arr.append(list(readings))
                count+=1
                print(count)
                if(count>=maxCount):
                    break
                if(count%10000==0):
                    with open(file_path, 'a', newline='') as file:
                        writer = csv.writer(file)
                        for row in arr[1:]:
                            writer.writerow(row)
                    arr=[['A0', 'A1', 'A2', 'A3', 'A4', 'A5', 'A6', 'A7', 'A8', 'A9', 'A10', 'A11', 'A12', 'A13', 'A14', 'A15', 'A16', 'A17']]
            else:
                print("Incomplete data received")
    
      
    with open(file_path, 'a', newline='') as file:
        writer = csv.writer(file)
        for row in arr[1:]:
            writer.writerow(row)
    



if __name__ == "__main__":
    main()
