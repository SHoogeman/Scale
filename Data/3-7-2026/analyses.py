import numpy as np 
from lib.Formulas import Formula
import matplotlib.pyplot as plt

data = np.genfromtxt(r"C:\Users\shoog\Documents\Scale\Scale\Data\3-7-2026\DATALOG.txt", delimiter=',', skip_header=100, dtype=object, filling_values="0", autostrip=True, loose=True, invalid_raise=False)

F = Formula()

i_max_plot_1 = 3500
N = i_max_plot_1 + 300
N_max = N + 1440 * 2
Time = data[:i_max_plot_1, 0]
P_input = data[:i_max_plot_1, 1]
T_input = data[:i_max_plot_1, 2]
RH_input = data[:i_max_plot_1, 3]
P_ambient = data[:i_max_plot_1, 4]
T_ambient = data[:i_max_plot_1, 5]
RH_ambient = data[:i_max_plot_1, 6]
P_output = data[:i_max_plot_1, 7]
T_output = data[:i_max_plot_1, 8]
RH_output = data[:i_max_plot_1, 9]
Air_vel_output = data[:i_max_plot_1, 10]
Air_vel_input = data[:i_max_plot_1, 11]
current = data[:i_max_plot_1, 13]
Mass = F.Count_to_mass_call_3(data[:i_max_plot_1, 12].astype(float)) #- F.Count_to_mass_call_3(data[:, 12].astype(float))[0]
Flow_from_mass = np.diff(Mass[:i_max_plot_1])  # kg/per minute 
Mass_from_flow = np.concatenate(([Mass[0]], Mass[0] + np.cumsum(Flow_from_mass)))
time_index = np.arange(len(Time))

Mass_constant = F.Count_to_mass_call_3(data[N:N_max, 12].astype(float))
Time_constant = data[N:N_max, 0]
Current_constant = data[N:N_max, 13]
T_input_constant = data[N:N_max, 2].astype(float)
T_output_constant = data[N:N_max, 8].astype(float)
T_ambient_constant = data[N:N_max, 5].astype(float)
P_input_constant = data[N:N_max, 1].astype(float)
P_output_constant = data[N:N_max, 7].astype(float)
P_ambient_constant = data[N:N_max, 4].astype(float)
RH_input_constant = data[N:N_max, 3].astype(float)
RH_output_constant = data[N:N_max, 9].astype(float)
RH_ambient_constant = data[N:N_max, 6].astype(float)
Air_vel_output_constant = data[N:N_max, 10].astype(float)
Air_vel_input_constant = data[N:N_max, 11].astype(float)

A_in = 0.332*0.429
A_out = 0.231*0.123

Flow = F.Flow(A_in=A_in, A_out=A_out, V_out=Air_vel_output.astype(float), V_in=Air_vel_input.astype(float), T_out=T_output.astype(float), T_in=T_input.astype(float), RH_out=RH_output.astype(float), RH_in=RH_input.astype(float))
Flow_average = F.running_average(Flow)

Mass_from_function_flow = np.concatenate(([Mass[0]], Mass[0] + np.cumsum(Flow[1:])))


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

fig, ax = plt.subplots(figsize=(15, 5), constrained_layout=True) # plot of the mass 
ax.scatter(Time, Mass, color="#0072B2", linewidth=1.6,
        label="Linear fit", zorder=2)
ax.plot(time_index, Mass_from_function_flow, color="#D55E00", linewidth=1.6,
        label="Mass recreated equation ... (kg)", zorder=2)
ax.set_xticks(Time[::300])
ax.tick_params(axis="x", labelrotation=45)
ax.set_xlabel("$t$ (minutes)")
ax.set_ylabel("$m$ (kg)")
ax.tick_params(which="both", top=False, right=False)
ax.minorticks_on()
ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
ax.legend(frameon=False, loc="upper right", handlelength=1.8)
plt.savefig("Flow_compared to parameters.png", dpi=600, bbox_inches="tight")
plt.show()


fig, ax = plt.subplots(figsize=(15, 5), constrained_layout=True) #plot of the flow 
ax.plot(Time[1:], Flow_average, color="#D55E00", linewidth=1.2, alpha=0.8, label="Flow average from measured parameters")
ax.plot(Time[1:], Flow_from_mass, color="#009E73", linewidth=1.2, alpha=0.8, label="Flow derived from measured mass")
ax.tick_params(axis="x", labelrotation=45)
ax.set_xticks(Time[::300])
ax.set_xlabel("$t$ (minutes)")
ax.set_ylabel("$Q$ (kg/minute)")
ax.tick_params(which="both", top=False, right=False)
ax.minorticks_on()
ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
ax.legend(frameon=False, loc="upper right", handlelength=1.8)
plt.savefig("flow plot.png", dpi=600, bbox_inches="tight")
plt.show()

fig, ax = plt.subplots(figsize=(15, 5), constrained_layout=True) #plot of the flow 
mean_mass = np.mean(Mass_constant)
std_mass = np.std(Mass_constant)
ax.scatter(Time_constant, Mass_constant, color="#D55E00", linewidth=1.2, alpha=0.8,
           label=f"Mass constant (mean = {mean_mass:.3f}, std = {std_mass:.3f})")
