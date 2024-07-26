import serial
import struct
import numpy as np
import matplotlib.pyplot as plt
# Constants
serial_port = 'COM6'  # Replace with your actual serial port
baud_rate = 115200
num_pins = 18
buffer_size = num_pins * 2  # Each reading is 2 bytes

def decode_serial_data(data):
    # Unpack the data using struct
    readings = struct.unpack('<' + 'H' * num_pins, data)
    return readings[1] 
    return readings

def main():
    arr=np.zeros(100000)
    count=0
    with serial.Serial(serial_port, baud_rate) as ser:
        while True:
            # Read the buffer size of data
            data = ser.read(buffer_size)
            
            # Ensure we received the correct amount of data
            if len(data) == buffer_size:
                readings = decode_serial_data(data)
                arr[count]=readings
                count+=1
                print(readings)
                if(count>=len(arr)):
                    break
            else:
                print("Incomplete data received")
  
    

    plt.plot(arr)

   
    plt.xlabel('Index')
    plt.ylabel('Value')
    plt.title('Simple Line Plot')

    plt.show()

if __name__ == "__main__":
    main()
