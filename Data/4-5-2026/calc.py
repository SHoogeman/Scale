import numpy as np
import matplotlib.pyplot as plt
from scipy.ndimage import uniform_filter1d
import os 
a = -3.79435112675148e-05
b = -19.448113373468736

path = os.getcwd() 
data_path = path + fr"\Scale\Data\4-5-2026\datalog.txt"

def linear(x):
    return a*x + b



def e_s(T: float) -> float:
    """The function calculates the saturation vapor pressure of water at a given temperature T in Celsius."""
    e = 6.112 * np.exp((17.67 * T) / (T + 243.5))
    return e

def AH(T: float, RH: float) -> float:
    """The function calculates the absolute humidity in g/m3 given the temperature T in Celsius and relative humidity RH in percentage."""
    AH = (216.7 * (RH / 100) * e_s(T)) / (T + 273.15)
    return AH

data = np.loadtxt(data_path, delimiter=',', skiprows=117, max_rows=1440, dtype=str) #indexed because some rows are not data, but tests. 

Time = data[:, 0]
Pressure_output = data[:, 1].astype(float)
Temp_output = data[:, 2].astype(float)
relative_humidity_output = data[:, 3].astype(float)
Pressure_input = data[:, 4].astype(float)
Temp_input = data[:, 5].astype(float)
relative_humidity_input = data[:, 6].astype(float)
raw_weight = data[:, 7].astype(float)
weight = linear(raw_weight)
amp = data[:, 8].astype(float)

dm = weight[1:] - weight[:-1]

T_graph = np.linspace(0, len(Time), len(Time))


plt.rcParams.update({
    "font.family": "sans-serif",
    "font.sans-serif": ["Arial", "Helvetica", "DejaVu Sans"],
    "font.size": 9,
    "axes.labelsize": 10,
    "axes.linewidth": 0.8,
    "xtick.labelsize": 8,
    "ytick.labelsize": 8,
    "xtick.direction": "in",
    "ytick.direction": "in",
    "xtick.major.width": 0.8,
    "ytick.major.width": 0.8,
    "savefig.dpi": 300,
})

fig, ax1 = plt.subplots(figsize=(15, 5), constrained_layout=True) # plot of the mass 

line1 = ax1.plot(Time, weight - weight[0], label="Weight", color='blue')
ax1.tick_params(axis="x", labelrotation=45)
ax1.set_xticks(Time[::100])
ax1.set_xlabel("$t$ (minutes)")
ax1.set_ylabel("$m$ (kg)")
ax1.minorticks_on()
ax1.spines["top"].set_visible(False)
ax1.set_ylim(bottom=0)  # Set the lower limit of the y-axis to 0
ax1.set_xlim(left=0, right=len(Time))  # Set the lower limit of the x-axis to 0


ax2 = ax1.twinx()  # create a second y-axis
ax2.spines = ax1.spines
line2 = ax2.plot(Time, uniform_filter1d(AH(Temp_input, relative_humidity_input), size=15, mode="nearest"), label="Absolute Humidity", color='orange')
ax2.set_xlabel("$t$ (minutes)")
ax2.set_ylabel(r"$AH\;(\mathrm{kg\,m^{-3}})$")


lines = line1 + line2 
labels = [line.get_label() for line in lines]
ax1.legend(lines, labels, frameon=False, loc="upper right", handlelength=1.8)

plt.show()



fig, ax1 = plt.subplots(figsize=(15, 5), constrained_layout=True) # plot of the mass 

line1 = ax1.plot(Time, weight - weight[0], label="Weight", color='blue')
ax1.tick_params(axis="x", labelrotation=45)
ax1.set_xticks(Time[::100])
ax1.set_xlabel("$t$ (minutes)")
ax1.set_ylabel("$m$ (kg)")
ax1.minorticks_on()
ax1.spines["top"].set_visible(False)
ax1.set_ylim(bottom=0)  # Set the lower limit of the y-axis to 0
ax1.set_xlim(left=0, right=len(Time))  # Set the lower limit of the x-axis to 0


ax2 = ax1.twinx()  # create a second y-axis
ax2.spines = ax1.spines
line2 = ax2.plot(Time, uniform_filter1d(AH(Temp_output, relative_humidity_output), size=15, mode="nearest"), label="Absolute Humidity", color='orange')
ax2.set_xlabel("$t$ (minutes)")
ax2.set_ylabel(r"$AH\;(\mathrm{kg\,m^{-3}})$")


lines = line1 + line2 
labels = [line.get_label() for line in lines]
ax1.legend(lines, labels, frameon=False, loc="upper right", handlelength=1.8)



plt.show()



ig, ax1 = plt.subplots(figsize=(15, 5), constrained_layout=True) # plot of the mass 

line1 = ax1.plot(Time, weight - weight[0], label="Weight", color='blue')
ax1.tick_params(axis="x", labelrotation=45)
ax1.set_xticks(Time[::100])
ax1.set_xlabel("$t$ (minutes)")
ax1.set_ylabel("$m$ (kg)")
ax1.minorticks_on()
ax1.spines["top"].set_visible(False)
ax1.set_ylim(bottom=0)  # Set the lower limit of the y-axis to 0
ax1.set_xlim(left=0, right=len(Time))  # Set the lower limit of the x-axis to 0


ax2 = ax1.twinx()  # create a second y-axis
ax2.spines = ax1.spines
line2 = ax2.plot(Time, uniform_filter1d(amp, size=10, mode="nearest"), label="Ampere", color='orange')
ax2.set_xlabel("$t$ (minutes)")
ax2.set_ylabel(r"$AH\;(\mathrm{A})$")


lines = line1 + line2 
labels = [line.get_label() for line in lines]
ax1.legend(lines, labels, frameon=False, loc="upper right", handlelength=1.8)



plt.show()


# fig, axes = plt.subplots(6, 1, figsize=(8, 12))

# axes[0].plot(T_graph, Pressure1, label="Pressure 1")
# axes[0].plot(T_graph, Pressure2, label="Pressure 2")    
# axes[0].set_xlabel("Time")
# axes[0].set_ylabel("Pressure (Pa)") 
# axes[0].legend()

# axes[1].plot(T_graph, Temp1, label="Temperature 1")
# axes[1].plot(T_graph, Temp2, label="Temperature 2")
# axes[1].set_xlabel("Time")
# axes[1].set_ylabel("Temperature (°C)")
# axes[1].legend()

# axes[2].plot(T_graph, relative_humidity1, label="Relative Humidity 1")
# axes[2].plot(T_graph, relative_humidity2, label="Relative Humidity 2")
# axes[2].set_xlabel("Time")  
# axes[2].set_ylabel("Relative Humidity (%)")
# axes[2].legend()

# axes[3].plot(T_graph, weight, label="Weight")
# axes[3].set_xlabel("Time")
# axes[3].set_ylabel("Weight (kg)")
# axes[3].legend()

# axes[4].plot(T_graph, amp, label="Amperes (A)")
# axes[4].set_xlabel("Time")  
# axes[4].set_ylabel("Amperes (A)")
# axes[4].legend()

# axes[5].plot(T_graph[1:], dm, label="Change in Weight")
# axes[5].set_xlabel("Time")
# axes[5].set_ylabel("Change in Weight (kg)")
# axes[5].legend()
# plt.tight_layout()
# plt.savefig("4-5-2026.png", dpi=800)
# plt.show()