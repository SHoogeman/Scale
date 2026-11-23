import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit


Mass = np.array([0, 870.3, 5599.2, 2319.70, 6514.6, 7918.9, 8789.2])/1000  # kg
Counts = np.array([-233493.95, -417995.28, -1433785.25, -731234.18,
                  -1631843.00, -1924863.62, -2110937.50])



x_count = np.linspace(min(Counts), max(Counts), 100)

def linear(x, a, b):
    return a*x + b

pop, cov = curve_fit(linear, Counts, Mass)

print("Covariance matrix:\n", cov)

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

fig, ax = plt.subplots(figsize=(3.5, 2.7), constrained_layout=True)
fit_counts = np.linspace(Counts.min(), Counts.max(), 200)
ax.plot(fit_counts, linear(fit_counts, *pop), color="#0072B2", linewidth=1.6,
        label="Linear fit", zorder=2)
ax.errorbar(Counts, Mass, yerr=0.001, xerr=5, fmt="o", linestyle="none",
            markersize=4.5, markerfacecolor="white", markeredgecolor="#D55E00",
            markeredgewidth=1.2, ecolor="#D55E00", elinewidth=0.8, capsize=2,
            label="Calibration points", zorder=3)
ax.set_xlabel("$N$")
ax.set_ylabel("$m$ (kg)")
ax.tick_params(which="both", top=False, right=False)
ax.minorticks_on()
ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
ax.legend(frameon=False, loc="upper right", handlelength=1.8)
plt.savefig("calibration_fit_version_3.png", dpi=600, bbox_inches="tight")
plt.show()
