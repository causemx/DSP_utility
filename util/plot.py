from pylab import *
from scipy.signal import savgol_filter
import numpy as np
import os, sys

#data1 = plotfile("savgol_output.log")
#show()

data2 = np.loadtxt("samples.log")
output = savgol_filter(data2, 11, 2)
plot(output)

show()
