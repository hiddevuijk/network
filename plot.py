import numpy as np
import matplotlib.pyplot as plt
from sys import exit

topology = open('topology.txt')
lines = topology.readlines()
topology.close()

Nx = 4
Ny = 4
Lx = 4
Ly = Lx*np.sqrt(3/4.)
Nv = int( lines[0].strip() )
Ne = int( lines[1].strip() )
Nb = int( lines[2].strip() )

xList = np.zeros( Nv )
yList = np.zeros( Nv )
edges = np.asarray( [ [-1]*5 ]*Ne )
bends = np.asarray( [ [-1]*8 ]*Nb )


for vi in range(Nv):
    xList[vi] = lines[4+vi].strip().split('\t')[1]
    yList[vi] = lines[4+vi].strip().split('\t')[2]


for ei in range(Ne):
    edges[ei] = lines[4+Nv+ei].strip().split('\t')

for bi in range(Nb):
    bends[bi] = lines[4+Nv+Ne+bi].strip().split('\t');



for ei in range(Ne):
    xfrom = xList[ edges[ei][0] ]
    yfrom = yList[ edges[ei][0] ]
    xto = xList[ edges[ei][1] ]
    yto = yList[ edges[ei][1] ]

    xb = edges[ei][2];
    yb = edges[ei][3];

    if( xb == 0 and yb == 0): 
        plt.plot( [ xfrom, xto], [yfrom, yto], linewidth=3, color='grey' )
    else: 
        plt.plot( [ xfrom, xto + xb*Lx], [yfrom, yto + yb*Ly], linewidth=3, color='lightgrey' )
        plt.plot( [ xto, xfrom-xb*Lx ], [yto , yfrom - yb*Ly], linewidth=3, color='lightgrey' )


plt.scatter(xList, yList,color='black', s=200, zorder=10)

d = 0.075
for i in range(Nv):
    plt.annotate( i, ( xList[i]-d, yList[i]-d ), color='red', zorder = 11 )

plt.show()
