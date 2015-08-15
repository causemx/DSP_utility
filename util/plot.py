from pylab import *
from scipy.signal import savgol_filter
import numpy as np
import os, sys

data = np.loadtxt("samples.log")
output = savgol_filter(data, 11, 2)
plot(output)
show()
