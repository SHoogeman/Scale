import numpy as np
import matplotlib.pyplot as plt
import os 

path = os.getcwd() 
data_path = path + r"\Scale\Data\DATALOG.TXT"
save_path = path + r"\Scale\Plots"

data = np.loadtxt(data_path, delimiter=',', skiprows=0, dtype=str)

Time = data[:,0]
Pressure = data[:,1]
Temperature = data[:,2]
Humidity = data[:,3]
Mass = data[:,4].astype(float)


dt = 42
dm = Mass[1:] - Mass[0:-1]  

t = np.linspace(0, dt*len(Time), len(dm))

Mass_flow = dm/dt 

filter = np.ones(15)
Flow = np.convolve(filter, Mass_flow, "same")
Mass_filtered = np.convolve(filter, Mass, "same")

fig, ax = plt.subplots(2)
ax[0].plot(t, Flow, label="flow")
ax[1].plot(np.linspace(0, dt*len(Time), len(Time)), Mass_filtered)
#ax[0].set_labels(xlabel= r"$\Phi_m$", ylabel = )
plt.savefig(save_path + "\MassflowAndWeight", dpi=800)
plt.show()