ax.tick_params(axis="x", labelrotation=45)
ax.set_xticks(Time_constant[::250])
ax.set_xlabel("$t$ (minutes)")
ax.set_ylabel("$Q$ (kg/minute)")
ax.tick_params(which="both", top=False, right=False)
ax.minorticks_on()
ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
ax.legend(frameon=False, loc="upper right", handlelength=1.8)
plt.savefig("constant mass.png", dpi=600, bbox_inches="tight")
plt.show()


fig, ax = plt.subplots(figsize=(15, 5), constrained_layout=True) 
current_values = current.astype(float)
ax.plot(Time, current_values, color="#D55E00", linewidth=1.0, alpha=0.9,
    label="Current under constant mass regime")
ax.tick_params(axis="x", labelrotation=45)
ax.set_ylim(0, 10)
ax.set_xticks(Time[::300])
ax.set_yticks(np.arange(0, 11, 2))
ax.set_xlabel("$t$ (minutes)")
ax.set_ylabel("Current (A)")
ax.tick_params(which="both", top=False, right=False)
ax.minorticks_on()
ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
ax.legend(frameon=False, loc="upper right", handlelength=1.8)
plt.savefig("constant current 2 dagen.png", dpi=600, bbox_inches="tight")
plt.show()

fig, ax = plt.subplots(figsize=(15, 5), constrained_layout=True)
ax.plot(Time_constant, T_output_constant.astype(float), color="#D55E00", linewidth=1.2,
    label="Output temperature")
ax.plot(Time_constant, T_input_constant.astype(float), color="#0072B2", linewidth=1.2,
    label="Input temperature")
ax.plot(Time_constant, T_ambient_constant.astype(float), color="#009E73", linewidth=1.2,
    label="Ambient temperature")
ax.tick_params(axis="x", labelrotation=45)
ax.set_xticks(Time_constant[::250])
ax.set_xlabel("$t$ (minutes)")
ax.set_ylabel("Temperature")
ax.tick_params(which="both", top=False, right=False)
ax.minorticks_on()
ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
ax.legend(frameon=False, loc="upper right", handlelength=1.8)
plt.savefig("constant temperatures 2 dagen.png", dpi=600, bbox_inches="tight")
plt.show()

fig, ax = plt.subplots(figsize=(15, 5), constrained_layout=True)
ax.plot(Time_constant, P_output_constant, color="#D55E00", linewidth=1.2,
    label="Output pressure")
ax.plot(Time_constant, P_input_constant, color="#0072B2", linewidth=1.2,
    label="Input pressure")
ax.plot(Time_constant, P_ambient_constant, color="#009E73", linewidth=1.2,
    label="Ambient pressure")
ax.tick_params(axis="x", labelrotation=45)
ax.set_xticks(Time_constant[::250])
ax.set_xlabel("$t$ (minutes)")
ax.set_ylabel("Pressure")
ax.tick_params(which="both", top=False, right=False)
ax.minorticks_on()
ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
ax.legend(frameon=False, loc="upper right", handlelength=1.8)
plt.savefig("constant pressures 2 dagen.png", dpi=600, bbox_inches="tight")
plt.show()

fig, ax = plt.subplots(figsize=(15, 5), constrained_layout=True)
ax.plot(Time_constant, Air_vel_output_constant, color="#D55E00", linewidth=1.2,
    label="Output air velocity")
ax.plot(Time_constant, Air_vel_input_constant, color="#0072B2", linewidth=1.2,
    label="Input air velocity")
ax.tick_params(axis="x", labelrotation=45)
ax.set_xticks(Time_constant[::250])
ax.set_xlabel("$t$ (minutes)")
ax.set_ylabel("Air velocity")
ax.tick_params(which="both", top=False, right=False)
ax.minorticks_on()
ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
ax.legend(frameon=False, loc="upper right", handlelength=1.8)
plt.savefig("constant air velocity 2 dagen.png", dpi=600, bbox_inches="tight")
plt.show()

fig, ax = plt.subplots(figsize=(15, 5), constrained_layout=True)
ax.plot(Time_constant, RH_output_constant, color="#D55E00", linewidth=1.2,
    label="Output humidity")
ax.plot(Time_constant, RH_input_constant, color="#0072B2", linewidth=1.2,
    label="Input humidity")
ax.plot(Time_constant, RH_ambient_constant, color="#009E73", linewidth=1.2,
    label="Ambient humidity")
ax.tick_params(axis="x", labelrotation=45)
ax.set_xticks(Time_constant[::250])
ax.set_xlabel("$t$ (minutes)")
ax.set_ylabel("Relative humidity (%)")
ax.tick_params(which="both", top=False, right=False)
ax.minorticks_on()
ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
ax.legend(frameon=False, loc="upper right", handlelength=1.8)
plt.savefig("constant humidity 2 dagen.png", dpi=600, bbox_inches="tight")
plt.show()


