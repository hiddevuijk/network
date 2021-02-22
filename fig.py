import numpy as np
import matplotlib.pyplot as plt
from sys import exit



data = np.loadtxt("strain.dat")
N = int( data.shape[0])
#N = int(N/2)
g = data[:N,0]
e = data[:N,1]

dg = g[1] - g[0]
dedg = np.gradient(e,g)
ddedg = np.gradient(dedg,g)

plt.scatter(g,ddedg, color='blue', label=r"$K = \frac{ d^2 H}{d\gamma^2}$")
#plt.scatter(g,dedg, color='magenta', label=r"$\sigma= \frac{d H}{d \gamma}$")
#plt.scatter(g,e, color='red', label=r"$H$")

emax = max(e)
#emax = max( emax, max(dedg) )
#emax = max( emax, max(ddedg) )

plt.ylim([ 1e-17, emax*2])
plt.yscale('log')
plt.xlim([ 0.5*dg, max(g)] )
plt.xscale('log')
plt.legend();

plt.xlabel(r"$\gamma$", fontsize=20)

plt.show()
