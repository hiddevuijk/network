#ifndef GUARD_NETWORK_H
#define GUARD_NETWORK_H

#include "vec2.h"
#include "graph.h"
#include "generate_graph.h"

#include "/usr/include/gsl/gsl_multimin.h"

#include <math.h>


double energy( const gsl_vector *v, void *params);
void dEnergy( const gsl_vector *v, void *params, gsl_vector *df);
void EdE( const gsl_vector *x, void *params, double *f, gsl_vector *df);

class Network
{
  private:
    class Edge;
    class Bend;

  public:
    Network(const Graph&);

    void shear(double delta_gamma); 
    int get_Nv() const {return Nv; }
    int get_Nedges() const { return Ne; }
    int get_Nbends() const { return Nb; }
    
    double get_edgeEnergy( int ei, const gsl_vector *r) const
        { return edges[ei].energy(r); }

    std::vector<double> get_positions() const;
    void savePositions(std::ostream& out) const;
  private:
    class Edge{
      public:
        int i,j; 
        int xb, yb;
        double l0; 
        double k;
        double Lx,Ly;

        double energy(const gsl_vector *r) const;
    };
    class Bend{
    };

    void minimize();

    const gsl_multimin_fdfminimizer_type *T;
    gsl_multimin_fdfminimizer *s;

    gsl_vector *r;
    gsl_multimin_function_fdf functions;

    std::vector<Edge> edges;
    std::vector<Bend> bends;


    double k;
    double Lx, Ly;
    int Nv,Ne,Nb;

    double gamma;
    double maxIter = 100;
};

Network::Network(const Graph& g)
{
    
    Nv = g.Nvertices();

    T = gsl_multimin_fdfminimizer_conjugate_fr;
    s = gsl_multimin_fdfminimizer_alloc(T,2*Nv);

    r = gsl_vector_alloc(2*Nv);

    functions.n = 2*Nv;
    functions.f = &energy;
    functions.df = &dEnergy;
    functions.fdf = &EdE;
    functions.params = this;

    for(int vi=0; vi<Nv; ++vi ) {
        vec2 p = g.getVertexPosition(vi);
        gsl_vector_set( r, 2*vi  , p.x);   
        gsl_vector_set( r, 2*vi+1, p.y);   
    }

    std::vector<std::vector<int> > e = g.getEdges();
    Ne = e.size();
    edges = std::vector<Edge>(Ne);
    for(int ei=0; ei< Ne; ++ei ) {
        edges[ei].i = e[ei][0]; 
        edges[ei].j = e[ei][1]; 
        edges[ei].xb = e[ei][2]; 
        edges[ei].yb = e[ei][3]; 
        edges[ei].l0 = e[ei][4]; 
        edges[ei].k = k; 
        edges[ei].Lx = Lx; 
        edges[ei].Ly = Ly; 
    }
}

void Network::shear( double delta_gamma )
{
    gamma += delta_gamma;
    double x,y;
    //affine deformation
    for( int vi=0; vi<Nv; ++vi ) {
        y = gsl_vector_get(r,2*vi+1);
        x = gsl_vector_get(r,2*vi);
        gsl_vector_set( r, 2*vi, x+delta_gamma*y );
    }
    minimize();
}

void Network::minimize()
{
    // add params to network
    gsl_multimin_fdfminimizer_set(s, &functions, r, 0.1, 1e-4);

    int iter = 0;
    int status;
    do{
        iter++;    
        status = gsl_multimin_fdfminimizer_iterate(s);
        if( status ) break;
        std::cout << iter << std::endl;
        status = gsl_multimin_test_gradient( s->gradient, 1e-3);
        
    } while( status == GSL_CONTINUE && iter < maxIter);
}



std::vector<double> Network::get_positions() const
{
    std::vector<double> positions(2*Nv);
    for(int i=0; i<2*Nv; ++i) {
        positions[i] = gsl_vector_get(r,i);
    }

    return positions;
}

void Network::savePositions(std::ostream& out) const
{
    for(int i=0; i<Nv; ++i) {
        out << gsl_vector_get(r, 2*i) << '\t';
        out << gsl_vector_get(r, 2*i+1) << '\n';
    }
}


double Network::Edge::energy(const gsl_vector *r) const
{
    double dx = gsl_vector_get(r, 2*i) - gsl_vector_get(r,2*j) + Lx*xb;
    double dy = gsl_vector_get(r, 2*i+1) - gsl_vector_get(r,2*j+1) + Ly*yb;
    double l = std::sqrt( dx*dx + dy*dy);
    l -= l0;
    return 0.5*k*l*l;
}


/*
    energy functions
*/

double energy( const gsl_vector *v, void *params)
{
    Network *net = (Network *) params;
    double e=0;
    for(int ei=0; ei < net->get_Nedges(); ++ei) {
        e += net->get_edgeEnergy(ei, v);
    }

    return e;
}

void dEnergy( const gsl_vector *v, void *params, gsl_vector *df)
{ }

void EdE( const gsl_vector *x, void *params, double *f, gsl_vector *df)
{
    *f = energy(x,params);
    dEnergy( x, params, df);
}
#endif
