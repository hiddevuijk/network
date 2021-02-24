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

emin = min(e)
emax = max(e)

emin = min( emin, min(dedg) )
emax = max( emax, max(dedg) )

emin = min( emin, min(ddedg) )
emax = max( emax, max(ddedg) )

plt.scatter(g,e, color='red', label=r"$H$")
plt.scatter(g,dedg, color='magenta', label=r"$\sigma= \frac{d H}{d \gamma}$")
plt.scatter(g,ddedg, color='blue', label=r"$K = \frac{ d^2 H}{d\gamma^2}$")

plt.yscale('log')
plt.xscale('log')
#plt.ylim([ emin/2, emax*2])
plt.ylim([1e-12,10])
plt.xlim([ 0.5*dg, max(g)] )
plt.legend();

plt.xlabel(r"$\gamma$", fontsize=20)

plt.show()
