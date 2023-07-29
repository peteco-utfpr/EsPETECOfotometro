import serial
import time
import matplotlib.pyplot as plt

mean_of = 1

data = []
x = list(range(1,2095))
ser = serial.Serial('COM5', 115200)


while(1):
    mean_data = []
    for i in range(2094): #resets data
        mean_data.append(0)

    for j in range(mean_of): #take a mean of this many readings
        while(ser.read() != b'\n'):#wait for marker
            print("a")
        data.clear()
        for i in range(2094): #read all data, byte by byte
            lower = int.from_bytes(ser.read(), "big")
            upper = int.from_bytes(ser.read(), "big")
            dat = lower | (upper << 8)
            mean_data[i] += dat


    for i in range(2094): 
        mean_data[i] /= mean_of

    plt.plot(mean_data)
    plt.ylim([0, 800])
    plt.grid()
    plt.show(block = False)
    plt.pause(0.05)
    plt.clf()
    
  
