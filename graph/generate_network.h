#ifndef GUARD_GENERATE_NETWORK_H
#define GUARD_GENERATE_NETWORK_H

#include "graph.h"

#include "math.h"

// calculate vertex index from xi, yi
int xy2v(int xi, int yi, int Nx, int Ny)
{ return yi*Nx + xi; }

// vertex index from East neighbor of vertex at (xi, yi)
int viNeighborW(int xi, int yi, int Nx, int Ny)
{ return xy2v( (xi + Nx - 1) % Nx, yi, Nx, Ny); }

int viNeighborE(int xi,int yi, int Nx, int Ny)
{ return xy2v( (xi + 1) % Nx, yi, Nx, Ny); }

int viNeighborSW(int xi, int yi, int Nx, int Ny)
{ return xy2v( xi, (yi+Ny-1)%Ny, Nx, Ny ); }

int viNeighborSE(int xi, int yi, int Nx, int Ny)
{ return xy2v( (xi+1) % Nx, (yi+Ny-1)%Ny , Nx, Ny); }



Graph generateNetwork(int Nx, int Ny, double Lx)
{
    // Ny must be even!!

    int Nv = Nx*Ny; // number of vertices

    Graph graph(Nv);

    double dx = Lx/Nx;
    double dy = std::sqrt(1 - 0.25*dx*dx);
    //double Ly = Ny*dy;

    int vi; 
    for(int xi=0; xi<Nx; ++xi) {
    for(int yi=0; yi<Ny; ++yi) {
        vi = xy2v(xi,yi,Nx,Ny);
        graph.set_position( vi, xi*dx, yi*dy);
        graph.addEdge( vi, viNeighborE(xi,yi, Nx, Ny));
        graph.addEdge( vi, viNeighborSE(xi,yi, Nx, Ny));
        graph.addEdge( vi, viNeighborSW(xi,yi, Nx, Ny));
    }}

    return graph;

}

#endif
