from pylab import *
import numpy as np
import os, sys

with open('samples.log') as f:
    values = f.read()
try:
    x = [float(x) for x in values.split('\n')]
except:
    print('some trouble..')
print(x)
