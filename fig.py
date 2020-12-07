import numpy as np
import matplotlib.pyplot as plt
from sys import exit


data = np.loadtxt("strain.dat")
N = int( data.shape[0])
g = data[:N,0]
e = data[:N,1]


emax = max(e)
plt.ylim([ 1e-27, emax*1.1])
plt.yscale('log')

plt.plot(g,e)
plt.show()
