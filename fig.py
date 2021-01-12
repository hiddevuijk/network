import numpy as np
import matplotlib.pyplot as plt
from sys import exit



data = np.loadtxt("strain.dat")
N = int( data.shape[0])
g = data[:N,0]
e = data[:N,1]

dg = g[1] - g[0]
dedg = np.gradient(e,g)
ddedg = np.gradient(dedg,g)

plt.plot(g,ddedg, color='blue', label=2)
plt.plot(g,dedg, color='magenta', label=1)
plt.plot(g,e, color='red', label=0)

emax = max(e)
emax = max( emax, max(dedg) )
emax = max( emax, max(ddedg) )

plt.ylim([ 1e-15, emax*2])
plt.yscale('log')
plt.xlim([ 10*dg, max(g)] )
plt.xscale('log')
plt.legend();

plt.show()
