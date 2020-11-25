#ifndef GUARD_NETWORK_H
#define GUARD_NETWORK_H

#include "vec2.h"
#include "graph.h"
#include "generate_graph.h"

#include <math.h>

class Network
{
  private:
    class Edge;
    class Bend;

  public:
    Network(Graph);
    std::vector<vec2> initPositions;
    std::vector<Edge> edges;
    std::vector<Bend> bends;

    void forces( const std::vector<vec2>& positions, 
                std::vector<vec2>& forces);

    

  private:
    class Edge{
      public:
        int i,j; 
        int xb, yb;
        double l0; 
    };
    class Bend{
    };

    void forceEdge( int ei, const std::vector<vec2> &p,
                    std::vector<vec2> &f);

    double k;
    double Lx, Ly;
    int Nv,Ne,Nb;
};

Network::Network(Graph g)
{
    Nv = g.Nvertices();
    initPositions = std::vector<vec2>(Nv);
    for(int vi=0; vi<Nv; ++vi ) {
        initPositions[vi] = g.getVertexPosition(vi);
    }

    std::vector<std::vector<int> > e = g.getEdges();
    Ne = e.size();
    for(unsigned long int ei=0; ei< e.size(); ++ei ) {
       edges[ei].i = e[ei][0]; 
       edges[ei].j = e[ei][1]; 
       edges[ei].xb = e[ei][2]; 
       edges[ei].yb = e[ei][3]; 
       edges[ei].l0 = e[ei][4]; 
    }

}


void Network::forces( const std::vector<vec2>& positions, 
                std::vector<vec2>& forces)
{
    for(int i=0;i<Nv; ++i) {
        forces[i].x = 0;
        forces[i].y = 0;
    }

    for(int ei=0; ei<Ne; ++ei) {
        forceEdge(ei,positions, forces);
    }

}

void Network::forceEdge( int ei, const std::vector<vec2> &p,
                    std::vector<vec2> &f)
{
    int i = edges[ei].i;
    int j = edges[ei].j;

    vec2 d( p[i].x - p[j].x + edges[ei].xb*Lx,
            p[i].y - p[j].y + edges[ei].yb*Ly);

    double l = std::sqrt( d.x*d.x + d.y*d.y );
    d.x /= l;
    d.y /= l;

    double kdl = k*(l-edges[ei].l0);

    f[i].x += d.x*kdl;
    f[i].y += d.y*kdl;

    f[j].x -= d.x*kdl;
    f[j].y -= d.y*kdl;
    
}


#endif
