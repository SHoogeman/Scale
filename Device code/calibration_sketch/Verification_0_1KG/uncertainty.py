import numpy as np
import matplotlib.pyplot as plt
import os 

a = -2.7878686513615973e-07
b = 0.03036894394514445 

path = os.getcwd() 
data_path = path + fr"\calibration_sketch\Verification_0_1KG\LOG00002.txt"


data = np.loadtxt(data_path, delimiter=',', skiprows=0, dtype=str, max_rows=None)#indexed because some rows are not data, but tests. 

Time = data[:, 0]
i = np.arange(len(Time))
Count = data[:, 1].astype(float)

def linear(x, a, b):
    return a*x + b

Mass = linear(Count, a, b)*1000

mean_mass = np.mean(Mass)
std_mass = np.std(Mass)


m_minus_dx = Mass - mean_mass

plt.figure(figsize=(20, 8))
plt.plot(Time, m_minus_dx, "x", label="data")
plt.xticks(i[::150], Time[::150], rotation=45)
plt.axhline(y=0, color='r', linestyle='--', label='Mean Mass')
plt.xlabel("Time")
plt.ylabel("Mass (Kg)")
plt.title(f"Mean: {mean_mass:.4f}, Std: {std_mass:.4f}")
plt.legend()
plt.show()
