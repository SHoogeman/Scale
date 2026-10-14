import numpy as np 
from lib.Formulas import Formula
import matplotlib.pyplot as plt

data = np.loadtxt(r"C:\Users\shoog\Documents\Scale\Scale\Data\3-7-2026\DATALOG.txt", delimiter=',', skiprows=200, max_rows=230, dtype=str) 

F = Formula()

Time = data[:, 0]
P_output = data[:, 1]
T_output = data[:, 2]
RH_output = data[:, 3]
P_ambient = data[:, 4]
T_ambient = data[:, 5]
RH_ambient = data[:, 6]
P_input = data[:, 7]
T_input = data[:, 8]
RH_input = data[:, 9]
Air_vel_output = data[:, 10]
Air_vel_input = data[:, 11]
Mass = F.Count_to_mass_call_3(data[:, 12].astype(float)) - F.Count_to_mass_call_3(data[:, 12].astype(float))[0]
Flow = Mass[:-1] - Mass[1:]
current = data[:, 13]
A_in = 0.332*0.429
A_out = 0.231*0.123

Flow = F.Flow(A_in=A_in, A_out=A_out, V_out=Air_vel_output.astype(float), V_in=Air_vel_input.astype(float), T_out=T_ambient.astype(float), T_in=T_input.astype(float), RH_out=RH_ambient.astype(float), RH_in=RH_input.astype(float))
Flow_average = F.running_average(Flow)

mass_theory = np.cumsum(F.intergrate(Flow, 60))
print(mass_theory)


plt.figure(figsize=(12, 6))
plt.scatter(Time, Flow, label='Flow (kg/s)', color='blue')
plt.plot(Time[1:], Flow_average, label='Flow (kg/s) - Theory', color='red')
plt.xlabel('Time')
plt.ylabel('Flow (kg/s)')
plt.title('Flow vs Time')
plt.legend()
plt.savefig(r"C:\Users\shoog\Documents\Scale\Scale\Data\3-7-2026\Flow_vs_Time.png")
plt.show()

plt.figure(figsize=(12, 6))
plt.scatter(Time, Mass, label='Mass (kg)', color='green')
plt.plot(Time[1:], mass_theory, label='Mass (kg) - Theory', color='orange')
plt.xlabel('Time')
plt.ylabel('Mass (kg)')
plt.title('Mass vs Time')
plt.legend()
plt.savefig(r"C:\Users\shoog\Documents\Scale\Scale\Data\3-7-2026\Mass_vs_Time.png")
plt.show()
