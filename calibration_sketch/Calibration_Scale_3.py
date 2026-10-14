import numpy as np 
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
Mass = np.array([0, 8.85, 25.80, 109.00, 88.95, 46 ])
Count = np.array([-398336.71, -644789.93, -1033827.81, -3099282.25, -2601952.75, -1527778.87])


Mass_x = np.linspace(min(Mass), max(Mass), 100)

def linear(x, a, b):
    return a*x + b

pop, cov = curve_fit(linear, Count, Mass)

print("Covariance matrix:\n", cov)

perr = np.sqrt(np.diag(cov))


print("a =", pop[0], "±", perr[0])
print("b =", pop[1], "±", perr[1])



def sigma(a, U_a, b, U_b, Count): #uncertainty propagation with u(Count) == zero 
    return np.sqrt((Count*(U_a))**2 + (U_b)**2)

Count = -3099282.25

print("Uncertainty in Mass for a mass of 109.00 Kg:", sigma(pop[0], perr[0], pop[1], perr[1], Count))

plt.figure()
plt.plot(Count, linear(Count, *pop), label="fit")
plt.plot(Count, Mass, "x", label="data")
##plt.plot(result, result_mass, "x", label="result")
#plt.plot(result, linear(result, *pop), "x", label="fit result")
plt.xlabel("Count")
plt.ylabel("Mass (Kg)")
plt.legend()
plt.show()
