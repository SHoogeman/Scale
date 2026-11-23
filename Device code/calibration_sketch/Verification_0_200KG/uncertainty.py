import numpy as np
import matplotlib.pyplot as plt
import os 

a = -4.0512523274988005e-05
b = -16.40137635957496

path = os.getcwd() 
data_path = path + fr"\calibration_sketch\Verification_0_200KG\LOG00002.txt"
data_path = r"C:\Users\shoog\Documents\Scale\Scale\calibration_sketch\Verification_0_200KG\LOG00002.txt"

data = np.genfromtxt(data_path, delimiter=',', skip_header=0, dtype=object, filling_values="0", autostrip=True, loose=True, invalid_raise=False)

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
plt.savefig("uncertainty_0_200KG_weird.png", dpi=600, bbox_inches="tight")
plt.show()
