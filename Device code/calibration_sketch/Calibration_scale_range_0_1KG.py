import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

Mass = np.array([0.0, 0.105, 0.1946, 0.3260, 0.4041, 0.5046, 0.6126, 0.6942, 0.8035, 0.9438])
Counts = np.array([104235.50, -259304.87, -588020.75, -1062829.00, -1342492.50, -1702532.75, -2088977.00, -2381685.25, -2772831.75, -3274688.00])
#used mettler PE 6000 as a reference scale for calibration


x_count = np.linspace(min(Counts), max(Counts), 100)

def linear(x, a, b):
    return a*x + b

pop, cov = curve_fit(linear, Counts, Mass)

print("Covariance matrix:\n", cov)

perr = np.sqrt(np.diag(cov))


print("a =", pop[0], "±", perr[0])
print("b =", pop[1], "±", perr[1])


plt.figure()
plt.plot(x_count, linear(x_count, *pop), label=f"fit (a={pop[0]:.2e}, b={pop[1]:.2f})")
plt.plot(Counts, Mass, "x", label="data")
##plt.plot(result, result_mass, "x", label="result")
#plt.plot(result, linear(result, *pop), "x", label="fit result")
plt.xlabel("Count")
plt.ylabel("Mass (Kg)")
plt.legend()
plt.show()
