import numpy as np
import matplotlib.pyplot as plt
from sys import exit



data = np.loadtxt("strain.dat")
N = int( data.shape[0])
g = data[:N,0]
e = data[:N,1]

dg = g[1] - g[0]
dedg = np.gradient(e,dg)
ddedg = np.gradient(dedg,dg)

plt.plot(g,ddedg, color='blue')
#plt.plot(g,e, color='red')

emax = max(e)
emax = max( emax, max(dedg) )
emax = max( emax, max(ddedg) )

plt.ylim([ 1e-15, emax*2])
plt.yscale('log')
plt.xlim([ dg, g[-1] ])
plt.xscale('log')

plt.show()
