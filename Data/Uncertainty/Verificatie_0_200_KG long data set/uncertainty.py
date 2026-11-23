from importlib.resources import path

import numpy as np
import matplotlib.pyplot as plt
import os 

a = -4.0512523274988005e-05
b = -16.40137635957496
 
dir_path = os.path.dirname(os.path.realpath(__file__))

data_path2 = dir_path + r"\LOG00141.txt"


plots_dir = os.path.abspath(os.path.join(dir_path, "..", "..", "..", "plots"))
path_figure = os.path.join(plots_dir, "uncertainty_0_200KG.png")


data2 = np.genfromtxt(data_path2, delimiter=',', skip_header=0, dtype=object, filling_values="0", autostrip=True, loose=True, invalid_raise=False)


data = data2

Time = data[:, 0]
i = np.arange(len(Time))
Count = data[:, 1].astype(float)

def linear(x, a, b):
    return a*x + b

Mass = linear(Count, a, b)

mean_mass = np.mean(Mass)
std_mass = np.std(Mass)


m_minus_dx = Mass - mean_mass

plt.figure(figsize=(20, 8))
plt.plot(Time, m_minus_dx, "x", label="data")
plt.xticks(Time[::150], rotation=45)
plt.axhline(y=0, color='r', linestyle='--', label='Mean Mass')
plt.xlabel("Time")
plt.ylabel("Mass (Kg)")
plt.title(f"Mean: {mean_mass:.4f}, Std: {std_mass:.4f}")
plt.legend()
plt.savefig(path_figure, dpi=600, bbox_inches="tight")
plt.show()
