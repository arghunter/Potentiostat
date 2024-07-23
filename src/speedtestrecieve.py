import serial
import time
import matplotlib.pyplot as plt
import numpy as np

# Open the serial port
ser = serial.Serial('COM6', 9600, timeout=1)  


time.sleep(2)


data = [[] for _ in range(18)]

try:
    while True:
        line = ser.readline()
        if line:
            print(line)
            
            # numbers = list(map(int, line.strip('[]').split(',')))
           
            # if len(numbers) == 18:
            #     for i in range(18):
            #         data[i].append(numbers[i])
            # print(numbers)
except KeyboardInterrupt:
    print("Interrupted by user")

# Close the serial port
ser.close()

# Plot the data
colors = plt.cm.jet(np.linspace(0, 1, 18))  

for i in range(18):
    plt.plot(data[i], color=colors[i], label=f'Sensor {i+1}')

plt.xlabel('Sample')
plt.ylabel('Value')
plt.title('Serial Data Plot from 18 Sensors')
plt.legend()
plt.show()
