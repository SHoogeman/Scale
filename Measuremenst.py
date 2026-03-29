import numpy as np
import matplotlib.pyplot as plt
import os 

path = os.getcwd() 
data_path = path + r"\Scale\Data\DATALOG.TXT"
save_path = path + r"\Scale\Plots"

data = np.loadtxt(data_path, delimiter=',', skiprows=0, dtype=str)

Time = data[:,0]
Pressure = data[:,1].astype(float)
Temperature = data[:,2].astype(float)
Humidity = data[:,3].astype(float)
Mass = data[:,4].astype(float)


dt = 42
dm = Mass[1:] - Mass[0:-1]  

t = np.linspace(0, dt*len(Time), len(dm))

Mass_flow = dm/dt 

filter = np.ones(15)
Flow = np.convolve(filter, Mass_flow, "same")
Mass_filtered = np.convolve(filter, Mass, "same")

fig, ax = plt.subplots(5)
ax[0].plot(t, Flow, label="flow")
ax[1].plot(np.linspace(0, dt*len(Time), len(Time)), Mass_filtered)
ax[2].plot(np.linspace(0, dt*len(Temperature), len(Temperature)), Temperature)
ax[3].plot(np.linspace(0, dt*len(Temperature), len(Temperature)), Humidity)
ax[4].plot(np.linspace(0, dt*len(Temperature), len(Temperature)), Pressure)
plt.savefig(save_path + "\MassflowAndWeight", dpi=800)
plt.show()

#cor_hum_Mass_flow = np.correlate(Mass_flow, Humidity, "same")

#fig, axs = plt.subplots(1)
#axs.plot(np.linspace(0, dt*len(cor_hum_Mass_flow), len(cor_hum_Mass_flow)), cor_hum_Mass_flow)
#plt.savefig(save_path + "\correlation", dpi=800)

