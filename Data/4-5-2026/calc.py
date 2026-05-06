import numpy as np
import matplotlib.pyplot as plt
import os 
a = -3.79435112675148e-05
b = -19.448113373468736

path = os.getcwd() 
data_path = path + fr"\Data\4-5-2026\datalog.txt"

def linear(x):
    return a*x + b

data = np.loadtxt(data_path, delimiter=',', skiprows=0, dtype=str)[117:]

Time = data[:, 0]
Pressure1 = data[:, 1].astype(float)
Temp1 = data[:, 2].astype(float)
relative_humidity1 = data[:, 3].astype(float)
Pressure2 = data[:, 4].astype(float)
Temp2 = data[:, 5].astype(float)
relative_humidity2 = data[:, 6].astype(float)
raw_weight = data[:, 7].astype(float)
weight = linear(raw_weight)
amp = data[:, 8].astype(float)

dm = weight[1:] - weight[:-1]

T_graph = np.linspace(0, len(Time), len(Time))

fig, axes = plt.subplots(6, 1, figsize=(8, 12))

axes[0].plot(T_graph, Pressure1, label="Pressure 1")
axes[0].plot(T_graph, Pressure2, label="Pressure 2")    
axes[0].set_xlabel("Time")
axes[0].set_ylabel("Pressure (Pa)") 
axes[0].legend()

axes[1].plot(T_graph, Temp1, label="Temperature 1")
axes[1].plot(T_graph, Temp2, label="Temperature 2")
axes[1].set_xlabel("Time")
axes[1].set_ylabel("Temperature (°C)")
axes[1].legend()

axes[2].plot(T_graph, relative_humidity1, label="Relative Humidity 1")
axes[2].plot(T_graph, relative_humidity2, label="Relative Humidity 2")
axes[2].set_xlabel("Time")  
axes[2].set_ylabel("Relative Humidity (%)")
axes[2].legend()

axes[3].plot(T_graph, weight, label="Weight")
axes[3].set_xlabel("Time")
axes[3].set_ylabel("Weight (kg)")
axes[3].legend()

axes[4].plot(T_graph, amp, label="Amplitude")
axes[4].set_xlabel("Time")  
axes[4].set_ylabel("Amplitude")
axes[4].legend()

axes[5].plot(T_graph[1:], dm, label="Change in Weight")
axes[5].set_xlabel("Time")
axes[5].set_ylabel("Change in Weight (kg)")
axes[5].legend()
plt.tight_layout()
plt.show()