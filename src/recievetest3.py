import serial
import struct

# Constants
serial_port = 'COM6'  # Replace with your actual serial port
baud_rate = 230400
num_pins = 18
buffer_size = num_pins * 2  # Each reading is 2 bytes

def decode_serial_data(data):
    # Unpack the data using struct
    readings = struct.unpack('<' + 'H' * num_pins, data)
    return readings

def main():
    with serial.Serial(serial_port, baud_rate) as ser:
        while True:
            # Read the buffer size of data
            data = ser.read(buffer_size)
            
            # Ensure we received the correct amount of data
            if len(data) == buffer_size:
                readings = decode_serial_data(data)
                print("Analog Readings:", readings)
            else:
                print("Incomplete data received")

if __name__ == "__main__":
    main()
