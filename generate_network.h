#ifndef GUARD_GENERATE_NETWORK_H
#define GUARD_GENERATE_NETWORK_H

#include "network.h"


#include "boost/random.hpp"


#include <math.h>
#include <iostream>



// calculate vertex index from xi, yi
int xy2v(int xi, int yi, int Nx, int Ny)
{ return yi*Nx + xi; }

int v2x(int v, int Nx)
{   return v%Nx; }

int v2y( int v, int Nx)
{ return (v - v%Nx)/Nx; }

// vertex index from West neighbor of vertex at (xi, yi)
int viNeighborW(int xi, int yi, int Nx, int Ny)
{ return xy2v( (xi + Nx - 1) % Nx, yi, Nx, Ny); }

int viNeighborE(int xi,int yi, int Nx, int Ny)
{ return xy2v( (xi + 1) % Nx, yi, Nx, Ny); }

int viNeighborNE(int xi, int yi, int Nx, int Ny)
{ return xy2v( (xi + yi%2)%Nx, (yi+1)%Ny, Nx,Ny); }

int viNeighborNW(int xi, int yi, int Nx, int Ny)
{ return xy2v( (xi  +Nx -1 + yi%2)%Nx, (yi+1)%Ny , Nx, Ny);}

int viNeighborSE(int xi, int yi, int Nx, int Ny)
{ return xy2v( (xi + yi%2)%Nx  , (yi + Ny -1)%Ny  ,Nx, Ny); }

int viNeighborSW(int xi, int yi, int Nx, int Ny)
{ return xy2v( (xi + Nx -1 + yi%2)%Nx  , (yi+Ny-1)%Ny, Nx, Ny); }

// neighbor vertex index from vertex index
int viNeighborW(int vi, int Nx, int Ny)
{ return viNeighborW( v2x(vi,Nx), v2y(vi,Nx), Nx, Ny); }

int viNeighborE(int vi, int Nx, int Ny)
{ return viNeighborE( v2x(vi,Nx), v2y(vi,Nx), Nx, Ny); }

int viNeighborNE(int vi, int Nx, int Ny)
{ return viNeighborNE( v2x(vi,Nx), v2y(vi,Nx), Nx, Ny); }

int viNeighborNW(int vi, int Nx, int Ny)
{ return viNeighborNW( v2x(vi,Nx), v2y(vi,Nx), Nx, Ny); }

int viNeighborSE(int vi, int Nx, int Ny)
{ return viNeighborSE( v2x(vi,Nx), v2y(vi,Nx), Nx, Ny); }

int viNeighborSW(int vi, int Nx, int Ny)
{ return viNeighborSW( v2x(vi,Nx), v2y(vi,Nx), Nx, Ny); }




Network generateNetwork(int Nx, int Ny, double Lx)
{
    long int seed = 123456789;
    boost::random::mt19937 rng(seed);
    boost::random::uniform_int_distribution<int> randX(1,Nx-1);
    boost::random::uniform_int_distribution<int> randY(1,Ny-1);

    // Ny must be even!!

    int Nv = Nx*Ny; // number of vertices
    Network net(Nv);
     
    double dx = Lx/Nx;
    double dy = dx*std::sqrt(3./4.);

    int vi, viNeighbor;
    int xb, yb;
    for(int xi=0; xi<Nx; ++xi) {
    for(int yi=0; yi<Ny; ++yi) {
        vi = xy2v(xi,yi,Nx,Ny);
        net.setVertexPosition( vi, xi*dx + (yi%2)*0.5*dx, yi*dy);

        // add East edge
        viNeighbor = viNeighborE(xi,yi,Nx,Ny);
        if( xi == Nx -1 ) xb = 1;
        else xb = 0;
        yb = 0;
        
        net.addEdge( vi, viNeighbor, xb, yb, dx);

        // add South-East edge
        viNeighbor = viNeighborSE(xi,yi,Nx,Ny);
        if( xi == Nx -1 and yi % 2 == 1) xb = 1; 
        else xb = 0;
        if( yi == 0 ) yb = -1;
        else yb = 0;
        
        net.addEdge( vi, viNeighbor, xb, yb, dx);

        // add South-West edge
        viNeighbor = viNeighborSW(xi,yi,Nx,Ny);
        if( xi == 0 and yi % 2 == 0 ) xb = -1; 
        else xb = 0;

        if( yi ==0 ) yb = -1;
        else yb = 0;
        
        net.addEdge( vi, viNeighbor, xb, yb, dx);


    }}

    // add Bends
    int viPrev, viNext;

    // add W-E bends
    for(int yi=0; yi<Ny; ++yi ){
        vi = xy2v(0,yi,Nx,Ny);
        for(int xi = 0; xi < Nx; ++xi ) {
            vi = xy2v(xi,yi,Nx,Ny); 
            viPrev = viNeighborW(xi,yi,Nx,Ny);
            viNext = viNeighborE(xi,yi,Nx,Ny);
            net.addBend(vi, viPrev, viNext);
        } 
        //for(int xi=0; xi<Nx; ++xi ) {
        //    vi = xy2v(xi,yi,Nx,Ny); 
        //    viNext = viNeighborE(xi,yi,Nx,Ny);
        //    net.polymerize(vi, viNext);
        //}

    }

    // add SW-NE bends
    for(int yi=0; yi<Ny; ++yi ){
    for(int xi=0; xi<Nx; ++xi ) {
        vi = xy2v(xi,yi,Nx,Ny);
        viPrev = viNeighborSW(vi,Nx,Ny);  
        viNext = viNeighborNE(vi,Nx,Ny);  
        net.addBend(vi, viPrev, viNext);

    }}

    //polymerize SW-NE bends
    //for(int yi=0; yi<Ny; ++yi ){
    //for(int xi=0; xi<Nx; ++xi ) {
    //    vi = xy2v(xi,yi,Nx,Ny);
    //    viNext = viNeighborNE(vi,Nx,Ny);  
    //    net.polymerize(vi, viNext);
    //}}

    // add SE-NW bends
    for(int yi=0; yi<Ny; ++yi ){
    for(int xi=0; xi<Nx; ++xi ) {
        vi = xy2v(xi,yi,Nx,Ny);
        viPrev = viNeighborSE(vi,Nx,Ny);  
        viNext = viNeighborNW(vi,Nx,Ny);  
        net.addBend(vi, viPrev, viNext);

    }}
    //polymerize SE-NW bends
    //for(int yi=0; yi<Ny; ++yi ){
    //for(int xi=0; xi<Nx; ++xi ) {
    //    vi = xy2v(xi,yi,Nx,Ny);
    //    viNext = viNeighborNW(vi,Nx,Ny);  
    //    //net.polymerize(vi, viNext);
    //}}



    // cut every filament once
    //for(int yi=0; yi<Ny; ++yi) {
    //    int xi = randX(rng);
    //    int vi = xy2v(xi,yi,Nx,Ny);
    //    int viNext = viNeighborE(vi,Nx,Ny);
    //    std::cout << vi << '\t' << viNext << '\n';
    //    net.deleteEdge(vi,viNext);
    //}


   return net;

}




#endif
