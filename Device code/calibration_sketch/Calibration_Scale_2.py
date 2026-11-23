import numpy as np 
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
from scipy.optimize import curve_fit
Names = np.array(["Zero", "Sjoerd", "Liam", "Koen", "Richard", "Tim", "Sam", "Joaquin", "Dorian", "Emma", "Elio", "Danielle"])
Mass = np.array([0, 109.95, 91.05, 86.7, 95.15, 104.40, 104.05, 74.55, 74.55, 64.80, 58.65, 64.40])
Count = np.array([-520334.40, -3446526.00, -2907385.25, -2791163.50, -3006724.24, -3259088.00, -3248268.50, -2474684.25, -2472603.75, -2223376.25, -2058640.75, -2205856.25])

result = np.array([-3432588.00])
result_mass = np.array([110.60])

Mass_x = np.linspace(min(Mass), max(Mass), 100)

def linear(x, a, b):
    return a*x + b

pop, cov = curve_fit(linear, Count, Mass)

perr = np.sqrt(np.diag(cov))

print("a =", pop[0], "±", perr[0])
print("b =", pop[1], "±", perr[1])

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

fig, ax = plt.subplots(figsize=(5.5, 3.5), constrained_layout=True)
fit_counts = np.linspace(Count.min(), Count.max(), 200)
ax.plot(fit_counts, linear(fit_counts, *pop), color="#0072B2", linewidth=1.6,
        label="Linear fit", zorder=2)
colors = plt.cm.tab20(np.linspace(0, 1, len(Names)))
for name, count, mass, color in zip(Names, Count, Mass, colors):
    ax.errorbar(count, mass, yerr=5, xerr=0.1, fmt="o", linestyle="none",
        markersize=4.5, markerfacecolor="white", markeredgecolor=color,
        markeredgewidth=1.2, ecolor=color, elinewidth=0.8, capsize=2,
        zorder=3)
ax.set_xlabel("$N$")
ax.set_ylabel("$m$ (kg)")
ax.tick_params(which="both", top=False, right=False)
ax.minorticks_on()
ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
fit_handle = Line2D([], [], color="#0072B2", linewidth=1.6, label="y = ax + b")
name_handles = [Line2D([], [], marker="o", linestyle="none", markersize=4.5,
              markerfacecolor="white", markeredgecolor=color,
              markeredgewidth=1.2, label=name)
          for name, color in zip(Names, colors)]
fit_legend = ax.legend(handles=[fit_handle], frameon=False,
                      loc="upper left", bbox_to_anchor=(1.02, 0.3),
                      fontsize=8, handlelength=1.2)
ax.add_artist(fit_legend)
ax.legend(handles=name_handles, title="Calibration points",
         frameon=False, loc="upper left", bbox_to_anchor=(1.02, 1),
         fontsize=8, title_fontsize=8, handlelength=1.2, borderaxespad=0.0)
plt.savefig("calibration_fit_version_2.png", dpi=600, bbox_inches="tight")
plt.show()

