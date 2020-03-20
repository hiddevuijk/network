import numpy as np
import matplotlib.pyplot as plt
from sys import exit

m = np.loadtxt("m.dat")
positions = np.loadtxt("xy.dat")

N = 10
for node_from, node in enumerate(m):
    xfrom = positions[node_from,0]
    yfrom = positions[node_from,1]
    for node_to in node:
        node_to = int(node_to)
        if node_to == -1: continue
        xto = positions[node_to,0] 
        yto = positions[node_to,1] 

        plt.plot([xfrom, xto], [yfrom, yto])
plt.show()

       


