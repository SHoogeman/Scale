import numpy as np
import matplotlib.pyplot as plt
import os 

a = -4.676308455055926e-06
b = -1.094659339857342

data_path = fr"C:\Users\shoog\Documents\Scale\Scale\calibration_sketch\Verificatio_0_10_KG\LOG00103.txt"


data = np.loadtxt(data_path, delimiter=',', skiprows=0, dtype=str, max_rows=None)#indexed because some rows are not data, but tests. 

Time = data[:, 0]
i = np.arange(len(Time))
Count = data[:, 1].astype(float)

def linear(x, a, b):
    return a*x + b

Mass = linear(Count, a, b)

mean_mass = np.mean(Mass)
std_mass = np.std(Mass)


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

fig, ax = plt.subplots(figsize=(3.5, 2.7), constrained_layout=True)
ax.plot(Time, Mass, "o", linestyle="none", markersize=4.5,
    markerfacecolor="white", markeredgecolor="#D55E00",
    markeredgewidth=1.2, label="Measurements", zorder=3)
ax.axhline(y=0, color="#0072B2", linestyle="--", linewidth=1.6,
       label="Zero mass", zorder=2)
ax.set_xticks(i[::150])
ax.set_xticklabels(Time[::150], rotation=45, ha="right")
ax.set_xlabel("Time")
ax.set_ylabel("Mass (kg)")
ax.set_title(f"Mean: {mean_mass:.4f}, Std: {std_mass:.4f}")
ax.tick_params(which="both", top=False, right=False)
ax.minorticks_on()
ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
ax.legend(frameon=False, loc="upper right", handlelength=1.8)
plt.show()
