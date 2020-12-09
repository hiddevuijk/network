#ifndef GUARD_NETWORK_H
#define GUARD_NETWORK_H

/*
to do:
    -- write EdEnergy
    -- destructors
*/

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
    void shearAffine(double delta_gamma); 
    int get_Nv() const {return Nv; }
    int get_Nedges() const { return Ne; }
    int get_Nbends() const { return Nb; }

    double totalEnergy() const; 
    double get_edgeEnergy( int ei, const gsl_vector *r) const
        { return edges[ei].energy(r, this); }

    void get_edgeDEnergy( int ei, const gsl_vector *r, gsl_vector *df) const
        {  edges[ei].dEnergy(r, df, this); }

    double get_edgeEdE( int ei, const gsl_vector *r, gsl_vector *df) const
        { return edges[ei].energy_dEnergy(r, df, this); }

    std::vector<double> get_positions() const;
    void savePositions(std::ostream& out) const;

    void set_k(double k);
    void set_Lx(double Lxx);
    void set_Ly(double Lyy);
  private:
    class Edge{
      public:
        int i,j; 
        int xb, yb;
        double l0; 

        double energy(const gsl_vector *r, const Network *net) const;
        void dEnergy( const gsl_vector *r,  gsl_vector *df, const Network *net) const;
        double energy_dEnergy(const gsl_vector *r, gsl_vector *df, const Network *net) const;
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
  public:
    double k;
    double Lx, Ly;
    int Nv,Ne,Nb;

    double gamma;
    int maxIter = 1000000;
};

Network::Network(const Graph& g)
{
    
    Nv = g.Nvertices();

    //T = gsl_multimin_fdfminimizer_conjugate_fr;
    //T = gsl_multimin_fdfminimizer_conjugate_pr;
    //T = gsl_multimin_fdfminimizer_vector_bfgs;
    T = gsl_multimin_fdfminimizer_vector_bfgs2;
    //T = gsl_multimin_fdfminimizer_steepest_descent;

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
    }
}

void Network::shearAffine( double delta_gamma )
{
    gamma += delta_gamma;
    double x,y;
    //affine deformation
    for( int vi=0; vi<Nv; ++vi ) {
        x = gsl_vector_get(r,2*vi);
        y = gsl_vector_get(r,2*vi+1);
        gsl_vector_set( r, 2*vi, x+delta_gamma*y );
    }

    minimize();
}


void Network::shear( double delta_gamma )
{
    gamma += delta_gamma;
    minimize();
}

void Network::minimize()
{
    // add params to network
    gsl_multimin_fdfminimizer_set(s, &functions, r, 1e-5, 0.1);

    int iter = 0;
    int status;
    do{
        iter++;    
        status = gsl_multimin_fdfminimizer_iterate(s);
        if( status ) break;
        status = gsl_multimin_test_gradient( s->gradient, 1e-18);
        
    } while( status == GSL_CONTINUE && iter < maxIter);
    if( iter >= maxIter ) std::cout << "\t Fuck \n";

    // copy to r
    gsl_vector * x = gsl_multimin_fdfminimizer_x(s);
    for(int i=0;i<2*Nv; ++i) {
        gsl_vector_set(r,i, gsl_vector_get(x,i) );
    }
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

double Network::totalEnergy() const
{
    double e = 0;
    for(int i=0;i<Ne;++i) {
        e += get_edgeEnergy(i,r);
    }
    return e;
}

double Network::Edge::energy(const gsl_vector *r,const Network *net) const
{

    double dx = gsl_vector_get(r, 2*i) - gsl_vector_get(r,2*j) - (net->Lx)*xb - yb*net->gamma;
    double dy = gsl_vector_get(r, 2*i+1) - gsl_vector_get(r,2*j+1) - (net->Ly)*yb;


    double l = std::sqrt( dx*dx + dy*dy);
    l -= l0;
    return 0.5*(net->k)*l*l;
}

void Network::Edge::dEnergy( const gsl_vector *r, gsl_vector *df, const Network *net) const
{
    double dx = gsl_vector_get(r, 2*i) - gsl_vector_get(r,2*j) - (net->Lx)*xb - yb*net->gamma;
    double dy = gsl_vector_get(r, 2*i+1) - gsl_vector_get(r,2*j+1) - (net->Ly)*yb;
    double l = std::sqrt( dx*dx + dy*dy);

    dx *= (net->k)*(1-l0/l);
    dy *= (net->k)*(1-l0/l);

    double temp = gsl_vector_get(df, 2*i) + dx;
    gsl_vector_set(df, 2*i, temp );

    temp = gsl_vector_get(df, 2*j) - dx;
    gsl_vector_set(df, 2*j, temp );

    temp = gsl_vector_get(df, 2*i+1) + dy;
    gsl_vector_set(df, 2*i+1, temp);

    temp = gsl_vector_get(df, 2*j+1) - dy;
    gsl_vector_set(df, 2*j+1, temp);

}

double Network::Edge::energy_dEnergy(const gsl_vector *r, gsl_vector *df, const Network *net) const
{

    double dx = gsl_vector_get(r, 2*i) - gsl_vector_get(r,2*j) - (net->Lx)*xb - yb*net->gamma;
    double dy = gsl_vector_get(r, 2*i+1) - gsl_vector_get(r,2*j+1) - (net->Ly)*yb;
    double l = std::sqrt( dx*dx + dy*dy);

    dx *= (net->k)*(1-l0/l);
    dy *= (net->k)*(1-l0/l);

    double temp = gsl_vector_get(df, 2*i) + dx;
    gsl_vector_set(df, 2*i, temp );

    temp = gsl_vector_get(df, 2*j) - dx;
    gsl_vector_set(df, 2*j, temp );

    temp = gsl_vector_get(df, 2*i+1) + dy;
    gsl_vector_set(df, 2*i+1, temp);

    temp = gsl_vector_get(df, 2*j+1) - dy;
    gsl_vector_set(df, 2*j+1, temp);



    l -= l0;
    return 0.5*(net->k)*l*l;

}

void Network::set_k(double kk)
{ k = kk; }

void Network::set_Lx(double Lxx)
{ Lx = Lxx; }
void Network::set_Ly(double Lyy)
{ Ly = Lyy; }

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
{
    
    Network *net = (Network *) params; 
    gsl_vector_scale( df, 0); // set all elements to 0
    for( int ei=0; ei< net->get_Nedges(); ++ei ) {
        net->get_edgeDEnergy(ei, v, df);
    } 
}

void EdE( const gsl_vector *v, void *params, double *f, gsl_vector *df)
{
    *f = energy(v,params);
    dEnergy( v, params, df);

    Network *net = (Network *) params;
    double e = 0;
    gsl_vector_scale( df, 0); // set all elements to 0
    for(int ei=0; ei < net->get_Nedges(); ++ei) {
        e += net->get_edgeEdE(ei, v, df);
    }
    *f = e;
}
#endif
