import numpy as np


class Formula:
    """This class contains the formulas used in the project."""

    def __init__(self):
        self.calibration_params_3 = {
            "a": -4.0512523274988005e-05,
            "a_error": 2.265254139154693e-07,
            "b": -16.40137635957496,
            "b_error": 0.4172112277249661,
        }

    def e_s(self, T: float) -> float:
        """The function calculates the saturation vapor pressure of water at a given temperature T in Celsius."""
        e = 6.112 * np.exp((17.67 * T) / (T + 243.5))
        return e

    def AH(self, T: float, RH: float) -> float:
        """The function calculates the absolute humidity in g/m3 given the temperature T in Celsius and relative humidity RH in percentage."""
        AH = (216.7 * (RH / 100) * self.e_s(T)) / (T + 273.15)
        return AH

    def Flow(self, A_in: float, A_out: float, V_out: float, V_in: float, T_out: float, T_in: float, RH_out: float, RH_in: float) -> float:
        """Calculate net water mass flow in kg/s.

        V_in and V_out are volumetric flow rates in m3/s. Since these already
        include the cross-sectional area, multiplying them by A_in/A_out
        would count the area twice. The area arguments remain for compatibility.
        Positive values represent net inflow.
        """
        AH_out = self.AH(T_out, RH_out)
        AH_in = self.AH(T_in, RH_in)
        Flow = (V_in * AH_in - V_out * AH_out) / 1000
        return Flow

    def Count_to_mass_call_3(self, Count: float) -> float:
        """The function converts the count to mass in kg given the count and the calibration parameters a and b."""
        Mass = self.calibration_params_3["a"] * Count + self.calibration_params_3["b"]
        return Mass

    def running_average(self, x: np.ndarray) -> np.ndarray:
        average = ((x[:-1] +x[1:])/2)
        return average
    
    def intergrate(self, x: np.ndarray, t: float) -> np.ndarray:
        """The function integrates the array x over time t using the trapezoidal rule."""
        res = []
        for i in range(len(x)-1):
            midpoint = (x[i] + x[i+1]) / 2
            integral = midpoint * t + x[i-1] if i > 0 else midpoint * t
            res.append(integral)
        return np.array(res)
