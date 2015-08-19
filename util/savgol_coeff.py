from pylab import *
from scipy.signal import savgol_coeffs, convolve
import numpy as np


data = np.loadtxt("samples.log")
coef = savgol_coeffs(11, 2)
ret = convolve(data, coef, 'same')
plot(ret)
show()
