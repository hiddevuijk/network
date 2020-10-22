#ifndef GUARD_GENERATE_NETWORK_H
#define GUARD_GENERATE_NETWORK_H


/*
   \| \ | \ | \ |
  --4 --5 --6 --7 --
  \ | \ | \ | \ | \
  --0 --1 --2 --3--
    | \ | \ | \ | \


    NW  N
      \ |
    W - + - E
        | \
        S   SE

*/

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
        //graph.addEdge( vi, viNeighborSE(xi,yi, Nx, Ny));
        //graph.addEdge( vi, viNeighborS(xi,yi, Nx, Ny));
    }}

    // add bends / filaments
    int v0;
    int filament_index = 0; 
    int viPrev, viNext;

    
    // add W-E bends
    for(int yi =0; yi < Ny; ++yi ){
        // filament starts at vi
        vi = xy2v(0,yi,Nx,Ny); 
        graph.filaments.push_back(vi);

        for(int xi = 0; xi < Nx; ++xi ) {
            vi = xy2v(xi,yi,Nx,Ny); 
            viPrev = viNeighborW(xi,yi,Nx,Ny);
            viNext = viNeighborE(xi,yi,Nx,Ny);
            graph.addBend(vi, viPrev, viNext, viPrev, viNext, filament_index);
        } 
        filament_index += 1;
    }

        

   return graph;

}

#endif
